/*
 * @file: ring_buffer.c
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "ring_buffer.h"

IoVTable RingBufferIoVTable =
  {
    RingBuffer_Init,
    RingBuffer_DeInit,
    RingBuffer_Write,
    RingBuffer_Read,
  };

/**
 * @brief This is a init function for ring buffer
 * @param pInit Instance of Ring Buffer
 * @param pBuffer User's Buffer
 * @param Length Length of Buffer
 * @note User should not write to buffer manually
 * @retval Status Code
 */


// void * const pBuffer,
// int32_t RingBuffer_Init(RingBufferObject_TypeDef * const pInit,
//                      uint32_t const Length)
// 
int32_t RingBuffer_Init (RingBufferObject_TypeDef * pInit, RingBufferInit_TypeDef * pInitData)
{
	pInit->Length = pInitData->Length;
	pInit->pBuffer = pInitData->pBuffer;

	pInit->WriteIdx = 0;
	pInit->ReadIdx = 0;

	return 0;
}

/**
 * @brief This function deInitializes a ring buffer
 * @param pInit Instance of ring buffer
 * @retval Status Code
 */
int32_t RingBuffer_DeInit(RingBufferObject_TypeDef * pInit)
{
  (void) (pInit);
	return 0;
}

/**
 * @brief This Function writes to ring buffer
 * @param pInit Instance of Ring Buffer
 * @param pData Pointer of data
 * @param Size Size Data to write
 * @retval Where is the WriteIDX is
 */

// uint32_t RingBuffer_Write (RingBufferObject_TypeDef * const pInit,
//                             void * const pData,
//                            size_t const Size)
int32_t RingBuffer_Write (RingBufferObject_TypeDef * pInit, void * const pData, size_t const Size)
{
	uint32_t i;
	for (i = 0; i < Size; ++i)
		( (uint8_t *)(pInit->pBuffer) )[(pInit->WriteIdx +i) % (pInit->Length)] =
				( (uint8_t *)(pData) )[i];

  pInit->WriteIdx = (pInit->WriteIdx +Size) % (pInit->Length);
	return pInit->WriteIdx;
}

/**
 * @brief This Function reads from ring buffer
 * @param pInit Instance of Ring Buffer
 * @param pData Pointer of where to write
 * @param Size Size where to write and how much to read
 * @retval Where is the ReadIDX is
 */

// uint32_t RingBuffer_Read (RingBufferObject_TypeDef * const pInit,
//                            void * const pData,
//                           size_t const Size)
int32_t RingBuffer_Read (RingBufferObject_TypeDef * pInit, void * const pData, size_t const Size)
{
	uint32_t i;
	for (i = 0; i < Size; ++i)
				( (uint8_t *)(pData) ) [i] =
						( (uint8_t *)(pInit->pBuffer) )[(pInit->ReadIdx +i) % (pInit->Length)];

  pInit->ReadIdx = (pInit->ReadIdx +Size) % (pInit->Length);
	return pInit->ReadIdx;
}

#ifdef __cplusplus
}
#endif
