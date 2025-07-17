#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ring_buffer.h"


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

int
main (void)
{
  // RingBuffer_Init (&rb, &buff [0], BUFF_SIZE);
  RingBufferInit_TypeDef ringBufferInit = {.pBuffer = buff, .Length = BUFF_SIZE};
  RingBufferIoVTable.init(&rb, &ringBufferInit);
  //uint8_t data;

  uint8_t i;
  uint32_t data;
  for (i = 0; i < 8; ++i)
  {
    data = i * 1;
    fprintf(stdout, "INFO Data %d, i: %d\n", data, i);
    // RingBuffer_Write_XBit(&rb, &data, sizeof(data));
    RingBufferIoVTable.write (&rb, (uint8_t *)&data, sizeof(data), NULL);
  }

  uint8_t f;
  for (i = 0; i < 16; ++i)
  {
    // f = RingBuffer_Read_XBit(&rb, &data, sizeof(data));
    f = RingBufferIoVTable.read(&rb, (uint8_t *)&data, sizeof(data), NULL);
    fprintf (stdout, "Data:%d : %d  f:%d\n",i, data, f);
  }
    //RingBuffer_Write_XBit(&rb, &i, sizeof(uint8_t));



  return 0;

}
