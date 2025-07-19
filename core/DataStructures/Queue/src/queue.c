/**
 * @file: queue.c
 *
 */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "queue.h"

IoVTable RingBufferIoVTable =
  {
    QueueBuffer_Init,
    QueueBuffer_DeInit,
    QueueBuffer_Write,
    QueueBuffer_Read,
  };

/**
 * @brief This is a init function for queue
 * @param pInit Instance of queue
 * @param pBuffer User's Buffer
 * @param Length Length of Buffer
 * @note User should not write to buffer manually
 * @retval Status Code
 */
int32_t QueueBuffer_Init (QueueObject_TypeDef * const pInit, QueueBufferInit_Typedef * pInitData)
{
  pInit->Length = pInitData->Length;
  pInit->pBuffer = pInitData->pBuffer;

  pInit->Height = 0;
  pInit->ReadIdx = 0;
  pInit->WriteIdx = 0;

  return 0;
}

/**
 * @brief This function deInitializes a queue
 * @param pInit Instance of queue
 * @retval Status Code
 */
int32_t QueueBuffer_DeInit (QueueBufferObject_TypeDef * const pInit)
{
  (void)(pInit);
  return 0;
}

/**
 * @brief This Function writes to queue
 * @param pInit Instance of queue
 * @param pData Pointer of data
 * @param Sizeof Sizeof Data to write
 * @retval Where is the WriteIDX is
 */
int32_t QueueBuffer_Write (QueueBufferObject_TypeDef * const pInit, void * const pData, size_t const Size)
{
  if (pInit->WriteIdx < 0)
  {
    return QUEUE_ERR_WRITEIDX_LESSOREQALTOZERO;
  }

  uint32_t i;
  for (i = 0; i < Size; ++i)
  {
    ( (uint8_t *)(pInit->pBuffer) )[(pInit->WriteIdx +i) % (pInit->Length)] = ( (uint8_t *)(pData) )[i];
  }

  pInit->WriteIdx = (pInit->WriteIdx + Size) % pInit->Length;
  pInit->Height += 1;

  return pInit->WriteIdx;
}

/**
 * @brief This Function reads from queue
 * @param pInit Instance of queue
 * @param pData Pointer of where to write
 * @param Sizeof Sizeof where to write and how much to read
 * @retval Where is the ReadIDX is
 */
int32_t QueueBuffer_Read (QueueBufferObject_TypeDef * const pInit, void * const pData, size_t const Size)
{
  if ((pInit->ReadIdx < 0) || (pInit->Height <= 0))
  {
    return QUEUE_ERR_READIDX_LESSOREQALTOZERO;
  }

	uint32_t i;
	for (i = 0; i < Size; ++i)
				( (uint8_t *)(pData) ) [i] =
						( (uint8_t *)(pInit->pBuffer) )[(pInit->ReadIdx +i) % (pInit->Length)];

  pInit->ReadIdx = (pInit->ReadIdx + Size) % (pInit->Length);
  pInit->Height -= 1;
	return pInit->ReadIdx;
}

#ifdef __cplusplus
}
#endif
