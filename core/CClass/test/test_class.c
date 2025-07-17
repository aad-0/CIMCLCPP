/**
 *
 *
 */

#include "test_class.h"
// #include <stdio.h>
// #include <stdint.h>
#include <unistd.h>

/** Vtable **/
// this vtable is shared accross all objects, it does not need to be in this way
VTABLE_METHOD_DECLR(SomeIoVTable, write, uint32_t, uint8_t const * const pBuffer, uint32_t const BufferSize)
{
  if ( 0 > write (STDOUT_FD, pBuffer, BufferSize) )
  {
    return -1;
  }

  return 0;
}


VTABLE_METHOD_DECLR(SomeIoVTable, read, uint32_t, uint8_t * const pBuffer, uint32_t const BufferSize)
{
  CIMCLCPP_Unused (pBuffer);
  CIMCLCPP_Unused (BufferSize);

  return 0;
}

SomeIoVTable SomeIoVTableGeneric = 
{
  VTABLE_ASSIGN_VTABLE(SomeIoVTable, .write, write),
  VTABLE_ASSIGN_VTABLE(SomeIoVTable, .read, read),
};



