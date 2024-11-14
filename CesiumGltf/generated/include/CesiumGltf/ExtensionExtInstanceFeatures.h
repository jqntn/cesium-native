// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/ExtensionExtInstanceFeaturesFeatureId.h"
#include "CesiumGltf/Library.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <vector>

namespace CesiumGltf {
/**
 * @brief An object describing per-instance feature IDs.
 */
struct CESIUMGLTF_API ExtensionExtInstanceFeatures final
    : public CesiumUtility::ExtensibleObject {
  static inline constexpr const char* TypeName = "ExtensionExtInstanceFeatures";
  static inline constexpr const char* ExtensionName = "EXT_instance_features";

  /**
   * @brief An array of feature ID sets.
   */
  std::vector<CesiumGltf::ExtensionExtInstanceFeaturesFeatureId> featureIds;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += sizeof(ExtensionExtInstanceFeatures);
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             sizeof(CesiumUtility::ExtensibleObject);

    accum += sizeof(CesiumGltf::ExtensionExtInstanceFeaturesFeatureId) *
             this->featureIds.capacity();
    for (const CesiumGltf::ExtensionExtInstanceFeaturesFeatureId& value :
         this->featureIds) {
      accum += value.getSizeBytes() -
               sizeof(CesiumGltf::ExtensionExtInstanceFeaturesFeatureId);
    }
    return accum;
  }
};
} // namespace CesiumGltf
