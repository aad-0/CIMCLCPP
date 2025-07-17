/**
 * @file CIMCLCPP.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-09
 * 
 * @copyright Copyright (c) 2025
 */

#ifndef CIMCLCPP_H
#define CIMCLCPP_H


#ifdef __cplusplus
#warn "CPLUSPLUS CODE"
#define CIMCLCPP_BEGIN_BLOCK_C_HEADER \
  extern "C" {

#define CIMCLCPP_END_BLOCK_C_HEADER  \
  }

#else
#define CIMCLCPP_BEGIN_BLOCK_C_HEADER
#define CIMCLCPP_END_BLOCK_C_HEADER

#endif



#define CIMCLCPP_POINTER void *
#define CIMCLCPP_USERDATA CIMCLCPP_POINTER


#define CIMCLCPP_U8 uint8_t
#define CIMCLCPP_I8 int8_t
#define CIMCLCPP_BYTE CIMCLCPP_U8

#define CIMCLCPP_U16 uint16_t
#define CIMCLCPP_I16 int16_t
#define CIMCLCPP_W16 CIMCLCPP_U16

#define CIMCLCPP_U32 uint32_t
#define CIMCLCPP_I32 int32_t
#define CIMCLCPP_W32 CIMCLCPP_U32


#define CIMCLCPP_Weak __attribute__((weak))
#define CIMCLCPP_force_inline __attribute__((always_inline))

#define CIMCLCPP_Unused(__X) (void) __X

// #define CIMCLCPP_CleanUp

#endif // CIMCLCPP_H
