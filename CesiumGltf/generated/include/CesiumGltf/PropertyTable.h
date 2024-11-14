// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "CesiumGltf/Library.h"
#include "CesiumGltf/PropertyTableProperty.h"

#include <CesiumUtility/ExtensibleObject.h>

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>

namespace CesiumGltf {
/**
 * @brief Properties conforming to a class, organized as property values stored
 * in binary columnar arrays.
 */
struct CESIUMGLTF_API PropertyTable final
    : public CesiumUtility::ExtensibleObject {
  static inline constexpr const char* TypeName = "PropertyTable";

  /**
   * @brief The name of the property table, e.g. for display purposes.
   */
  std::optional<std::string> name;

  /**
   * @brief The class that property values conform to. The value must be a class
   * ID declared in the `classes` dictionary.
   */
  std::string classProperty;

  /**
   * @brief The number of elements in each property array.
   */
  int64_t count = int64_t();

  /**
   * @brief A dictionary, where each key corresponds to a property ID in the
   * class' `properties` dictionary and each value is an object describing where
   * property values are stored. Required properties must be included in this
   * dictionary.
   */
  std::unordered_map<std::string, CesiumGltf::PropertyTableProperty> properties;

  /**
   * @brief Calculates the size in bytes of this object, including the contents
   * of all collections, pointers, and strings. This will NOT include the size
   * of any extensions attached to the object. Calling this method may be slow
   * as it requires traversing the object's entire structure.
   */
  int64_t getSizeBytes() const {
    int64_t accum = 0;
    accum += sizeof(PropertyTable);
    accum += CesiumUtility::ExtensibleObject::getSizeBytes() -
             sizeof(CesiumUtility::ExtensibleObject);
    if (this->name) {
      accum += this->name->capacity() * sizeof(char);
    }
    accum += this->classProperty.capacity() * sizeof(char);

    accum += this->properties.bucket_count() *
             (sizeof(std::string) + sizeof(CesiumGltf::PropertyTableProperty));
    for (const auto& [k, v] : this->properties) {
      accum += k.capacity() * sizeof(char) - sizeof(std::string);
      accum += v.getSizeBytes() - sizeof(CesiumGltf::PropertyTableProperty);
    }
    return accum;
  }
};
} // namespace CesiumGltf
