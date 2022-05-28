/*
 * queue_p.h
 *
 *  Created on: Nov 30, 2021
 *      Author: lenovo
 */

#ifndef QUEUE_P_H_
#define QUEUE_P_H_

#include "stdint.h"

#define QUEUE_SIZE	30

typedef struct
{
	uint8_t status; // 0 - empty, 1 - not empty , 2 - full
	uint8_t head;
	uint8_t tail;
	uint8_t number;	 // the items are waiting in queue
	uint8_t *address; // storage queue value
	uint8_t value;
}sQueue_Struct_TypeDef;





uint8_t Q_push_data_to_queue(sQueue_Struct_TypeDef *sQueue, uint8_t value);
uint8_t Q_get_number_items(sQueue_Struct_TypeDef *sQueue);
uint8_t Q_get_data_from_queue(sQueue_Struct_TypeDef *sQueue, uint8_t Type);
uint8_t Q_clear_queue(sQueue_Struct_TypeDef *sQueue);

#endif /* QUEUE_P_H_ */
