/**
 * @file: robin_queue.c
 */

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include <robin_queue_buffer.h>


IoVTable RobinQueueBufferIoVTable =
  {
    RobinQueue_Init,
    RobinQueue_DeInit,
    RobinQueue_Write,
    RobinQueue_Read,
  };


/**
 * @brief Init function for robin queue.
 * @note user should declare a instance of robinQueue and use this function to initialize
 * @param pInit Instance of RobinQueue, RobinQueueBufferObject_TypeDef Aka RobinQueue_s
 * @param pBuffers Array of Queue's that will be used to construct robin queue structure
 * @note user should declare and initialize these buffers
 * @parma Length Length of pBuffers
 * @retval Status Code
 */
int32_t RobinQueue_Init (RobinQueueBufferObject_TypeDef * const pInit, RobinQueueInit_TypeDef * pInitData)
{
  pInit->pVTable = & RobinQueueBufferIoVTable;
	pInit->pBuffers = pInitData->pBuffers;
	pInit->Length = pInitData->Length;

	return 0;
}

/**
 * @brief This function deInits the robin queue
 * @note pBuffers are responsiblity of user
 * @param pInit Instance of RobinQueue, RobinQueueBufferObject_TypeDef Aka RobinQueue_s
 * @retval status code
 */
int32_t RobinQueue_DeInit (RobinQueueBufferObject_TypeDef * const pInit)
{
	(void)pInit;

	uint32_t i ;
	for (i = 0; i < pInit->Length; ++i)
  {
    pInit->pBuffers [i]->pVTable->deInit(pInit->pBuffers [i]);
		// Queue_DeInit(pInit->pBuffers [i]);
  }

	return 0;
}

/**
 * @brief This function writes to a robin queue respect to its priority
 * @param pInit Instance of RobinQueue, RobinQueueBufferObject_TypeDef Aka RobinQueue_s
 * @param Priority Priority of message
 * @note Priority cannot be bigger then than the length
 * @note smaller priorities has higher priority
 * @param pData Data that will be written to a queue
 * @param Sizeof Sizeof pData
 * @retval Status Code
 */

int32_t RobinQueue_Write (RobinQueueBufferObject_TypeDef * const pInit, 
                          void * const pData, size_t const Size, uint32_t const* pPriority)
{
	if (pInit->Length <= *pPriority)
			return ROBINQUEUE_ERR_PRIORITY_NOTVALID;

  return pInit->pBuffers [*pPriority]->pVTable->write (pInit->pBuffers [*pPriority], pData, Size, NULL);
	// return Queue_Write(pInit->pBuffers [Priority], pData, Sizeof);
}

/**
 * @brief This function reads from robin queue.
 * @note It returns higher prioritized message in the queues
 * @param pInit Instance of RobinQueue, RobinQueueBufferObject_TypeDef Aka RobinQueue_s
 * @param pData Data pointer where the message will be written to
 * @param Sizeof Sizeof pData pointer, also message size
 * @note careful with sizes. using different queues for different sized messages is encouraged
 * @retval Status Code
 */
int32_t RobinQueue_Read (RobinQueueBufferObject_TypeDef * const pInit,
	                         void * const pData,
	                         size_t const Size)
{
	uint32_t i;
	for (i = 0; i < pInit->Length; ++i)
		{
			if (0 < ((pInit->pBuffers[i])->Height) )
				{
          return pInit->pBuffers [i]->pVTable->read (pInit->pBuffers [i], pData, Size, NULL);
					// vreturn Queue_Read(pInit->pBuffers [i], pData, Size);
				}
		}

	return ROBINQUEUE_STATUS_EMPTY;
}

#ifdef __cplusplus
}
#endif
