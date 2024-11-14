// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/Library.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace CesiumGltf {
/**
 * @brief ExtensionNodeMaxarMeshVariants Mappings Value
 */
struct CESIUMGLTF_API ExtensionNodeMaxarMeshVariantsMappingsValue final
    : public CesiumUtility::ExtensibleObject {
  static inline constexpr const char* TypeName =
      "ExtensionNodeMaxarMeshVariantsMappingsValue";

  /**
   * @brief An array of variant index values.
   */
  std::vector<int32_t> variants;

  /**
   * @brief The mesh associated with the set of variants.
   */
  int32_t mesh = -1;

  /**
   * @brief The user-defined name of this variant mesh mapping.
   */
  std::optional<std::string> name;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += sizeof(ExtensionNodeMaxarMeshVariantsMappingsValue);
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             sizeof(CesiumUtility::ExtensibleObject);
    accum += sizeof(int32_t) * this->variants.capacity();
    if (this->name) {
      accum += this->name->capacity() * sizeof(char);
    }
    return accum;
  }
};
} // namespace CesiumGltf
