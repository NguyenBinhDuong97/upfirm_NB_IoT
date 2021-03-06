/*
 * EventDrive.h
 * This lib was based on queue lib of MR.Vo Hoang Phuong, a SaoViet staff
 *
 *  Created on: May 12, 2022
 *      Author: Admin
 */

#ifndef EVENTDRIVE_H_
#define EVENTDRIVE_H_

#include "main.h"
#include "queue_p.h"


typedef uint8_t event;
typedef uint8_t pre_event;
typedef void(*HandlerEvent) (event EventName);
typedef struct {
	sQueue_Struct_TypeDef *sQueue;
	HandlerEvent  Dispatch;
} object;

#endif /* EVENTDRIVE_H_ */
