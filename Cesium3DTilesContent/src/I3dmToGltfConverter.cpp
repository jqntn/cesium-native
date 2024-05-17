// Heavily inspired by PntsToGltfConverter.cpp

#include <Cesium3DTilesContent/BinaryToGltfConverter.h>
#include <Cesium3DTilesContent/I3dmToGltfConverter.h>
#include <Cesium3DTilesContent/LegacyUtilities.h>
#include <CesiumGeospatial/LocalHorizontalCoordinateSystem.h>
#include <CesiumGltf/AccessorUtility.h>
#include <CesiumGltf/AccessorView.h>
#include <CesiumGltf/ExtensionExtMeshGpuInstancing.h>
#include <CesiumGltf/Model.h>
#include <CesiumGltf/PropertyTransformations.h>
#include <CesiumGltfContent/GltfUtilities.h>
#include <CesiumUtility/AttributeCompression.h>

#include <glm/gtx/matrix_decompose.hpp>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <optional>
#include <set>

using namespace CesiumGltf;

namespace Cesium3DTilesContent {
using namespace LegacyUtilities;

namespace {
struct I3dmHeader {
  unsigned char magic[4] = {0, 0, 0, 0};
  uint32_t version = 0;
  uint32_t byteLength = 0;
  uint32_t featureTableJsonByteLength = 0;
  uint32_t featureTableBinaryByteLength = 0;
  uint32_t batchTableJsonByteLength = 0;
  uint32_t batchTableBinaryByteLength = 0;
  uint32_t gltfFormat = 0;
};

struct DecodedInstances {
  std::vector<glm::vec3> positions;
  std::vector<glm::fquat> rotations;
  std::vector<glm::vec3> scales;
  std::string gltfUri;
  bool rotationENU = false;
  std::optional<glm::dvec3> rtcCenter;
};

// Instance positions may arrive in ECEF coordinates or with other large
// displacements that will cause problems during rendering. Determine the mean
// position of the instances and reposition them relative to it, thus creating
// a new RTC center.
//
// If an RTC center value is already present, then the newly-computed center is
// added to it.

void repositionInstances(DecodedInstances& decodedInstances) {
  if (decodedInstances.positions.empty()) {
    return;
  }
  glm::dvec3 newCenter(0.0, 0.0, 0.0);
  for (const auto& pos : decodedInstances.positions) {
    newCenter += glm::dvec3(pos);
  }
  newCenter /= static_cast<double>(decodedInstances.positions.size());
  std::transform(
      decodedInstances.positions.begin(),
      decodedInstances.positions.end(),
      decodedInstances.positions.begin(),
      [&](const glm::vec3& pos) {
        return glm::vec3(glm::dvec3(pos) - newCenter);
      });
  if (decodedInstances.rtcCenter) {
    newCenter += *decodedInstances.rtcCenter;
  }
  decodedInstances.rtcCenter = newCenter;
}

void parseI3dmHeader(
    const gsl::span<const std::byte>& instancesBinary,
    I3dmHeader& header,
    uint32_t& headerLength,
    GltfConverterResult& result) {
  if (instancesBinary.size() < sizeof(I3dmHeader)) {
    result.errors.emplaceError("The I3DM is invalid because it is too small to "
                               "include a I3DM header.");
    return;
  }

  const I3dmHeader* pHeader =
      reinterpret_cast<const I3dmHeader*>(instancesBinary.data());

  header = *pHeader;
  headerLength = sizeof(I3dmHeader);

  if (pHeader->version != 1) {
    result.errors.emplaceError(fmt::format(
        "The I3DM file is version {}, which is unsupported.",
        pHeader->version));
    return;
  }

  if (static_cast<uint32_t>(instancesBinary.size()) < pHeader->byteLength) {
    result.errors.emplaceError(
        "The I3DM is invalid because the total data available is less than the "
        "size specified in its header.");
    return;
  }
}

struct I3dmContent {
  uint32_t instancesLength = 0;
  std::optional<glm::dvec3> rtcCenter;
  std::optional<glm::dvec3> quantizedVolumeOffset;
  std::optional<glm::dvec3> quantizedVolumeScale;
  bool eastNorthUp = false;

  // Offsets into the feature table.
  std::optional<uint32_t> position;
  std::optional<uint32_t> positionQuantized;
  std::optional<uint32_t> normalUp;
  std::optional<uint32_t> normalRight;
  std::optional<uint32_t> normalUpOct32p;
  std::optional<uint32_t> normalRightOct32p;
  std::optional<uint32_t> scale;
  std::optional<uint32_t> scaleNonUniform;
  std::optional<uint32_t> batchId;
  CesiumUtility::ErrorList errors;
};

glm::vec3 decodeOct32P(const uint16_t rawOct[2]) {
  glm::dvec3 result = CesiumUtility::AttributeCompression::octDecodeInRange(
      rawOct[0],
      rawOct[1],
      static_cast<uint16_t>(65535));
  return glm::vec3(result);
}

/*
  Calculate the rotation quaternion described by the up, right vectors passed
  in NORMAL_UP and NORMAL_RIGHT. This is composed of two rotations:
   + The rotation that takes the up vector to its new position;
   + The rotation around the new up vector that takes the right vector to its
  new position.

  I like to think of each rotation as describing a coordinate frame. The
  calculation of the second rotation must take place within the first frame.

  The rotations are calculated by finding the rotation that takes one vector to
  another. If we take the dot and cross products of the two vectors and store
  them in a quaternion, that quaternion represents twice the required rotation.
  We get the correct quaternion by "averaging" with the zero rotation
  quaternion, in a way analagous to finding the half vector between two 3D
  vectors.
 */

glm::quat rotation(const glm::vec3& vec1, const glm::vec3& vec2) {
  float cosRot = dot(vec1, vec2);
  // Not using epsilon for these tests. If abs(cosRot) < 1.0, we can still
  // create a sensible rotation.
  if (cosRot >= 1.0f) {
    // zero rotation
    return glm::quat(1.0, 0.0f, 0.0f, 0.0f);
  }
  if (cosRot <= -1.0f) {
    // Choose principal axis that is "most orthogonal" to the first vector.
    glm::vec3 orthoVec(0.0f, 0.0f, 0.0f);
    glm::vec3 absVals(std::fabs(vec1.x), std::fabs(vec1.y), std::fabs(vec1.z));
    if (absVals.z <= absVals.x && absVals.z <= absVals.y) {
      orthoVec.z = 1.0f;
    } else if (absVals.x <= absVals.y) {
      orthoVec.x = 1.0f;
    } else {
      orthoVec.y = 1.0f;
    }
    auto rotAxis = cross(vec1, orthoVec);
    rotAxis = normalize(rotAxis);
    // rotation by pi radians
    return glm::quat(0.0f, rotAxis.x, rotAxis.y, rotAxis.z);
  }
  auto rotAxis = cross(vec1, vec2);
  glm::quat sumQuat(cosRot + 1.0f, rotAxis.x, rotAxis.y, rotAxis.z);
  return normalize(sumQuat);
}

glm::quat rotationFromUpRight(const glm::vec3& up, const glm::vec3& right) {
  // First rotation: up
  auto upRot = rotation(glm::vec3(0.0f, 1.0f, 0.0f), up);
  // We can rotate a point vector by a quaternion using q * (0, v) *
  // conj(q). But here we are doing an inverse rotation of the right vector into
  // the "up frame."
  glm::quat temp =
      conjugate(upRot) * glm::quat(0.0f, right.x, right.y, right.z) * upRot;
  glm::vec3 innerRight(temp.x, temp.y, temp.z);
  glm::quat rightRot = rotation(glm::vec3(1.0f, 0.0f, 0.0f), innerRight);
  return upRot * rightRot;
}

struct ConvertedI3dm {
  GltfConverterResult gltfResult;
  DecodedInstances decodedInstances;
};

/* The approach:
  + Parse the i3dm header, decoding and creating all the instance transforms.
  This includes "exotic" things like OCT encoding of rotations and ENU rotations
  for each instance.
  + For each node with a mesh (a "mesh node"), the instance transforms must be
    transformed into the local coordinates of the mesh and then stored in the
    EXT_mesh_gpu_instancing extension for that node. It would be nice to avoid a
    lot of duplicate data for mesh nodes with the same chain of transforms to
    the tile root. One way to do this would be to store the accessors in a hash
    table, hashed by mesh transform.
  + Add the instance transforms to the glTF buffers, buffer views, and
    accessors.
  + Future work: Metadata / feature id?
*/

CesiumAsync::Future<ConvertedI3dm> convertI3dmContent(
    const gsl::span<const std::byte>& instancesBinary,
    const I3dmHeader& header,
    uint32_t headerLength,
    const CesiumGltfReader::GltfReaderOptions& options,
    const AssetFetcher& assetFetcher,
    GltfConverterResult& result) {
  ConvertedI3dm convertedI3dm;
  DecodedInstances& decodedInstances = convertedI3dm.decodedInstances;
  convertedI3dm.gltfResult = result;
  auto finishEarly = [&]() {
    return assetFetcher.asyncSystem.createResolvedFuture(std::move(convertedI3dm));
  };
  if (header.featureTableJsonByteLength == 0 ||
      header.featureTableBinaryByteLength == 0) {
    return finishEarly();
  }
  const uint32_t gltfStart = headerLength + header.featureTableJsonByteLength +
                             header.featureTableBinaryByteLength +
                             header.batchTableJsonByteLength +
                             header.batchTableBinaryByteLength;
  const uint32_t gltfEnd = header.byteLength;
  auto gltfData = instancesBinary.subspan(gltfStart, gltfEnd - gltfStart);
  std::optional<CesiumAsync::Future<ByteResult>> assetFuture;
  auto featureTableJsonData =
      instancesBinary.subspan(headerLength, header.featureTableJsonByteLength);
  rapidjson::Document featureTableJson;
  featureTableJson.Parse(
      reinterpret_cast<const char*>(featureTableJsonData.data()),
      featureTableJsonData.size());
  if (featureTableJson.HasParseError()) {
    convertedI3dm.gltfResult.errors.emplaceError(fmt::format(
        "Error when parsing feature table JSON, error code {} at byte offset "
        "{}",
        featureTableJson.GetParseError(),
        featureTableJson.GetErrorOffset()));
    return finishEarly();
  }
  I3dmContent parsedContent;
  // Global semantics
  if (auto optInstancesLength =
          getValue<uint32_t>(featureTableJson, "INSTANCES_LENGTH")) {
    parsedContent.instancesLength = *optInstancesLength;
  } else {
    convertedI3dm.gltfResult.errors.emplaceError(
        "Error parsing I3DM feature table, no valid "
        "INSTANCES_LENGTH was found.");
    return finishEarly();
  }
  parsedContent.rtcCenter =
      parseArrayValueDVec3(featureTableJson, "RTC_CENTER");
  decodedInstances.rtcCenter = parsedContent.rtcCenter;

  parsedContent.position =
      parseOffsetForSemantic(
          featureTableJson, "POSITION", convertedI3dm.gltfResult.errors);
  if (!parsedContent.position) {
    if (convertedI3dm.gltfResult.errors.hasErrors()) {
      return finishEarly();
    }
    parsedContent.positionQuantized = parseOffsetForSemantic(
        featureTableJson,
        "POSITION_QUANTIZED",
        convertedI3dm.gltfResult.errors);
    if (convertedI3dm.gltfResult.errors.hasErrors()) {
      return finishEarly();
    }
  }
  if (parsedContent.positionQuantized) {
    parsedContent.quantizedVolumeOffset =
        parseArrayValueDVec3(featureTableJson, "QUANTIZED_VOLUME_OFFSET");
    if (!parsedContent.quantizedVolumeOffset) {
      convertedI3dm.gltfResult.errors.emplaceError(
          "Error parsing I3DM feature table, the I3dm uses quatized positions "
          "but has no valid QUANTIZED_VOLUME_OFFSET property");
      return finishEarly();
    }
    parsedContent.quantizedVolumeScale =
        parseArrayValueDVec3(featureTableJson, "QUANTIZED_VOLUME_SCALE");
    if (!parsedContent.quantizedVolumeScale) {
      convertedI3dm.gltfResult.errors.emplaceError(
          "Error parsing I3DM feature table, the I3dm uses quatized positions "
          "but has no valid QUANTIZED_VOLUME_SCALE property");
      return finishEarly();
    }
  }
  decodedInstances.rotationENU = false;
  if (auto optENU = getValue<bool>(featureTableJson, "EAST_NORTH_UP")) {
    parsedContent.eastNorthUp = *optENU;
    decodedInstances.rotationENU = *optENU;
  }
  parsedContent.normalUp =
      parseOffsetForSemantic(
          featureTableJson, "NORMAL_UP", convertedI3dm.gltfResult.errors);
  parsedContent.normalRight = parseOffsetForSemantic(
      featureTableJson,
      "NORMAL_RIGHT",
      convertedI3dm.gltfResult.errors);
  parsedContent.normalUpOct32p = parseOffsetForSemantic(
      featureTableJson,
      "NORMAL_UP_OCT32P",
      convertedI3dm.gltfResult.errors);
  parsedContent.normalRightOct32p = parseOffsetForSemantic(
      featureTableJson,
      "NORMAL_RIGHT_OCT32P",
      convertedI3dm.gltfResult.errors);
  parsedContent.scale =
      parseOffsetForSemantic(
          featureTableJson, "SCALE", convertedI3dm.gltfResult.errors);
  parsedContent.scaleNonUniform = parseOffsetForSemantic(
      featureTableJson,
      "SCALE_NON_UNIFORM",
      convertedI3dm.gltfResult.errors);
  parsedContent.batchId =
      parseOffsetForSemantic(
          featureTableJson, "BATCH_ID", convertedI3dm.gltfResult.errors);
  if (convertedI3dm.gltfResult.errors.hasErrors()) {
    return finishEarly();
  }
  auto featureTableBinaryData = instancesBinary.subspan(
      headerLength + header.featureTableJsonByteLength,
      header.featureTableBinaryByteLength);
  decodedInstances.positions.resize(
      parsedContent.instancesLength,
      glm::vec3(0.0f, 0.0f, 0.0f));
  if (parsedContent.position) {
    const auto* rawPosition = reinterpret_cast<const glm::vec3*>(
        featureTableBinaryData.data() + *parsedContent.position);
    for (unsigned i = 0; i < parsedContent.instancesLength; ++i) {
      decodedInstances.positions[i] += rawPosition[i];
    }
  } else {
    const auto* rawQPosition = reinterpret_cast<const uint16_t(*)[3]>(
        featureTableBinaryData.data() + *parsedContent.positionQuantized);
    for (unsigned i = 0; i < parsedContent.instancesLength; ++i) {
      const auto* posQuantized = &rawQPosition[i];
      float position[3];
      for (unsigned j = 0; j < 3; ++j) {
        position[j] = static_cast<float>(
            (*posQuantized)[j] / 65535.0 *
                (*parsedContent.quantizedVolumeScale)[j] +
            (*parsedContent.quantizedVolumeOffset)[j]);
      }
      decodedInstances.positions[i] +=
          glm::vec3(position[0], position[1], position[2]);
    }
  }
  decodedInstances.rotations.resize(
      parsedContent.instancesLength,
      glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
  if (parsedContent.normalUp && parsedContent.normalRight) {
    const auto* rawUp = reinterpret_cast<const glm::vec3*>(
        featureTableBinaryData.data() + *parsedContent.normalUp);
    const auto* rawRight = reinterpret_cast<const glm::vec3*>(
        featureTableBinaryData.data() + *parsedContent.normalRight);
    for (unsigned i = 0; i < parsedContent.instancesLength; ++i) {
      decodedInstances.rotations[i] =
          rotationFromUpRight(rawUp[i], rawRight[i]);
    }
  } else if (parsedContent.normalUpOct32p && parsedContent.normalRightOct32p) {
    const auto* rawUpOct = reinterpret_cast<const uint16_t(*)[2]>(
        featureTableBinaryData.data() + *parsedContent.normalUpOct32p);
    const auto* rawRightOct = reinterpret_cast<const uint16_t(*)[2]>(
        featureTableBinaryData.data() + *parsedContent.normalRightOct32p);
    for (unsigned i = 0; i < parsedContent.instancesLength; ++i) {
      glm::vec3 dUp = decodeOct32P(rawUpOct[i]);
      glm::vec3 dRight = decodeOct32P(rawRightOct[i]);
      decodedInstances.rotations[i] = rotationFromUpRight(dUp, dRight);
    }
  } else if (decodedInstances.rotationENU) {
    glm::dmat4 worldTransform = assetFetcher.tileTransform;
    if (decodedInstances.rtcCenter) {
      worldTransform = translate(worldTransform, *decodedInstances.rtcCenter);
    }
    auto worldTransformInv = inverse(worldTransform);
    for (size_t i = 0; i < decodedInstances.positions.size(); ++i) {
      auto worldPos =
          worldTransform * glm::dvec4(decodedInstances.positions[i], 1.0);
      CesiumGeospatial::LocalHorizontalCoordinateSystem enu(
          (glm::dvec3(worldPos)));
      const auto& ecef = enu.getLocalToEcefTransformation();
      // back into tile coordinate system
      auto tileFrame = worldTransformInv * ecef;
      glm::quat tileFrameRot =
          rotationFromUpRight(glm::vec3(tileFrame[1]), glm::vec3(tileFrame[0]));
      decodedInstances.rotations[i] = tileFrameRot;
    }
  }
  decodedInstances.scales.resize(
      parsedContent.instancesLength,
      glm::vec3(1.0, 1.0, 1.0));
  if (parsedContent.scale) {
    const auto* rawScale = reinterpret_cast<const float*>(
        featureTableBinaryData.data() + *parsedContent.scale);
    for (unsigned i = 0; i < parsedContent.instancesLength; ++i) {
      decodedInstances.scales[i] =
          glm::vec3(rawScale[i], rawScale[i], rawScale[i]);
    }
  } else if (parsedContent.scaleNonUniform) {
    const auto* rawScaleNonUniform = reinterpret_cast<const glm::vec3*>(
        featureTableBinaryData.data() + *parsedContent.scaleNonUniform);
    for (unsigned i = 0; i < parsedContent.instancesLength; ++i) {
      decodedInstances.scales[i] = rawScaleNonUniform[i];
    }
  }
  repositionInstances(decodedInstances);
  ByteResult byteResult;
  if (header.gltfFormat == 0) {
    // Recursively fetch and read the glTF content.
    auto gltfUri = std::string(
        reinterpret_cast<const char*>(gltfData.data()),
        gltfData.size());
    return assetFetcher.get(gltfUri)
        .thenImmediately(
            [options, assetFetcher](ByteResult&& byteResult)
                -> CesiumAsync::Future<GltfConverterResult> {
              if (byteResult.errorList.hasErrors()) {
                GltfConverterResult errorResult;
                errorResult.errors.merge(byteResult.errorList);
                return assetFetcher.asyncSystem.createResolvedFuture(
                    std::move(errorResult));
              }
              return BinaryToGltfConverter::convert(
                  byteResult.bytes,
                  options,
                  assetFetcher);
            })
        .thenImmediately([convertedI3dm = std::move(convertedI3dm)](
                             GltfConverterResult&& converterResult) mutable {
          if (converterResult.errors.hasErrors()) {
            convertedI3dm.gltfResult.errors.merge(converterResult.errors);
          } else {
            convertedI3dm.gltfResult = converterResult;
          }
          return convertedI3dm;
        });
  } else {
    return BinaryToGltfConverter::convert(gltfData, options, assetFetcher)
        .thenImmediately([convertedI3dm = std::move(convertedI3dm)](
                             GltfConverterResult&& converterResult) mutable {
          if (converterResult.errors.hasErrors()) {
            return convertedI3dm;
          }
          convertedI3dm.gltfResult = converterResult;
          return convertedI3dm;
        });
  }
}

glm::dmat4
composeInstanceTransform(size_t i, const DecodedInstances& decodedInstances) {
  glm::dmat4 result(1.0);
  if (!decodedInstances.positions.empty()) {
    result = translate(result, glm::dvec3(decodedInstances.positions[i]));
  }
  if (!decodedInstances.rotations.empty()) {
    result = result * toMat4(glm::dquat(decodedInstances.rotations[i]));
  }
  if (!decodedInstances.scales.empty()) {
    result = scale(result, glm::dvec3(decodedInstances.scales[i]));
  }
  return result;
}

std::vector<glm::dmat4> meshGpuInstances(
    GltfConverterResult& result,
    const ExtensionExtMeshGpuInstancing& gpuExt) {
  const Model& model = *result.model;
  std::vector<glm::dmat4> instances;
  if (gpuExt.attributes.empty()) {
    return instances;
  }
  auto getInstanceAccessor = [&](const char* name) -> const Accessor* {
    if (auto accessorItr = gpuExt.attributes.find(name);
        accessorItr != gpuExt.attributes.end()) {
      return Model::getSafe(&model.accessors, accessorItr->second);
    }
    return nullptr;
  };
  auto errorOut = [&](const std::string& errorMsg) {
    result.errors.emplaceError(errorMsg);
    return instances;
  };

  const Accessor* translations = getInstanceAccessor("TRANSLATION");
  const Accessor* rotations = getInstanceAccessor("ROTATION");
  const Accessor* scales = getInstanceAccessor("SCALE");
  int64_t count = 0;
  if (translations) {
    count = translations->count;
  }
  if (rotations) {
    if (count == 0) {
      count = rotations->count;
    } else if (count != rotations->count) {
      return errorOut(fmt::format(
          "instance rotation count {} not consistent with {}",
          rotations->count,
          count));
    }
  }
  if (scales) {
    if (count == 0) {
      count = scales->count;
    } else if (count != scales->count) {
      return errorOut(fmt::format(
          "instance scale count {} not consistent with {}",
          scales->count,
          count));
    }
  }
  if (count == 0) {
    return errorOut("No valid instance data");
  }
  instances.resize(static_cast<size_t>(count), glm::dmat4(1.0));
  if (translations) {
    AccessorView<CesiumGltf::AccessorTypes::VEC3<float>> translationAccessor(
        model,
        *translations);
    if (translationAccessor.status() == AccessorViewStatus::Valid) {
      for (unsigned i = 0; i < count; ++i) {
        auto transVec = toGlm<glm::dvec3>(translationAccessor[i]);
        instances[i] = glm::translate(instances[i], transVec);
      }
    } else {
      return errorOut("invalid accessor for instance translations");
    }
  }
  if (rotations) {
    auto quatAccessorView = getQuaternionAccessorView(model, rotations);
    std::visit(
        [&](auto&& arg) {
          for (unsigned i = 0; i < count; ++i) {
            auto quat = toGlmQuat<glm::dquat>(arg[i]);
            instances[i] = instances[i] * glm::toMat4(quat);
          }
        },
        quatAccessorView);
  }
  if (scales) {
    AccessorView<CesiumGltf::AccessorTypes::VEC3<float>> scaleAccessor(
        model,
        *scales);
    if (scaleAccessor.status() == AccessorViewStatus::Valid) {
      for (unsigned i = 0; i < count; ++i) {
        auto scaleFactors = toGlm<glm::dvec3>(scaleAccessor[i]);
        instances[i] = glm::scale(instances[i], scaleFactors);
      }
    } else {
      return errorOut("invalid accessor for instance translations");
    }
  }
  return instances;
}

const size_t rotOffset = sizeof(glm::vec3);
const size_t scaleOffset = rotOffset + sizeof(glm::quat);
const size_t totalStride =
    sizeof(glm::vec3) + sizeof(glm::quat) + sizeof(glm::vec3);

void copyToBuffer(
    const glm::dvec3& position,
    const glm::dquat& rotation,
    const glm::dvec3& scale,
    std::byte* bufferLoc) {
  glm::vec3 fposition(position);
  std::memcpy(bufferLoc, &fposition, sizeof(fposition));
  glm::quat frotation(rotation);
  std::memcpy(bufferLoc + rotOffset, &frotation, sizeof(frotation));
  glm::vec3 fscale(scale);
  std::memcpy(bufferLoc + scaleOffset, &fscale, sizeof(fscale));
}

void copyToBuffer(
    const glm::dvec3& position,
    const glm::dquat& rotation,
    const glm::dvec3& scale,
    std::vector<std::byte>& bufferData,
    size_t i) {
  copyToBuffer(position, rotation, scale, &bufferData[i * totalStride]);
}

void instantiateGltfInstances(
    GltfConverterResult& result,
    const DecodedInstances& decodedInstances) {
  std::set<CesiumGltf::Node*> meshNodes;
  int32_t instanceBufferId = createBufferInGltf(*result.model);
  auto& instanceBuffer =
      result.model->buffers[static_cast<uint32_t>(instanceBufferId)];
  int32_t instanceBufferViewId = createBufferViewInGltf(
      *result.model,
      instanceBufferId,
      0,
      static_cast<int64_t>(totalStride));
  auto& instanceBufferView =
      result.model->bufferViews[static_cast<uint32_t>(instanceBufferViewId)];
  const auto numInstances =
      static_cast<uint32_t>(decodedInstances.positions.size());
  auto upToZ = CesiumGltfContent::GltfUtilities::applyGltfUpAxisTransform(
      *result.model,
      glm::dmat4x4(1.0));
  result.model->forEachPrimitiveInScene(
      -1,
      [&](Model& gltf,
          Node& node,
          Mesh&,
          MeshPrimitive&,
          const glm::dmat4& transform) {
        auto [nodeItr, notSeen] = meshNodes.insert(&node);
        if (!notSeen) {
          return;
        }
        std::vector<glm::dmat4> existingInstances{glm::dmat4(1.0)};
        auto& gpuExt = node.addExtension<ExtensionExtMeshGpuInstancing>();
        if (!gpuExt.attributes.empty()) {
          // The model already has instances! We will need to create the outer
          // product of these instances and those coming from i3dm.
          existingInstances = meshGpuInstances(result, gpuExt);
          if (numInstances * existingInstances.size() >
              std::numeric_limits<uint32_t>::max()) {
            result.errors.emplaceError(fmt::format(
                "Too many instances: {} from i3dm and {} from glb",
                numInstances,
                existingInstances.size()));
            return;
          }
        }
        const uint32_t numNewInstances =
            static_cast<uint32_t>(numInstances * existingInstances.size());
        const size_t instanceDataSize = totalStride * numNewInstances;
        auto dataBaseOffset =
            static_cast<uint32_t>(instanceBuffer.cesium.data.size());
        instanceBuffer.cesium.data.resize(dataBaseOffset + instanceDataSize);
        // Transform instance transform into local glTF coordinate system.
        const auto toTile = upToZ * transform;
        const auto toTileInv = inverse(toTile);
        size_t destInstanceIndx = 0;
        for (unsigned i = 0; i < numInstances; ++i) {
          auto instMat = composeInstanceTransform(i, decodedInstances);
          instMat = toTileInv * instMat * toTile;
          for (const auto& innerMat : existingInstances) {
            auto finalMat = instMat * innerMat;
            glm::dvec3 position, scale, skew;
            glm::dquat rotation;
            glm::dvec4 perspective;
            decompose(finalMat, scale, rotation, position, skew, perspective);
            copyToBuffer(
                position,
                rotation,
                scale,
                instanceBuffer.cesium.data,
                destInstanceIndx++);
          }
        }
        auto posAccessorId = createAccessorInGltf(
            gltf,
            instanceBufferViewId,
            Accessor::ComponentType::FLOAT,
            numNewInstances,
            Accessor::Type::VEC3);
        auto& posAcessor = gltf.accessors[static_cast<uint32_t>(posAccessorId)];
        posAcessor.byteOffset = dataBaseOffset;
        gpuExt.attributes["TRANSLATION"] = posAccessorId;
        auto rotAccessorId = createAccessorInGltf(
            gltf,
            instanceBufferViewId,
            Accessor::ComponentType::FLOAT,
            numInstances,
            Accessor::Type::VEC4);
        auto& rotAccessor =
            gltf.accessors[static_cast<uint32_t>(rotAccessorId)];
        rotAccessor.byteOffset =
            static_cast<int64_t>(dataBaseOffset + rotOffset);
        gpuExt.attributes["ROTATION"] = rotAccessorId;
        auto scaleAccessorId = createAccessorInGltf(
            gltf,
            instanceBufferViewId,
            Accessor::ComponentType::FLOAT,
            numInstances,
            Accessor::Type::VEC3);
        auto& scaleAccessor =
            gltf.accessors[static_cast<uint32_t>(scaleAccessorId)];
        scaleAccessor.byteOffset =
            static_cast<int64_t>(dataBaseOffset + scaleOffset);
        gpuExt.attributes["SCALE"] = scaleAccessorId;
      });
  if (decodedInstances.rtcCenter) {
    applyRTC(*result.model, *decodedInstances.rtcCenter);
  }
  instanceBuffer.byteLength =
      static_cast<int64_t>(instanceBuffer.cesium.data.size());
  instanceBufferView.byteLength = instanceBuffer.byteLength;
}
} // namespace

CesiumAsync::Future<GltfConverterResult> I3dmToGltfConverter::convert(
    const gsl::span<const std::byte>& instancesBinary,
    const CesiumGltfReader::GltfReaderOptions& options,
    const AssetFetcher& assetFetcher) {
  GltfConverterResult result;
  I3dmHeader header;
  uint32_t headerLength = 0;
  parseI3dmHeader(instancesBinary, header, headerLength, result);
  if (result.errors) {
    return assetFetcher.asyncSystem.createResolvedFuture(std::move(result));
  }
  DecodedInstances decodedInstances;
  return convertI3dmContent(
             instancesBinary,
             header,
             headerLength,
             options,
             assetFetcher,
             result)
      .thenImmediately([](ConvertedI3dm&& convertedI3dm) {
        if (convertedI3dm.gltfResult.errors.hasErrors()) {
          return convertedI3dm.gltfResult;
        }
        instantiateGltfInstances(
            convertedI3dm.gltfResult,
            convertedI3dm.decodedInstances);
        return convertedI3dm.gltfResult;
      });
}
} // namespace Cesium3DTilesContent
