#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue_buffer.h"

#define reset(X) memset( ((X)), 0x00, sizeof(*X))

#define QUEUE_SIZE  (32U)
uint8_t buff [QUEUE_SIZE];

QueueBufferObject_TypeDef q;

int main (void)
{

  int8_t i;
  int32_t data;
  QueueBufferIoVTable.init(&q, &(QueueBufferInit_TypeDef){.pBuffer = buff, .Length = QUEUE_SIZE} );

  for (i = 0; i < 5; ++i)
  {
    data = i;
    q.pVTable->write (&q, &data, sizeof(data), NULL);
  }

  int32_t j;
  for (i = 0; i < 30; ++i)
  {
    j = q.pVTable->read(&q, &data, sizeof(data), NULL);
    fprintf(stdout, "I: %d J:%d, D:%d\n", i, j, data);
  }

  return 0;
}
