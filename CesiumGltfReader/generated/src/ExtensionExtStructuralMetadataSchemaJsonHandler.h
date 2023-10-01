// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "ExtensionExtStructuralMetadataClassJsonHandler.h"
#include "ExtensionExtStructuralMetadataEnumJsonHandler.h"

#include <CesiumGltf/ExtensionExtStructuralMetadataSchema.h>
#include <CesiumJsonReader/DictionaryJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/StringJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
}

namespace CesiumGltfReader {
class ExtensionExtStructuralMetadataSchemaJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = CesiumGltf::ExtensionExtStructuralMetadataSchema;

  ExtensionExtStructuralMetadataSchemaJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void reset(
      IJsonHandler* pParentHandler,
      CesiumGltf::ExtensionExtStructuralMetadataSchema* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyExtensionExtStructuralMetadataSchema(
      const std::string& objectType,
      const std::string_view& str,
      CesiumGltf::ExtensionExtStructuralMetadataSchema& o);

private:
  CesiumGltf::ExtensionExtStructuralMetadataSchema* _pObject = nullptr;
  CesiumJsonReader::StringJsonHandler _id;
  CesiumJsonReader::StringJsonHandler _name;
  CesiumJsonReader::StringJsonHandler _description;
  CesiumJsonReader::StringJsonHandler _version;
  CesiumJsonReader::DictionaryJsonHandler<
      CesiumGltf::ExtensionExtStructuralMetadataClass,
      ExtensionExtStructuralMetadataClassJsonHandler>
      _classes;
  CesiumJsonReader::DictionaryJsonHandler<
      CesiumGltf::ExtensionExtStructuralMetadataEnum,
      ExtensionExtStructuralMetadataEnumJsonHandler>
      _enums;
};
} // namespace CesiumGltfReader