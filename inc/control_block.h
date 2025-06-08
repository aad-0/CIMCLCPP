/**
 * @file control_block.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-09
 * 
 * @copyright Copyright (c) 2025
 */

#ifndef CONTROL_BLOCK_H
#define CONTROL_BLOCK_H

#include "project.h"
#include "cimclcpp.h"

typedef struct control_block_t
{
  // meta info
  char const * meta_str;
  char const * description;

  // control block
  size_t size;
  CIMCLCPP_Pointer belongsTo;

} TypeDef_ControlBlock;



#endif // CONTROL_BLOCK_H