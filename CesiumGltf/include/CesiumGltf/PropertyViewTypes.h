#pragma once

#include "CesiumGltf/PropertyAttributePropertyView.h"
#include "CesiumGltf/PropertyTablePropertyView.h"
#include "CesiumGltf/PropertyTexturePropertyView.h"

#include <glm/common.hpp>

#include <variant>

namespace CesiumGltf {

#define View PropertyTablePropertyView

/**
 * @brief An alias for all of the potential types of
 * {@link PropertyTablePropertyView} possible without normalization.
 * Can be useful for applications that want to implement abstract
 * representations of PropertyTablePropertyView, without the mess of templated
 * types.
 */
typedef std::variant<
    View<int8_t, false>,
    View<uint8_t, false>,
    View<int16_t, false>,
    View<uint16_t, false>,
    View<int32_t, false>,
    View<uint32_t, false>,
    View<int64_t, false>,
    View<uint64_t, false>,
    View<float>,
    View<double>,
    View<bool>,
    View<std::string_view>,
    View<glm::vec<2, int8_t>, false>,
    View<glm::vec<2, uint8_t>, false>,
    View<glm::vec<2, int16_t>, false>,
    View<glm::vec<2, uint16_t>, false>,
    View<glm::vec<2, int32_t>, false>,
    View<glm::vec<2, uint32_t>, false>,
    View<glm::vec<2, int64_t>, false>,
    View<glm::vec<2, uint64_t>, false>,
    View<glm::vec<2, float>>,
    View<glm::vec<2, double>>,
    View<glm::vec<3, int8_t>, false>,
    View<glm::vec<3, uint8_t>, false>,
    View<glm::vec<3, int16_t>, false>,
    View<glm::vec<3, uint16_t>, false>,
    View<glm::vec<3, int32_t>, false>,
    View<glm::vec<3, uint32_t>, false>,
    View<glm::vec<3, int64_t>, false>,
    View<glm::vec<3, uint64_t>, false>,
    View<glm::vec<3, float>>,
    View<glm::vec<3, double>>,
    View<glm::vec<4, int8_t>, false>,
    View<glm::vec<4, uint8_t>, false>,
    View<glm::vec<4, int16_t>, false>,
    View<glm::vec<4, uint16_t>, false>,
    View<glm::vec<4, int32_t>, false>,
    View<glm::vec<4, uint32_t>, false>,
    View<glm::vec<4, int64_t>, false>,
    View<glm::vec<4, uint64_t>, false>,
    View<glm::vec<4, float>>,
    View<glm::vec<4, double>>,
    View<glm::mat<2, 2, int8_t>, false>,
    View<glm::mat<2, 2, uint8_t>, false>,
    View<glm::mat<2, 2, int16_t>, false>,
    View<glm::mat<2, 2, uint16_t>, false>,
    View<glm::mat<2, 2, int32_t>, false>,
    View<glm::mat<2, 2, uint32_t>, false>,
    View<glm::mat<2, 2, int64_t>, false>,
    View<glm::mat<2, 2, uint64_t>, false>,
    View<glm::mat<2, 2, float>>,
    View<glm::mat<2, 2, double>>,
    View<glm::mat<3, 3, int8_t>, false>,
    View<glm::mat<3, 3, uint8_t>, false>,
    View<glm::mat<3, 3, int16_t>, false>,
    View<glm::mat<3, 3, uint16_t>, false>,
    View<glm::mat<3, 3, int32_t>, false>,
    View<glm::mat<3, 3, uint32_t>, false>,
    View<glm::mat<3, 3, int64_t>, false>,
    View<glm::mat<3, 3, uint64_t>, false>,
    View<glm::mat<3, 3, float>>,
    View<glm::mat<3, 3, double>>,
    View<glm::mat<4, 4, int8_t>, false>,
    View<glm::mat<4, 4, uint8_t>, false>,
    View<glm::mat<4, 4, int16_t>, false>,
    View<glm::mat<4, 4, uint16_t>, false>,
    View<glm::mat<4, 4, int32_t>, false>,
    View<glm::mat<4, 4, uint32_t>, false>,
    View<glm::mat<4, 4, int64_t>, false>,
    View<glm::mat<4, 4, uint64_t>, false>,
    View<glm::mat<4, 4, float>>,
    View<glm::mat<4, 4, double>>,
    View<PropertyArrayView<int8_t>, false>,
    View<PropertyArrayView<uint8_t>, false>,
    View<PropertyArrayView<int16_t>, false>,
    View<PropertyArrayView<uint16_t>, false>,
    View<PropertyArrayView<int32_t>, false>,
    View<PropertyArrayView<uint32_t>, false>,
    View<PropertyArrayView<int64_t>, false>,
    View<PropertyArrayView<uint64_t>, false>,
    View<PropertyArrayView<float>>,
    View<PropertyArrayView<double>>,
    View<PropertyArrayView<bool>>,
    View<PropertyArrayView<std::string_view>>,
    View<PropertyArrayView<glm::vec<2, int8_t>>, false>,
    View<PropertyArrayView<glm::vec<2, uint8_t>>, false>,
    View<PropertyArrayView<glm::vec<2, int16_t>>, false>,
    View<PropertyArrayView<glm::vec<2, uint16_t>>, false>,
    View<PropertyArrayView<glm::vec<2, int32_t>>, false>,
    View<PropertyArrayView<glm::vec<2, uint32_t>>, false>,
    View<PropertyArrayView<glm::vec<2, int64_t>>, false>,
    View<PropertyArrayView<glm::vec<2, uint64_t>>, false>,
    View<PropertyArrayView<glm::vec<2, float>>>,
    View<PropertyArrayView<glm::vec<2, double>>>,
    View<PropertyArrayView<glm::vec<3, int8_t>>, false>,
    View<PropertyArrayView<glm::vec<3, uint8_t>>, false>,
    View<PropertyArrayView<glm::vec<3, int16_t>>, false>,
    View<PropertyArrayView<glm::vec<3, uint16_t>>, false>,
    View<PropertyArrayView<glm::vec<3, int32_t>>, false>,
    View<PropertyArrayView<glm::vec<3, uint32_t>>, false>,
    View<PropertyArrayView<glm::vec<3, int64_t>>, false>,
    View<PropertyArrayView<glm::vec<3, uint64_t>>, false>,
    View<PropertyArrayView<glm::vec<3, float>>>,
    View<PropertyArrayView<glm::vec<3, double>>>,
    View<PropertyArrayView<glm::vec<4, int8_t>>, false>,
    View<PropertyArrayView<glm::vec<4, uint8_t>>, false>,
    View<PropertyArrayView<glm::vec<4, int16_t>>, false>,
    View<PropertyArrayView<glm::vec<4, uint16_t>>, false>,
    View<PropertyArrayView<glm::vec<4, int32_t>>, false>,
    View<PropertyArrayView<glm::vec<4, uint32_t>>, false>,
    View<PropertyArrayView<glm::vec<4, int64_t>>, false>,
    View<PropertyArrayView<glm::vec<4, uint64_t>>, false>,
    View<PropertyArrayView<glm::vec<4, float>>>,
    View<PropertyArrayView<glm::vec<4, double>>>,
    View<PropertyArrayView<glm::mat<2, 2, int8_t>>, false>,
    View<PropertyArrayView<glm::mat<2, 2, uint8_t>>, false>,
    View<PropertyArrayView<glm::mat<2, 2, int16_t>>, false>,
    View<PropertyArrayView<glm::mat<2, 2, uint16_t>>, false>,
    View<PropertyArrayView<glm::mat<2, 2, int32_t>>, false>,
    View<PropertyArrayView<glm::mat<2, 2, uint32_t>>, false>,
    View<PropertyArrayView<glm::mat<2, 2, int64_t>>, false>,
    View<PropertyArrayView<glm::mat<2, 2, uint64_t>>, false>,
    View<PropertyArrayView<glm::mat<2, 2, float>>>,
    View<PropertyArrayView<glm::mat<2, 2, double>>>,
    View<PropertyArrayView<glm::mat<3, 3, int8_t>>, false>,
    View<PropertyArrayView<glm::mat<3, 3, uint8_t>>, false>,
    View<PropertyArrayView<glm::mat<3, 3, int16_t>>, false>,
    View<PropertyArrayView<glm::mat<3, 3, uint16_t>>, false>,
    View<PropertyArrayView<glm::mat<3, 3, int32_t>>, false>,
    View<PropertyArrayView<glm::mat<3, 3, uint32_t>>, false>,
    View<PropertyArrayView<glm::mat<3, 3, int64_t>>, false>,
    View<PropertyArrayView<glm::mat<3, 3, uint64_t>>, false>,
    View<PropertyArrayView<glm::mat<3, 3, float>>>,
    View<PropertyArrayView<glm::mat<3, 3, double>>>,
    View<PropertyArrayView<glm::mat<4, 4, int8_t>>, false>,
    View<PropertyArrayView<glm::mat<4, 4, uint8_t>>, false>,
    View<PropertyArrayView<glm::mat<4, 4, int16_t>>, false>,
    View<PropertyArrayView<glm::mat<4, 4, uint16_t>>, false>,
    View<PropertyArrayView<glm::mat<4, 4, int32_t>>, false>,
    View<PropertyArrayView<glm::mat<4, 4, uint32_t>>, false>,
    View<PropertyArrayView<glm::mat<4, 4, int64_t>>, false>,
    View<PropertyArrayView<glm::mat<4, 4, uint64_t>>, false>,
    View<PropertyArrayView<glm::mat<4, 4, float>>>,
    View<PropertyArrayView<glm::mat<4, 4, double>>>>
    PropertyTablePropertyViewType;

/**
 * @brief An alias for all of the potential types of
 * {@link PropertyTablePropertyView} possible with normalization.
 * Can be useful for applications that want to implement abstract
 * representations of PropertyTablePropertyView, without the mess of templated
 * types.
 */
typedef std::variant<
    View<int8_t, true>,
    View<uint8_t, true>,
    View<int16_t, true>,
    View<uint16_t, true>,
    View<int32_t, true>,
    View<uint32_t, true>,
    View<int64_t, true>,
    View<uint64_t, true>,
    View<glm::vec<2, int8_t>, true>,
    View<glm::vec<2, uint8_t>, true>,
    View<glm::vec<2, int16_t>, true>,
    View<glm::vec<2, uint16_t>, true>,
    View<glm::vec<2, int32_t>, true>,
    View<glm::vec<2, uint32_t>, true>,
    View<glm::vec<2, int64_t>, true>,
    View<glm::vec<2, uint64_t>, true>,
    View<glm::vec<3, int8_t>, true>,
    View<glm::vec<3, uint8_t>, true>,
    View<glm::vec<3, int16_t>, true>,
    View<glm::vec<3, uint16_t>, true>,
    View<glm::vec<3, int32_t>, true>,
    View<glm::vec<3, uint32_t>, true>,
    View<glm::vec<3, int64_t>, true>,
    View<glm::vec<3, uint64_t>, true>,
    View<glm::vec<4, int8_t>, true>,
    View<glm::vec<4, uint8_t>, true>,
    View<glm::vec<4, int16_t>, true>,
    View<glm::vec<4, uint16_t>, true>,
    View<glm::vec<4, int32_t>, true>,
    View<glm::vec<4, uint32_t>, true>,
    View<glm::vec<4, int64_t>, true>,
    View<glm::vec<4, uint64_t>, true>,
    View<glm::mat<2, 2, int8_t>, true>,
    View<glm::mat<2, 2, uint8_t>, true>,
    View<glm::mat<2, 2, int16_t>, true>,
    View<glm::mat<2, 2, uint16_t>, true>,
    View<glm::mat<2, 2, int32_t>, true>,
    View<glm::mat<2, 2, uint32_t>, true>,
    View<glm::mat<2, 2, int64_t>, true>,
    View<glm::mat<2, 2, uint64_t>, true>,
    View<glm::mat<3, 3, int8_t>, true>,
    View<glm::mat<3, 3, uint8_t>, true>,
    View<glm::mat<3, 3, int16_t>, true>,
    View<glm::mat<3, 3, uint16_t>, true>,
    View<glm::mat<3, 3, int32_t>, true>,
    View<glm::mat<3, 3, uint32_t>, true>,
    View<glm::mat<3, 3, int64_t>, true>,
    View<glm::mat<3, 3, uint64_t>, true>,
    View<glm::mat<4, 4, int8_t>, true>,
    View<glm::mat<4, 4, uint8_t>, true>,
    View<glm::mat<4, 4, int16_t>, true>,
    View<glm::mat<4, 4, uint16_t>, true>,
    View<glm::mat<4, 4, int32_t>, true>,
    View<glm::mat<4, 4, uint32_t>, true>,
    View<glm::mat<4, 4, int64_t>, true>,
    View<glm::mat<4, 4, uint64_t>, true>,
    View<PropertyArrayView<int8_t>, true>,
    View<PropertyArrayView<uint8_t>, true>,
    View<PropertyArrayView<int16_t>, true>,
    View<PropertyArrayView<uint16_t>, true>,
    View<PropertyArrayView<int32_t>, true>,
    View<PropertyArrayView<uint32_t>, true>,
    View<PropertyArrayView<int64_t>, true>,
    View<PropertyArrayView<uint64_t>, true>,
    View<PropertyArrayView<glm::vec<2, int8_t>>, true>,
    View<PropertyArrayView<glm::vec<2, uint8_t>>, true>,
    View<PropertyArrayView<glm::vec<2, int16_t>>, true>,
    View<PropertyArrayView<glm::vec<2, uint16_t>>, true>,
    View<PropertyArrayView<glm::vec<2, int32_t>>, true>,
    View<PropertyArrayView<glm::vec<2, uint32_t>>, true>,
    View<PropertyArrayView<glm::vec<2, int64_t>>, true>,
    View<PropertyArrayView<glm::vec<2, uint64_t>>, true>,
    View<PropertyArrayView<glm::vec<3, int8_t>>, true>,
    View<PropertyArrayView<glm::vec<3, uint8_t>>, true>,
    View<PropertyArrayView<glm::vec<3, int16_t>>, true>,
    View<PropertyArrayView<glm::vec<3, uint16_t>>, true>,
    View<PropertyArrayView<glm::vec<3, int32_t>>, true>,
    View<PropertyArrayView<glm::vec<3, uint32_t>>, true>,
    View<PropertyArrayView<glm::vec<3, int64_t>>, true>,
    View<PropertyArrayView<glm::vec<3, uint64_t>>, true>,
    View<PropertyArrayView<glm::vec<4, int8_t>>, true>,
    View<PropertyArrayView<glm::vec<4, uint8_t>>, true>,
    View<PropertyArrayView<glm::vec<4, int16_t>>, true>,
    View<PropertyArrayView<glm::vec<4, uint16_t>>, true>,
    View<PropertyArrayView<glm::vec<4, int32_t>>, true>,
    View<PropertyArrayView<glm::vec<4, uint32_t>>, true>,
    View<PropertyArrayView<glm::vec<4, int64_t>>, true>,
    View<PropertyArrayView<glm::vec<4, uint64_t>>, true>,
    View<PropertyArrayView<glm::mat<2, 2, int8_t>>, true>,
    View<PropertyArrayView<glm::mat<2, 2, uint8_t>>, true>,
    View<PropertyArrayView<glm::mat<2, 2, int16_t>>, true>,
    View<PropertyArrayView<glm::mat<2, 2, uint16_t>>, true>,
    View<PropertyArrayView<glm::mat<2, 2, int32_t>>, true>,
    View<PropertyArrayView<glm::mat<2, 2, uint32_t>>, true>,
    View<PropertyArrayView<glm::mat<2, 2, int64_t>>, true>,
    View<PropertyArrayView<glm::mat<2, 2, uint64_t>>, true>,
    View<PropertyArrayView<glm::mat<3, 3, int8_t>>, true>,
    View<PropertyArrayView<glm::mat<3, 3, uint8_t>>, true>,
    View<PropertyArrayView<glm::mat<3, 3, int16_t>>, true>,
    View<PropertyArrayView<glm::mat<3, 3, uint16_t>>, true>,
    View<PropertyArrayView<glm::mat<3, 3, int32_t>>, true>,
    View<PropertyArrayView<glm::mat<3, 3, uint32_t>>, true>,
    View<PropertyArrayView<glm::mat<3, 3, int64_t>>, true>,
    View<PropertyArrayView<glm::mat<3, 3, uint64_t>>, true>,
    View<PropertyArrayView<glm::mat<4, 4, int8_t>>, true>,
    View<PropertyArrayView<glm::mat<4, 4, uint8_t>>, true>,
    View<PropertyArrayView<glm::mat<4, 4, int16_t>>, true>,
    View<PropertyArrayView<glm::mat<4, 4, uint16_t>>, true>,
    View<PropertyArrayView<glm::mat<4, 4, int32_t>>, true>,
    View<PropertyArrayView<glm::mat<4, 4, uint32_t>>, true>,
    View<PropertyArrayView<glm::mat<4, 4, int64_t>>, true>,
    View<PropertyArrayView<glm::mat<4, 4, uint64_t>>, true>>
    NormalizedPropertyTablePropertyViewType;

#undef View

#define View PropertyTexturePropertyView
/**
 *
 * @brief An alias for all of the potential types of
 * {@link PropertyTexturePropertyView} possible without normalization.
 * Can be useful for applications that want to implement abstract
 * representations of PropertyTexturePropertyView, without the mess of templated
 * types.
 */
typedef std::variant<
    View<int8_t, false>,
    View<uint8_t, false>,
    View<int16_t, false>,
    View<uint16_t, false>,
    View<int32_t, false>,
    View<uint32_t, false>,
    View<float>,
    View<glm::vec<2, int8_t>, false>,
    View<glm::vec<2, uint8_t>, false>,
    View<glm::vec<2, int16_t>, false>,
    View<glm::vec<2, uint16_t>, false>,
    View<glm::vec<3, int8_t>, false>,
    View<glm::vec<3, uint8_t>, false>,
    View<glm::vec<4, int8_t>, false>,
    View<glm::vec<4, uint8_t>, false>,
    View<PropertyArrayView<int8_t>, false>,
    View<PropertyArrayView<uint8_t>, false>,
    View<PropertyArrayView<int16_t>, false>,
    View<PropertyArrayView<uint16_t>, false>>
    PropertyTexturePropertyViewType;

/**
 * @brief An alias for all of the potential types of
 * {@link PropertyTexturePropertyView} possible with normalization.
 * Can be useful for applications that want to implement abstract
 * representations of PropertyTexturePropertyView, without the mess of templated
 * types.
 */
typedef std::variant<
    View<int8_t, true>,
    View<uint8_t, true>,
    View<int16_t, true>,
    View<uint16_t, true>,
    View<int32_t, true>,
    View<uint32_t, true>,
    View<glm::vec<2, int8_t>, true>,
    View<glm::vec<2, uint8_t>, true>,
    View<glm::vec<2, int16_t>, true>,
    View<glm::vec<2, uint16_t>, true>,
    View<glm::vec<3, int8_t>, true>,
    View<glm::vec<3, uint8_t>, true>,
    View<glm::vec<4, int8_t>, true>,
    View<glm::vec<4, uint8_t>, true>,
    View<PropertyArrayView<int8_t>, true>,
    View<PropertyArrayView<uint8_t>, true>,
    View<PropertyArrayView<int16_t>, true>,
    View<PropertyArrayView<uint16_t>, true>>
    NormalizedPropertyTexturePropertyViewType;

#undef View

#define View PropertyAttributePropertyView
/**
 *
 * @brief An alias for all of the potential types of
 * {@link PropertyAttributePropertyView} possible without normalization.
 * Can be useful for applications that want to implement abstract
 * representations of PropertyAttributePropertyView, without the mess of
 * templated types.
 */
typedef std::variant<
    View<int8_t, false>,
    View<uint8_t, false>,
    View<int16_t, false>,
    View<uint16_t, false>,
    View<uint32_t, false>,
    View<float>,
    View<glm::vec<2, int8_t>, false>,
    View<glm::vec<2, uint8_t>, false>,
    View<glm::vec<2, int16_t>, false>,
    View<glm::vec<2, uint16_t>, false>,
    View<glm::vec<2, uint32_t>, false>,
    View<glm::vec<2, float>>,
    View<glm::vec<3, int8_t>, false>,
    View<glm::vec<3, uint8_t>, false>,
    View<glm::vec<3, int16_t>, false>,
    View<glm::vec<3, uint16_t>, false>,
    View<glm::vec<3, uint32_t>, false>,
    View<glm::vec<3, float>>,
    View<glm::vec<4, int8_t>, false>,
    View<glm::vec<4, uint8_t>, false>,
    View<glm::vec<4, int16_t>, false>,
    View<glm::vec<4, uint16_t>, false>,
    View<glm::vec<4, uint32_t>, false>,
    View<glm::vec<4, float>>,
    View<glm::mat<2, 2, int8_t>, false>,
    View<glm::mat<2, 2, uint8_t>, false>,
    View<glm::mat<2, 2, int16_t>, false>,
    View<glm::mat<2, 2, uint16_t>, false>,
    View<glm::mat<2, 2, uint32_t>, false>,
    View<glm::mat<2, 2, float>>,
    View<glm::mat<3, 3, int8_t>, false>,
    View<glm::mat<3, 3, uint8_t>, false>,
    View<glm::mat<3, 3, int16_t>, false>,
    View<glm::mat<3, 3, uint16_t>, false>,
    View<glm::mat<3, 3, uint32_t>, false>,
    View<glm::mat<3, 3, float>>,
    View<glm::mat<4, 4, int8_t>, false>,
    View<glm::mat<4, 4, uint8_t>, false>,
    View<glm::mat<4, 4, int16_t>, false>,
    View<glm::mat<4, 4, uint16_t>, false>,
    View<glm::mat<4, 4, uint32_t>, false>,
    View<glm::mat<4, 4, float>>>
    PropertyAttributePropertyViewType;

/**
 * @brief An alias for all of the potential types of
 * {@link PropertyAttributePropertyView} possible with normalization.
 * Can be useful for applications that want to implement abstract
 * representations of PropertyAttributePropertyView, without the mess of
 * templated types.
 */
typedef std::variant<
    View<int8_t, true>,
    View<uint8_t, true>,
    View<int16_t, true>,
    View<uint16_t, true>,
    View<uint32_t, true>,
    View<glm::vec<2, int8_t>, true>,
    View<glm::vec<2, uint8_t>, true>,
    View<glm::vec<2, int16_t>, true>,
    View<glm::vec<2, uint16_t>, true>,
    View<glm::vec<2, uint32_t>, true>,
    View<glm::vec<3, int8_t>, true>,
    View<glm::vec<3, uint8_t>, true>,
    View<glm::vec<3, int16_t>, true>,
    View<glm::vec<3, uint16_t>, true>,
    View<glm::vec<3, uint32_t>, true>,
    View<glm::vec<4, int8_t>, true>,
    View<glm::vec<4, uint8_t>, true>,
    View<glm::vec<4, int16_t>, true>,
    View<glm::vec<4, uint16_t>, true>,
    View<glm::vec<4, uint32_t>, true>,
    View<glm::mat<2, 2, int8_t>, true>,
    View<glm::mat<2, 2, uint8_t>, true>,
    View<glm::mat<2, 2, int16_t>, true>,
    View<glm::mat<2, 2, uint16_t>, true>,
    View<glm::mat<2, 2, uint32_t>, true>,
    View<glm::mat<3, 3, int8_t>, true>,
    View<glm::mat<3, 3, uint8_t>, true>,
    View<glm::mat<3, 3, int16_t>, true>,
    View<glm::mat<3, 3, uint16_t>, true>,
    View<glm::mat<3, 3, uint32_t>, true>,
    View<glm::mat<4, 4, int8_t>, true>,
    View<glm::mat<4, 4, uint8_t>, true>,
    View<glm::mat<4, 4, int16_t>, true>,
    View<glm::mat<4, 4, uint16_t>, true>,
    View<glm::mat<4, 4, uint32_t>, true>>
    NormalizedPropertyAttributePropertyViewType;

#undef View
} // namespace CesiumGltf