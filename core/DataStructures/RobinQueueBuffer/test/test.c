#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "robin_queue_buffer.h"
#include "queue_buffer.h"
#include "io.h"


// int main (void)
// {
//   fprintf(stdout, "HI");
// 
//   return 0;
// }

#define reset(X) memset( ((X)), 0x00, sizeof(*X))

#define QUEUE_SIZE (32U)
#define ROBIN_QUEUE_SIZE  (4U)

uint8_t buff0 [QUEUE_SIZE];
uint8_t buff1 [QUEUE_SIZE];
uint8_t buff2 [QUEUE_SIZE];
uint8_t buff3 [QUEUE_SIZE];

QueueBufferObject_TypeDef q1, q2, q3, q4;
QueueBufferObject_TypeDef * pRobinQueueBuffers [] = {&q1, &q2, &q3, &q4};
RobinQueueBufferObject_TypeDef rb;
// IoVTable RingBufferIoVTableCustom;



int main (void)
{
  // initialize queues
  QueueBufferIoVTable.init(&q1, &(QueueBufferInit_TypeDef){.pBuffer = buff0, .Length = QUEUE_SIZE} );
  QueueBufferIoVTable.init(&q2, &(QueueBufferInit_TypeDef){.pBuffer = buff1, .Length = QUEUE_SIZE} );
  QueueBufferIoVTable.init(&q3, &(QueueBufferInit_TypeDef){.pBuffer = buff2, .Length = QUEUE_SIZE} );
  QueueBufferIoVTable.init(&q4, &(QueueBufferInit_TypeDef){.pBuffer = buff3, .Length = QUEUE_SIZE} );

  // init RobinQueue
  RobinQueueBufferIoVTable.init (& rb, & (RobinQueueInit_TypeDef){.pBuffers = pRobinQueueBuffers, .Length = 4} );

  // void *memcpy(void *dest, const void *src, size_t n);

  // memcpy (&RingBufferIoVTableCustom, &RingBufferIoVTable, sizeof(RingBufferIoVTable));

  //uint8_t data;

  uint32_t i;
  int32_t data;
  for (i = 0; i < 8; ++i)
  {
    data = (i+1) * 1;
    fprintf(stdout, "INFO Data %d, i: %d\n", data, i);
    uint32_t priority = i % 4;
    rb.pVTable->write (&rb, (uint8_t *)&data, sizeof(data), &priority);
  }

  uint8_t f;
  for (i = 0; i < 16; ++i)
  {
    // f = RingBuffer_Read_XBit(&rb, &data, sizeof(data));
    f = rb.pVTable->read(&rb, (uint8_t *)&data, sizeof(data), NULL);
    fprintf (stdout, "Data:%d : %d  f:%d\n",i, data, f);
  }



  return 0;

}
