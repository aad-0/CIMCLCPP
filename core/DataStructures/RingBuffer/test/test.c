#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ring_buffer.h"
#include "io.h"


// int main (void)
// {
//   fprintf(stdout, "HI");
// 
//   return 0;
// }

#define reset(X) memset( ((X)), 0x00, sizeof(*X))

#define BUFF_SIZE 32
uint8_t buff [BUFF_SIZE];
RingBufferObject_TypeDef rb;
IoVTable RingBufferIoVTableCustom;

VTABLE_METHOD_DECLR(custom, write, int32_t, uint8_t * pSrcBuffer, uint32_t Size, CIMCLCPP_USERDATA pUserData)
{
  uint32_t i;
    fprintf(stdout, "WRITE\n", pSrcBuffer [i]);
  for (i = 0; i < Size; ++i)
  {
    fprintf(stdout, "%X %d\n", pSrcBuffer [i], i);
  }
    fprintf(stdout, "WRITE_END\n", pSrcBuffer [i]);

  RingBufferIoVTable.write (pSelf, pSrcBuffer, Size, pUserData);

}


int main (void)
{
  // void *memcpy(void *dest, const void *src, size_t n);

  // memcpy (&RingBufferIoVTableCustom, &RingBufferIoVTable, sizeof(RingBufferIoVTable));
  RingBufferIoVTableCustom.init = RingBufferIoVTable.init;
  RingBufferIoVTableCustom.deInit = RingBufferIoVTable.deInit;
  RingBufferIoVTableCustom.write = custom_write;
  RingBufferIoVTableCustom.read = RingBufferIoVTable.read;
  
  RingBufferInit_TypeDef ringBufferInit = {.pBuffer = buff, .Length = BUFF_SIZE};
  RingBufferIoVTable.init(&rb, &(RingBufferInit_TypeDef) {.pBuffer = buff, .Length = BUFF_SIZE});
  rb.pVTable = & RingBufferIoVTableCustom;

  //uint8_t data;

  uint8_t i;
  uint32_t data;
  for (i = 0; i < 8; ++i)
  {
    data = i * 1;
    fprintf(stdout, "INFO Data %d, i: %d\n", data, i);
    // RingBuffer_Write_XBit(&rb, &data, sizeof(data));
    rb.pVTable->write (&rb, (uint8_t *)&data, sizeof(data), NULL);
  }

  uint8_t f;
  for (i = 0; i < 16; ++i)
  {
    // f = RingBuffer_Read_XBit(&rb, &data, sizeof(data));
    f = rb.pVTable->read(&rb, (uint8_t *)&data, sizeof(data), NULL);
    fprintf (stdout, "Data:%d : %d  f:%d\n",i, data, f);
  }
    //RingBuffer_Write_XBit(&rb, &i, sizeof(uint8_t));



  return 0;

}
