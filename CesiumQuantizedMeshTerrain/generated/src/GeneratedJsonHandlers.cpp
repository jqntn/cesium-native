// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
// NOLINTBEGIN(readability-duplicate-include)
#include "LayerJsonHandler.h"
#include "registerReaderExtensions.h"

#include <CesiumJsonReader/ArrayJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/IJsonHandler.h>
#include <CesiumJsonReader/JsonReader.h>
#include <CesiumQuantizedMeshTerrain/Layer.h>
#include <CesiumQuantizedMeshTerrain/LayerReader.h>
#include <CesiumUtility/Assert.h>

#include <rapidjson/document.h>

#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <vector>
// NOLINTEND(readability-duplicate-include)

namespace CesiumQuantizedMeshTerrain {

LayerJsonHandler::LayerJsonHandler(
    const CesiumJsonReader::JsonReaderOptions& options) noexcept
    : CesiumJsonReader::ExtensibleObjectJsonHandler(options),
      _attribution(),
      _available(options),
      _bounds(),
      _description(),
      _extensionsProperty(),
      _format(),
      _maxzoom(),
      _minzoom(),
      _metadataAvailability(),
      _name(),
      _parentUrl(),
      _projection(),
      _scheme(),
      _tiles(),
      _version() {}

void LayerJsonHandler::reset(
    CesiumJsonReader::IJsonHandler* pParentHandler,
    CesiumQuantizedMeshTerrain::Layer* pObject) {
  CesiumJsonReader::ExtensibleObjectJsonHandler::reset(pParentHandler, pObject);
  this->_pObject = pObject;
}

CesiumJsonReader::IJsonHandler*
LayerJsonHandler::readObjectKey(const std::string_view& str) {
  CESIUM_ASSERT(this->_pObject);
  return this->readObjectKeyLayer(
      CesiumQuantizedMeshTerrain::Layer::TypeName,
      str,
      *this->_pObject);
}

CesiumJsonReader::IJsonHandler* LayerJsonHandler::readObjectKeyLayer(
    const std::string& objectType,
    const std::string_view& str,
    CesiumQuantizedMeshTerrain::Layer& o) {
  using namespace std::string_literals;

  if ("attribution"s == str) {
    return property("attribution", this->_attribution, o.attribution);
  }
  if ("available"s == str) {
    return property("available", this->_available, o.available);
  }
  if ("bounds"s == str) {
    return property("bounds", this->_bounds, o.bounds);
  }
  if ("description"s == str) {
    return property("description", this->_description, o.description);
  }
  if ("extensions"s == str) {
    return property(
        "extensions",
        this->_extensionsProperty,
        o.extensionsProperty);
  }
  if ("format"s == str) {
    return property("format", this->_format, o.format);
  }
  if ("maxzoom"s == str) {
    return property("maxzoom", this->_maxzoom, o.maxzoom);
  }
  if ("minzoom"s == str) {
    return property("minzoom", this->_minzoom, o.minzoom);
  }
  if ("metadataAvailability"s == str) {
    return property(
        "metadataAvailability",
        this->_metadataAvailability,
        o.metadataAvailability);
  }
  if ("name"s == str) {
    return property("name", this->_name, o.name);
  }
  if ("parentUrl"s == str) {
    return property("parentUrl", this->_parentUrl, o.parentUrl);
  }
  if ("projection"s == str) {
    return property("projection", this->_projection, o.projection);
  }
  if ("scheme"s == str) {
    return property("scheme", this->_scheme, o.scheme);
  }
  if ("tiles"s == str) {
    return property("tiles", this->_tiles, o.tiles);
  }
  if ("version"s == str) {
    return property("version", this->_version, o.version);
  }

  return this->readObjectKeyExtensibleObject(objectType, str, *this->_pObject);
}

LayerReader::LayerReader() { registerReaderExtensions(this->_options); }

CesiumJsonReader::JsonReaderOptions& LayerReader::getOptions() {
  return this->_options;
}

const CesiumJsonReader::JsonReaderOptions& LayerReader::getOptions() const {
  return this->_options;
}

CesiumJsonReader::ReadJsonResult<CesiumQuantizedMeshTerrain::Layer>
LayerReader::readFromJson(const std::span<const std::byte>& data) const {
  LayerJsonHandler handler(this->_options);
  return CesiumJsonReader::JsonReader::readJson(data, handler);
}

CesiumJsonReader::ReadJsonResult<CesiumQuantizedMeshTerrain::Layer>
LayerReader::readFromJson(const rapidjson::Value& value) const {
  LayerJsonHandler handler(this->_options);
  return CesiumJsonReader::JsonReader::readJson(value, handler);
}

CesiumJsonReader::ReadJsonResult<std::vector<CesiumQuantizedMeshTerrain::Layer>>
LayerReader::readArrayFromJson(const rapidjson::Value& value) const {
  CesiumJsonReader::
      ArrayJsonHandler<CesiumQuantizedMeshTerrain::Layer, LayerJsonHandler>
          handler(this->_options);
  return CesiumJsonReader::JsonReader::readJson(value, handler);
}

} // namespace CesiumQuantizedMeshTerrain
// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
// NOLINTBEGIN(readability-duplicate-include)
#include "AvailabilityRectangleJsonHandler.h"
#include "registerReaderExtensions.h"

#include <CesiumJsonReader/ArrayJsonHandler.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/IJsonHandler.h>
#include <CesiumJsonReader/JsonReader.h>
#include <CesiumQuantizedMeshTerrain/AvailabilityRectangle.h>
#include <CesiumQuantizedMeshTerrain/AvailabilityRectangleReader.h>
#include <CesiumUtility/Assert.h>

#include <rapidjson/document.h>

#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <vector>
// NOLINTEND(readability-duplicate-include)

namespace CesiumQuantizedMeshTerrain {

AvailabilityRectangleJsonHandler::AvailabilityRectangleJsonHandler(
    const CesiumJsonReader::JsonReaderOptions& options) noexcept
    : CesiumJsonReader::ExtensibleObjectJsonHandler(options),
      _startX(),
      _startY(),
      _endX(),
      _endY() {}

void AvailabilityRectangleJsonHandler::reset(
    CesiumJsonReader::IJsonHandler* pParentHandler,
    CesiumQuantizedMeshTerrain::AvailabilityRectangle* pObject) {
  CesiumJsonReader::ExtensibleObjectJsonHandler::reset(pParentHandler, pObject);
  this->_pObject = pObject;
}

CesiumJsonReader::IJsonHandler*
AvailabilityRectangleJsonHandler::readObjectKey(const std::string_view& str) {
  CESIUM_ASSERT(this->_pObject);
  return this->readObjectKeyAvailabilityRectangle(
      CesiumQuantizedMeshTerrain::AvailabilityRectangle::TypeName,
      str,
      *this->_pObject);
}

CesiumJsonReader::IJsonHandler*
AvailabilityRectangleJsonHandler::readObjectKeyAvailabilityRectangle(
    const std::string& objectType,
    const std::string_view& str,
    CesiumQuantizedMeshTerrain::AvailabilityRectangle& o) {
  using namespace std::string_literals;

  if ("startX"s == str) {
    return property("startX", this->_startX, o.startX);
  }
  if ("startY"s == str) {
    return property("startY", this->_startY, o.startY);
  }
  if ("endX"s == str) {
    return property("endX", this->_endX, o.endX);
  }
  if ("endY"s == str) {
    return property("endY", this->_endY, o.endY);
  }

  return this->readObjectKeyExtensibleObject(objectType, str, *this->_pObject);
}

AvailabilityRectangleReader::AvailabilityRectangleReader() {
  registerReaderExtensions(this->_options);
}

CesiumJsonReader::JsonReaderOptions& AvailabilityRectangleReader::getOptions() {
  return this->_options;
}

const CesiumJsonReader::JsonReaderOptions&
AvailabilityRectangleReader::getOptions() const {
  return this->_options;
}

CesiumJsonReader::ReadJsonResult<
    CesiumQuantizedMeshTerrain::AvailabilityRectangle>
AvailabilityRectangleReader::readFromJson(
    const std::span<const std::byte>& data) const {
  AvailabilityRectangleJsonHandler handler(this->_options);
  return CesiumJsonReader::JsonReader::readJson(data, handler);
}

CesiumJsonReader::ReadJsonResult<
    CesiumQuantizedMeshTerrain::AvailabilityRectangle>
AvailabilityRectangleReader::readFromJson(const rapidjson::Value& value) const {
  AvailabilityRectangleJsonHandler handler(this->_options);
  return CesiumJsonReader::JsonReader::readJson(value, handler);
}

CesiumJsonReader::ReadJsonResult<
    std::vector<CesiumQuantizedMeshTerrain::AvailabilityRectangle>>
AvailabilityRectangleReader::readArrayFromJson(
    const rapidjson::Value& value) const {
  CesiumJsonReader::ArrayJsonHandler<
      CesiumQuantizedMeshTerrain::AvailabilityRectangle,
      AvailabilityRectangleJsonHandler>
      handler(this->_options);
  return CesiumJsonReader::JsonReader::readJson(value, handler);
}

} // namespace CesiumQuantizedMeshTerrain
