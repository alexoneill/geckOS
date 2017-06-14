/**
 * @file util.h
 * @brief Utilities for use in the kernel.
 *
 * @author Alex O'Neill <me@aoneill.me>
 */

#ifndef __INC_UTIL_H__
#define __INC_UTIL_H__

#define WORD 4
#define BITS 8
#define WORD_BITS (WORD * BITS)

/**
 * @brief Create a bit mask with all bits set from the LSB to a position
 *
 * The limit given is not inclusive
 *
 * @param lim The bit position to produce the mask until
 *
 * @return A number with all ones in the lower lim bits
 */
#define BIT_SET_LO(lim) \
  (~ (((uint32_t) (~0)) << (lim)))

/**
 * @brief Create a bit mask with all bits set from the MSB to a position
 *
 * The limit given is not inclusive
 *
 * @param lim The bit position to produce the mask until
 *
 * @return A number with all ones in the upper lim bits
 */
#define BIT_SET_HI(lim) \
  (~ (BIT_SET_LO((WORD_BITS) - lim)))

/**
 * @brief Check if a value has all bits set in a mask
 *
 * @param val The value to check
 * @param mask The mask to check against
 *
 * @return A non-zero value if all bits are set represented in the mask
 */
#define BITS_ALL(val, mask) \
  (((val) & (mask)) == (mask))

/**
 * @brief Check if a value has any of the bits in the mask set
 *
 * @param val The value to check
 * @param mask The mask to check against
 *
 * @return A non-zero value if any bits are set represented in the mask
 */
#define BITS_ANY(val, mask) \
  (!!((val) & (mask)))

/**
 * @brief Get the value of the bit in the given position of a value
 *
 * @param val The value to get the bit for
 * @param bit The position (from the LSB) to get the information for
 *
 * @return A non-zero value if the bit is set
 */
#define GET_NBIT(val, bit) \
  BITS_ALL(val, (1 << bit))

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
#define SET_NBIT(val, bit, on) \
  (val) |= ((!!on) << (bit))

#endif /* __INC_UTIL_H__ */
