/**
 *
 * @brief This file Just Determines Interface of Any IO Device
 * @note This IO Interface can be shared across any device/class ect which has read and write
 */

#ifndef IO_H
#define IO_H

/** Includes **/
#include "cclass.h"
#include "cimclcpp.h"

CIMCLCPP_BEGIN_BLOCK_C_HEADER


/**
 * @brief this is generic IO Class, User can pass through additional info using pUserData
 */
VTABLE(IoVTable)
{
  VTABLE_METHOD_PROTO (init, int32_t, CIMCLCPP_USERDATA pUserData);
  VTABLE_METHOD_PROTO (deInit, int32_t);
  VTABLE_METHOD_PROTO (write, int32_t, uint8_t * pSrcBuffer, uint32_t Size, CIMCLCPP_USERDATA pUserData);
  VTABLE_METHOD_PROTO (read, int32_t, int8_t * pDestBuffer, uint32_t Size, CIMCLCPP_USERDATA pUserData);
} IoVTable;



CIMCLCPP_END_BLOCK_C_HEADER
#endif // IO_H

