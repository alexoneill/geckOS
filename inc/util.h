/**
 * @file util.h
 * @brief Utilities for use in the kernel.
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __INC_UTIL_H__
#define __INC_UTIL_H__

/**
 * @brief Get the value of the bit in the given position of a value
 *
 * @param val The value to get the bit for
 * @param bit The position (from the LSB) to get the information for
 *
 * @return A non-zero value if the bit is set
 */
#define GET_BIT(val, bit) \
  !!((val) & (1 << bit))

/**
 * @brief Set the value of the bit in the given position of a value
 *
 * This preforms a single read and a single write on the passed value.
 * This may complicate your life if you pass (ptr++) for instance!
 *
 * @param val The value to set the bit in
 * @param bit The position (from the LSB) to modify
 * @param on A non-zero value if the bit should be raised
 */
#define SET_BIT(val, bit, on) \
  (val) = ((val) & ~(1 << bit)) | ((!!on) << bit)

/**
 * @brief Clear value of the bit in the given position of a value
 *
 * @param val The value to clear the bit in
 * @param bit The position (from the LSB) to modify
 */
#define CLEAR_BIT(val, bit) SET_BIT(val, bit, 0)

/**
 * @brief Raise value of the bit in the given position of a value
 *
 * @param val The value to raise the bit in
 * @param bit The position (from the LSB) to modify
 */
#define RAISE_BIT(val, bit) SET_BIT(val, bit, 1)

#endif /* __INC_UTIL_H__ */
