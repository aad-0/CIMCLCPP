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
#include "queue.h"
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
typedef struct RobinQueue_s
{
	Queue_TypeDef ** Buffers; /**< Array of Queues that will be used in robin queue*/
	uint32_t Length; /**< Length of Bufferrs */
	/* Buffers */
} RobinQueue_TypeDef;

int32_t RobinQueue_Init (RobinQueue_TypeDef * const pInit,
                         Queue_TypeDef ** const Buffers,
                         uint32_t const Length);

int32_t RobinQueue_DeInit (RobinQueue_TypeDef * const pInit);

int32_t RobinQueue_Write (RobinQueue_TypeDef * const pInit,
                          uint32_t const Priority,
                          void * const pData,
                          size_t const Sizeof);

int32_t RobinQueue_Read (RobinQueue_TypeDef * const pInit,
                         void * const pData,
                         size_t const Sizeof);

#ifdef __cplusplus
}
#endif
#endif /* INC_ROBIN_QUEUE_H_ */
