/*
 * application.c
 *
 *  Created on: May 12, 2022
 *      Author: Admin
 */

#include "application.h"

#define QUEUE_SIZE	30


uint8_t aBootQueue[QUEUE_SIZE];

uint8_t enter_boot = 0;

event EventApp = _INITIAL_;
pre_event PreEventApp = _INITIAL_;
uint32_t boot_address = START_BOOT_ADDRESS;

FLASH_EraseInitTypeDef EraseStruct = {
		.TypeErase = FLASH_TYPEERASE_SECTORS,
		.Banks = FLASH_BANK_1,
		.Sector = FLASH_SECTOR_4,
		.NbSectors = 8,
		.VoltageRange = FLASH_VOLTAGE_RANGE_3,
};
uint32_t Sector_Fault;
sQueue_Struct_TypeDef BootQueue =
{
		.number = 0,
		.head = 0,
		.tail = 0,
		.status = 0,
		.address = aBootQueue,
		.value = 0,
};

uint8_t sent_boot = 0;
object BootApp = {
	.sQueue = &BootQueue,
	.Dispatch = ApplicationBoot_Hanlde_Event,
};

volatile uint8_t CheckReceive = 0;
void ApplicationBoot_Hanlde_Event (event Event)
{
	switch (Event)
	{
	    case _INITIAL_:
            GPIO_Led_Initial();
            if (enter_boot == 1)
            	ApplicationBoot_Push_Event_to_Queue(BootApp.sQueue, _BOOT_START_);
            else
            {
            	ApplicationBoot_Push_Event_to_Queue(BootApp.sQueue, _CALL_APPLICATION_);
            }
	        break;
	    case _PUSH_BOOT_BUTTON_:
	    	break;
	    case _BOOT_START_:
	    	ApplicationBoot_BootStart_Hanlde();
	    	EventApp = _BOOTING_;
	    	break;
	    case _BOOTING_:

	    	break;
	    case _BOOT_END_:
	    	//EventApp = _BOOT_END_;
//	    	UART_Send_ACK(&ACK_END);
//	    	HAL_NVIC_SystemReset();
	    	break;
	    case _CHECK_LOAD_DATA_RECEIVE_DONE_:
	    	//CheckReceive = 1;
//            if ( (ApplicationBoot_Check_Receive_DataLoad_Done() == 1) && (Receive.ui16_length != 0) )
//            {
//            	ApplicationBoot_Push_Event_to_Queue (BootApp.sQueue, _CHECK_RECEIVE_);
//           //CheckReceive = 0;
//            }

	    	if (Receive.ui16_length != 0)
	    	{
	    		if (ApplicationBoot_Check_Receive_DataLoad_Done() == 1)
	    			ApplicationBoot_Push_Event_to_Queue (BootApp.sQueue, _CHECK_RECEIVE_);
	    	}
	    	break;
	    case _CHECK_RECEIVE_:
	    	ApplicationBoot_UART_CheckReceive();
//	    	UART_Reset_Data(&Receive);
	    	break;
	    case _CALL_APPLICATION_:
	    	ApplicationBoot_Jumpto_ApplicationProgram();
	    	break;
	    case _CHECK_BOOT_PROCESS_:
	    	break;
	    case _BOOT_ERROR_:
	    	break;
	}
}

uint8_t ApplicationBoot_Call_Event (event ApplicationEvent, event NameEvent)
{
	ApplicationEvent = NameEvent;
	return 1;
}

uint8_t ApplicationBoot_Push_Event_to_Queue (sQueue_Struct_TypeDef *AppQueue, event EventName)
{
	if (Q_push_data_to_queue( AppQueue, EventName ) == 0 )
		return 0;
	return 1;
}

uint8_t ApplicationBoot_Get_Event_from_Queue (sQueue_Struct_TypeDef *AppQueue, uint8_t Type)
{
	uint8_t result = 0;
	result = Q_get_data_from_queue( AppQueue, Type );
	if (result == 0)
		return 0;
	return result;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	enter_boot = 1;
}

uint8_t ApplicationBoot_Check_Receive_DataLoad_Done (void)
{
    if (ui16_PrevReceive == 8)
    	return 1;
    else if ((ui16_PrevReceive != Receive.ui16_length) )
    {
    	ui16_PrevReceive = Receive.ui16_length;
    	return 0;
    }
}

uint8_t ApplicationBoot_Initial (void)
{
	ApplicationBoot_Push_Event_to_Queue(BootApp.sQueue, _INITIAL_);
    return 1;
}

uint8_t ApplicationBoot_BootStart_Hanlde(void)
{
	GPIO_Led_BootStart_Notify();
	HAL_FLASH_Unlock();
	ApplicationBoot_EraseFlash_ApplicationProgram();
    UART_Send_ACK(&ACK_START);
    sent_boot = 1;
	return 1;
}

uint8_t ApplicationBoot_UART_CheckReceive(void)
{
	uint64_t LoadValue = 0;
	//Kiem tra cac URC va thuc hien cac chuc nang duoc yeu cau
	switch (EventApp)
	{
	    case _BOOTING_:
	    	if (strstr( (char*)Receive.DataArray, (char*)"end") != NULL)
	    	{
	    		HAL_FLASH_Lock();
	    		//ApplicationBoot_Push_Event_to_Queue(BootApp.sQueue, _BOOT_END_);
	    		UART_Reset_Data(&Receive);
	    		UART_Send_ACK(&ACK_END);
		    	HAL_NVIC_SystemReset();
	    	}
	    	else
	    	{
	    	//qua trinh nap
                LoadValue = Convert_Hex_to_Int2(Receive.DataArray, 8);
                ApplicationBoot_Flash_Program (boot_address, LoadValue);
                boot_address = boot_address + 4;
                HAL_UART_Transmit(&huart4, Receive.DataArray, Receive.ui16_length, 500);
                HAL_UART_Transmit(&huart4, (uint8_t*)"\r\n", 2, 500);
                UART_Reset_Data(&Receive);
                UART_Send_ACK(&ACK_DATA);
	    	}
	    	break;
	    case _BOOT_END_:
	    	break;
	}
	return 1;
}



uint8_t ApplicationBoot_Flash_Program (uint32_t Address, uint64_t Data)
{
//	HAL_FLASH_Unlock();

	//Wait for last operation to be completed
    FLASH_WaitForLastOperation(1000);
	uint8_t a;
	a = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Data);
	HAL_Delay(20);
	if ( a != HAL_OK)
		return 0;
	HAL_Delay(20);
//	HAL_FLASH_Lock();
	return 1;
}

uint8_t ApplicationBoot_Jumpto_ApplicationProgram(void)
{
	/* Turn off Peripheral, Clear Interrupt Flag*/
	HAL_RCC_DeInit();

	/* Clear Pending Interrupt Request, turn  off System Tick*/
	HAL_DeInit();

//	/* Turn off fault harder*/
//	SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk |\
//	SCB_SHCSR_BUSFAULTENA_Msk | \
//	SCB_SHCSR_MEMFAULTENA_Msk ) ;

	   /* Set Main Stack Pointer*/
	//__set_MSP(*(volatile uint32_t*) (0x08010000));
	void (*app_reset_handler)(void) = (void*)(*((volatile uint32_t*) (0x08010000 + 4U)));
	__set_MSP(*(volatile uint32_t*) (0x08010000));
	SCB->VTOR = 0x08010000;
	HAL_GPIO_WritePin(LED_START_GPIO_Port, LED_START_Pin, GPIO_PIN_RESET);
	app_reset_handler();
	return 1;
}

uint8_t ApplicationBoot_EraseFlash_ApplicationProgram(void)
{

    if (HAL_FLASHEx_Erase(&EraseStruct, &Sector_Fault) != HAL_OK)
    {
    	return Sector_Fault;
    }
    else
	return 1;
}

//HAL_StatusTypeDef	OnchipFlashWriteData (uint32_t andress, uint8_t	*data_address, uint32_t data_length)
//{
//	uint8_t		*temp_data_address;
//	uint16_t	i=0;
//	uint64_t	writeval=0,writetime=0;
//	uint32_t	temp_write_address;
//	HAL_StatusTypeDef status = HAL_ERROR;
//
//	temp_data_address = data_address;
//	temp_write_address = andress;
//
//	if ((data_length%8) == 0)
//	{
//		//Calculate number of word to write
//		writetime = data_length/8;
//
//		//Unlock flash
//		HAL_FLASH_Unlock();  //
//		//Wait for last operation to be completed
//        FLASH_WaitForLastOperation(1000);
//		//Change bytes order then write
//		for (i=0;i<writetime;i++)
//		{
//			writeval = *(__IO uint64_t*)(temp_data_address);
//
//			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,temp_write_address,writeval);
//
//			if (status == HAL_ERROR)
//				break;
//
//			temp_write_address = temp_write_address + 8;
//			temp_data_address = temp_data_address + 8;
//		}
//
//		//Lock flash
//		HAL_FLASH_Lock();
//	}
//
//	return status;
//}
