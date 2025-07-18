#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define reset(X) memset( ((X)), 0x00, sizeof(*X))

#define QUEUE_SIZE  (32U)
uint8_t buff [QUEUE_SIZE];

Queue_TypeDef q;

int
main (void)
{

  int8_t i;
  int32_t data;
  Queue_Init(&q, & buff [0], QUEUE_SIZE);

  for (i = 0; i < 5; ++i)
  {
    data = i;
    Queue_Write (&q, &data, sizeof(data));
  }

  int32_t j;
  for (i = 0; i < 30; ++i)
  {
    j = Queue_Read(&q, &data, sizeof(data));
    fprintf(stdout, "I: %d J:%d, D:%d\n", i, j, data);
  }

  return 0;
}
