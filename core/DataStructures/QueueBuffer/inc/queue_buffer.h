/**
 * @file queue.h
 */
#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include <stddef.h>
#include <stdint.h>
#include "io.h" // generic one which is in core

/* defines */
#define QUEUE_OK  (0)
#define QUEUE_ERR_LENGTH_LESSOREQUALTOZERO    (-1)
#define QUEUE_ERR_READIDX_LESSOREQALTOZERO   (-2)
#define QUEUE_ERR_WRITEIDX_LESSOREQALTOZERO  (-3)

/* typedefs */

/**
 * @brief This struct provides an queue architecture
 * @note user should use functions and not set any thing manually
 * @note Careful with buffer size on stm32
 */
typedef struct queue_buffer_object_s
{
  /* User */
  uint32_t Length; /**< Length of pBuffer */
  void * pBuffer; /**< User's Buffer */

  /* Control */
  uint32_t Height; /**< Height of Queue */
  int32_t ReadIdx; /**< Read index */
  int32_t WriteIdx; /**< Write Index */

} QueueBufferObject_TypeDef;

typedef struct queue_buffer_init_s
{
  void * pBuffer; /**< Buffer that user proviided */
  uint32_t Length; /** Length of Buffer */
} QueueBufferInit_TypeDef;


extern IoVTable QueueBufferIoVTable;
// int32_t Queue_Init (QueueBufferObject_TypeDef * const pInit, void * const pBuffer, uint32_t Length);
int32_t QueueBuffer_Init (QueueBufferObject_TypeDef * const pInit, QueueBufferInit_TypeDef * pInitData);
int32_t QueueBuffer_DeInit (QueueBufferObject_TypeDef * const pInit);
int32_t QueueBuffer_Write (QueueBufferObject_TypeDef * const pInit, void * const pData, size_t const Size);
int32_t QueueBuffer_Read (QueueBufferObject_TypeDef * const pInit, void * const pData, size_t const Size);

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */
