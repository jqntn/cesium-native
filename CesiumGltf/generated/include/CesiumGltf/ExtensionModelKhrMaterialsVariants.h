// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/ExtensionModelKhrMaterialsVariantsValue.h"
#include "CesiumGltf/Library.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <vector>

namespace CesiumGltf {
/**
 * @brief glTF extension that defines a material variations for mesh primitives
 */
struct CESIUMGLTF_API ExtensionModelKhrMaterialsVariants final
    : public CesiumUtility::ExtensibleObject {
  static inline constexpr const char* TypeName =
      "ExtensionModelKhrMaterialsVariants";
  static inline constexpr const char* ExtensionName = "KHR_materials_variants";

  /**
   * @brief variants
   */
  std::vector<CesiumGltf::ExtensionModelKhrMaterialsVariantsValue> variants;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += sizeof(ExtensionModelKhrMaterialsVariants);
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             sizeof(CesiumUtility::ExtensibleObject);

    accum += sizeof(CesiumGltf::ExtensionModelKhrMaterialsVariantsValue) *
             this->variants.capacity();
    for (const CesiumGltf::ExtensionModelKhrMaterialsVariantsValue& value :
         this->variants) {
      accum += value.getSizeBytes() -
               sizeof(CesiumGltf::ExtensionModelKhrMaterialsVariantsValue);
    }
    return accum;
  }
};
} // namespace CesiumGltf
