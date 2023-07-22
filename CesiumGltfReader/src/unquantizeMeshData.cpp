#include "unquantizeMeshData.h"

#include "CesiumGltfReader/GltfReader.h"

#include <CesiumGltf/AccessorView.h>

#include <meshoptimizer.h>

using namespace CesiumGltf;

namespace CesiumGltfReader {

namespace {
template <typename T, size_t N>
using xVec = AccessorView<glm::vec<N, T, glm::defaultp>>;

template <typename T> float intToFloat(T t) { return intToFloat(t); }

template <> float intToFloat(std::int8_t c) {
  return std::max(c / 127.0f, -1.0f);
}

template <> float intToFloat(std::uint8_t c) { return c / 127.0f; }

template <> float intToFloat(std::int16_t c) {
  return std::max(c / 65535.0f, -1.0f);
}

template <> float intToFloat(std::uint16_t c) { return c / 65535.0f; }

template <typename T, size_t N>
void unquantizeFloat(float* fPtr, const xVec<T, N>& quantizedView) {
  for (int i = 0; i < quantizedView.size(); i++) {
    const auto& q = quantizedView[i];
    for (unsigned int j = 0; j < N; j++) {
      *fPtr++ = intToFloat<T>(q[j]);
    }
  }
}

template <typename T, size_t N>
void unquantizeAccessor(Model& model, Accessor& accessor) {
  xVec<T, N> quantizedView(model, accessor);
  if (quantizedView.status() != AccessorViewStatus::Valid) {
    return;
  }
  if (quantizedView.size() != accessor.count) {
    return;
  }
  Buffer& buffer = model.buffers.emplace_back();
  int64_t byteLength = accessor.count * N * sizeof(float);
  buffer.byteLength = byteLength;
  buffer.cesium.data.resize(byteLength);

  accessor.componentType = AccessorSpec::ComponentType::FLOAT;
  accessor.byteOffset = 0;
  for (double& d : accessor.min) {
    d = intToFloat<T>(static_cast<T>(d));
  }
  for (double& d : accessor.max) {
    d = intToFloat<T>(static_cast<T>(d));
  }

  BufferView* pBufferView =
      Model::getSafe(&model.bufferViews, accessor.bufferView);
  pBufferView->buffer = static_cast<int32_t>(model.buffers.size() - 1);
  pBufferView->byteOffset = 0;
  pBufferView->byteStride = N * sizeof(float);
  pBufferView->byteLength = byteLength;

  unquantizeFloat(
      reinterpret_cast<float*>(buffer.cesium.data.data()),
      quantizedView);
}

template <size_t N> void unquantizeAccessor(Model& model, Accessor& accessor) {
  switch (accessor.componentType) {
  case Accessor::ComponentType::BYTE:
    unquantizeAccessor<std::int8_t, N>(model, accessor);
    break;
  case Accessor::ComponentType::UNSIGNED_BYTE:
    unquantizeAccessor<std::uint8_t, N>(model, accessor);
    break;
  case Accessor::ComponentType::SHORT:
    unquantizeAccessor<std::int16_t, N>(model, accessor);
    break;
  case Accessor::ComponentType::UNSIGNED_SHORT:
    unquantizeAccessor<std::uint16_t, N>(model, accessor);
    break;
  }
}

void unquantizeAccessor(Model& model, Accessor& accessor) {
  int8_t numberOfComponents = accessor.computeNumberOfComponents();
  switch (numberOfComponents) {
  case 2:
    unquantizeAccessor<2>(model, accessor);
    break;
  case 3:
    unquantizeAccessor<3>(model, accessor);
    break;
  case 4:
    unquantizeAccessor<4>(model, accessor);
    break;
  }
}
} // namespace

void unquantizeMeshData(Model& model) {
  for (Mesh& mesh : model.meshes) {
    for (MeshPrimitive& primitive : mesh.primitives) {
      for (std::pair<const std::string, int32_t> attribute :
           primitive.attributes) {
        Accessor* pAccessor =
            Model::getSafe(&model.accessors, attribute.second);
        if (pAccessor) {
          unquantizeAccessor(model, *pAccessor);
        }
      }
    }
  }
}
} // namespace CesiumGltfReader
