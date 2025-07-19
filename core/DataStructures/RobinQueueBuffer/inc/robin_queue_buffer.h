/**
 * @file: robin_queue.h
 *
 */

#ifndef INC_ROBIN_QUEUE_H_
#define INC_ROBIN_QUEUE_H_
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "queue_buffer.h"
/* defines */
#define ROBINQUEUE_OK (0)
#define ROBINQUEUE_ERR_PRIORITY_NOTVALID (-1)
#define ROBINQUEUE_STATUS_EMPTY	(1)
/* typedefs */
/**
 * @brief This is a robinqueue which has priority queues and consumes data
 * respect to that priority
 * @note lower indexes has higher priority
 */
typedef struct robin_queue_object_s
{
  IoVTable * pVTable;
	QueueBufferObject_TypeDef ** pBuffers; /**< Array of Queues that will be used in robin queue*/
	uint32_t Length; /**< Length of Bufferrs */
	/* Buffers */
} RobinQueueBufferObject_TypeDef;

typedef struct robin_queue_init_s
{
	QueueBufferObject_TypeDef ** pBuffers; /**< Array of Queues that will be used in robin queue*/
	uint32_t Length; /**< Length of Bufferrs */
} RobinQueueInit_TypeDef;


extern IoVTable RobinQueueBufferIoVTable;

int32_t RobinQueue_Init (RobinQueueBufferObject_TypeDef * const pInit, RobinQueueInit_TypeDef * pInitData);
int32_t RobinQueue_DeInit (RobinQueueBufferObject_TypeDef * const pInit);
int32_t RobinQueue_Write (RobinQueueBufferObject_TypeDef * const pInit, void * const pData, size_t const Size, uint32_t const* Priority);
int32_t RobinQueue_Read (RobinQueueBufferObject_TypeDef * const pInit, void * const pData, size_t const Size);

#ifdef __cplusplus
}
#endif
#endif /* INC_ROBIN_QUEUE_H_ */
