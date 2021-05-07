// This file was generated by generate-gltf-classes.
// DO NOT EDIT THIS FILE!
#include "PrimitiveEXT_feature_metadataJsonHandler.h"
#include "CesiumGltf/PrimitiveEXT_feature_metadata.h"

#include <cassert>
#include <string>

using namespace CesiumGltf;

PrimitiveEXT_feature_metadataJsonHandler::
    PrimitiveEXT_feature_metadataJsonHandler(
        const ReaderContext& context) noexcept
    : ExtensibleObjectJsonHandler(context),
      _featureIdAttributes(context),
      _featureIdTextures(context),
      _featureTextures() {}

void PrimitiveEXT_feature_metadataJsonHandler::reset(
    CesiumJsonReader::IJsonHandler* pParentHandler,
    PrimitiveEXT_feature_metadata* pObject) {
  ExtensibleObjectJsonHandler::reset(pParentHandler, pObject);
  this->_pObject = pObject;
}

CesiumJsonReader::IJsonHandler*
PrimitiveEXT_feature_metadataJsonHandler::readObjectKey(
    const std::string_view& str) {
  assert(this->_pObject);
  return this->readObjectKeyPrimitiveEXT_feature_metadata(
      PrimitiveEXT_feature_metadata::TypeName,
      str,
      *this->_pObject);
}

void PrimitiveEXT_feature_metadataJsonHandler::reset(
    CesiumJsonReader::IJsonHandler* pParentHandler,
    ExtensibleObject& o,
    const std::string_view& extensionName) {
  std::any& value =
      o.extensions.emplace(extensionName, PrimitiveEXT_feature_metadata())
          .first->second;
  this->reset(
      pParentHandler,
      &std::any_cast<PrimitiveEXT_feature_metadata&>(value));
}

CesiumJsonReader::IJsonHandler* PrimitiveEXT_feature_metadataJsonHandler::
    readObjectKeyPrimitiveEXT_feature_metadata(
        const std::string& objectType,
        const std::string_view& str,
        PrimitiveEXT_feature_metadata& o) {
  using namespace std::string_literals;

  if ("featureIdAttributes"s == str)
    return property(
        "featureIdAttributes",
        this->_featureIdAttributes,
        o.featureIdAttributes);
  if ("featureIdTextures"s == str)
    return property(
        "featureIdTextures",
        this->_featureIdTextures,
        o.featureIdTextures);
  if ("featureTextures"s == str)
    return property(
        "featureTextures",
        this->_featureTextures,
        o.featureTextures);

  return this->readObjectKeyExtensibleObject(objectType, str, *this->_pObject);
}
