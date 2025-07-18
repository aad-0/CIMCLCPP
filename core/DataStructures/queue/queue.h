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
typedef struct Queue_s
{
  /* User */
  uint32_t Length; /**< Length of pBuffer */
  void * pBuffer; /**< User's Buffer */

  /* Control */
  uint32_t Height; /**< Height of Queue */
  int32_t ReadIdx; /**< Read index */
  int32_t WriteIdx; /**< Write Index */

} Queue_TypeDef;

int32_t Queue_Init (Queue_TypeDef * const pInit, void * const pBuffer, uint32_t Length);
int32_t Queue_DeInit (Queue_TypeDef * const pInit);
int32_t Queue_Write (Queue_TypeDef * const pInit, void * const pData, size_t const Sizeof);
int32_t Queue_Read (Queue_TypeDef * const pInit, void * const pData, size_t const Sizeof);

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */
