/**
 * @file: robin_queue.c
 */

#ifdef __cplusplus
extern "C" {
#endif
/* Includes */
#include <robin_queue.h>

/**
 * @brief Init function for robin queue.
 * @note user should declare a instance of robinQueue and use this function to initialize
 * @param pInit Instance of RobinQueue, RobinQueue_TypeDef Aka RobinQueue_s
 * @param Buffers Array of Queue's that will be used to construct robin queue structure
 * @note user should declare and initialize these buffers
 * @parma Length Length of Buffers
 * @retval Status Code
 */
int32_t RobinQueue_Init (RobinQueue_TypeDef * const pInit,
	                         Queue_TypeDef ** const Buffers,
	                         uint32_t const Length)
{
	pInit->Buffers = Buffers;
	pInit->Length = Length;

	return 0;
}

/**
 * @brief This function deInits the robin queue
 * @note Buffers are responsiblity of user
 * @param pInit Instance of RobinQueue, RobinQueue_TypeDef Aka RobinQueue_s
 * @retval status code
 */
int32_t RobinQueue_DeInit (RobinQueue_TypeDef * const pInit)
{
	(void)pInit;

	uint32_t i ;
	for (i = 0; i < pInit->Length; ++i)
		Queue_DeInit(pInit->Buffers [i]);

	return 0;
}

/**
 * @brief This function writes to a robin queue respect to its priority
 * @param pInit Instance of RobinQueue, RobinQueue_TypeDef Aka RobinQueue_s
 * @param Priority Priority of message
 * @note Priority cannot be bigger then than the length
 * @note smaller priorities has higher priority
 * @param pData Data that will be written to a queue
 * @param Sizeof Sizeof pData
 * @retval Status Code
 */
int32_t RobinQueue_Write (RobinQueue_TypeDef * const pInit,
	                          uint32_t const Priority,
	                          void * const pData,
	                          size_t const Sizeof)
{
	if (pInit->Length <= Priority)
			return ROBINQUEUE_ERR_PRIORITY_NOTVALID;

	return Queue_Write(pInit->Buffers [Priority], pData, Sizeof);
}

/**
 * @brief This function reads from robin queue.
 * @note It returns higher prioritized message in the queues
 * @param pInit Instance of RobinQueue, RobinQueue_TypeDef Aka RobinQueue_s
 * @param pData Data pointer where the message will be written to
 * @param Sizeof Sizeof pData pointer, also message size
 * @note careful with sizes. using different queues for different sized messages is encouraged
 * @retval Status Code
 */
int32_t RobinQueue_Read (RobinQueue_TypeDef * const pInit,
	                         void * const pData,
	                         size_t const Sizeof)
{
	uint32_t i;
	for (i = 0; i < pInit->Length; ++i)
		{
			if (0 < ((pInit->Buffers[i])->Height) )
				{
					return Queue_Read(pInit->Buffers [i], pData, Sizeof);
				}
		}

	return ROBINQUEUE_STATUS_EMPTY;
}

#ifdef __cplusplus
}
#endif
