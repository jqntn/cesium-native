// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "BoundingVolumeJsonHandler.h"

#include <Cesium3DTiles/Content.h>
#include <CesiumJsonReader/ExtensibleObjectJsonHandler.h>
#include <CesiumJsonReader/StringJsonHandler.h>

namespace CesiumJsonReader {
class ExtensionReaderContext;
}

namespace Cesium3DTiles {
class ContentJsonHandler
    : public CesiumJsonReader::ExtensibleObjectJsonHandler {
public:
  using ValueType = Content;

  ContentJsonHandler(
      const CesiumJsonReader::ExtensionReaderContext& context) noexcept;
  void reset(IJsonHandler* pParentHandler, Content* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyContent(
      const std::string& objectType,
      const std::string_view& str,
      Content& o);

private:
  Content* _pObject = nullptr;
  BoundingVolumeJsonHandler _boundingVolume;
  CesiumJsonReader::StringJsonHandler _uri;
};
} // namespace Cesium3DTiles