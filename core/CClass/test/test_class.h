/**
 *
 */


#ifndef CCLASS_TEST_CLASS_H
#define CCLASS_TEST_CLASS_H

#include "cclass.h"
#include <stdint.h>


#define STDIN_FD    (0)
#define STDOUT_FD   (1)
#define STDERR_FD   (2)

/** Generic Some Io Class & Vtable **/
VTABLE(SomeIoVTable)
{
  VTABLE_METHOD_PROTO(write, uint32_t, uint8_t const * const pBuffer, uint32_t const BufferSize);
  VTABLE_METHOD_PROTO(read, uint32_t, uint8_t * const pBuffer, uint32_t const BufferSize);

} SomeIoVTable;


CLASS(SomeIoObject)
{
  CLASS_MEMBER(VTABLE_PTR, VTable);
  CLASS_MEMBER(void *, pFile);
} SomeIoObject;

// Extern
extern SomeIoVTable SomeIoVTableGeneric;

#endif // CCLASS_TEST_CLASS_H

