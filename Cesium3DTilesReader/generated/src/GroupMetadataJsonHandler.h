// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "MetadataEntityJsonHandler.h"

#include <Cesium3DTiles/GroupMetadata.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
} // namespace CesiumJsonReader

namespace Cesium3DTilesReader {
class GroupMetadataJsonHandler : public MetadataEntityJsonHandler {
public:
  using ValueType = Cesium3DTiles::GroupMetadata;

  explicit GroupMetadataJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void
  reset(IJsonHandler* pParentHandler, Cesium3DTiles::GroupMetadata* pObject);

  IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyGroupMetadata(
      const std::string& objectType,
      const std::string_view& str,
      Cesium3DTiles::GroupMetadata& o);

private:
  Cesium3DTiles::GroupMetadata* _pObject = nullptr;
};
} // namespace Cesium3DTilesReader
