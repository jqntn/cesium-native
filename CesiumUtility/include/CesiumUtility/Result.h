#pragma once

#include <CesiumUtility/ErrorList.h>
#include <CesiumUtility/IntrusivePointer.h>

#include <optional>

namespace CesiumUtility {

/**
 * @brief Holds the result of an operation. If the operation succeeds, it will
 * provide a value. It may also provide errors and warnings.
 *
 * @tparam T The type of value included in the result.
 */
template <typename T> struct Result {
  Result(T value_) noexcept : value(std::move(value_)), errors() {}

  Result(T value_, ErrorList errors_) noexcept
      : value(std::move(value_)), errors(std::move(errors_)) {}

  Result(ErrorList errors_) noexcept : value(), errors(std::move(errors_)) {}

  /**
   * @brief The value, if the operation succeeded to the point where it can
   * provide one.
   *
   * If a value is not provided because the operation failed, then there should
   * be at least one error in {@link errors} indicating what went wrong.
   */
  std::optional<T> value;

  /**
   * @brief The errors and warnings that occurred during the operation.
   *
   * If a {@link value} is provided, there should not be any errors in this
   * list, but there may be warnings. If a value is not provided, there should
   * be at least one error in this list.
   */
  ErrorList errors;
};

/**
 * @brief Holds the result of an operation. If the operation succeeds, it will
 * provide a value. It may also provide errors and warnings.
 *
 * @tparam T The type of value included in the result.
 */
template <typename T> struct Result<CesiumUtility::IntrusivePointer<T>> {
  Result(CesiumUtility::IntrusivePointer<T> pValue_) noexcept
      : pValue(std::move(pValue_)), errors() {}

  Result(CesiumUtility::IntrusivePointer<T> pValue_, ErrorList errors_) noexcept
      : pValue(std::move(pValue_)), errors(std::move(errors_)) {}

  Result(ErrorList errors_) noexcept
      : pValue(nullptr), errors(std::move(errors_)) {}

  /**
   * @brief The value, if the operation succeeded to the point where it can
   * provide one.
   *
   * If a value is not provided because the operation failed, then there should
   * be at least one error in {@link errors} indicating what went wrong.
   */
  CesiumUtility::IntrusivePointer<T> pValue;

  /**
   * @brief The errors and warnings that occurred during the operation.
   *
   * If a {@link pValue} is provided, there should not be any errors in this
   * list, but there may be warnings. If a pValue is not provided, there should
   * be at least one error in this list.
   */
  ErrorList errors;
};

/**
 * @brief A convenient shortcut for
 * `CesiumUtility::Result<CesiumUtility::IntrusivePointer<T>>`.
 *
 * @tparam T The type of object that the IntrusivePointer points to.
 */
template <typename T> using ResultPointer = Result<IntrusivePointer<T>>;

} // namespace CesiumUtility