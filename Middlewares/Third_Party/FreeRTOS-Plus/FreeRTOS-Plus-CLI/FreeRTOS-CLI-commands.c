/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


 /******************************************************************************
 *
 * http://www.FreeRTOS.org/cli
 *
 ******************************************************************************/


/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

#ifndef  configINCLUDE_TRACE_RELATED_CLI_COMMANDS
	#define configINCLUDE_TRACE_RELATED_CLI_COMMANDS 0
#endif

#ifndef configINCLUDE_QUERY_HEAP_COMMAND
	#define configINCLUDE_QUERY_HEAP_COMMAND 0
#endif

#define MAX_MSG_SIZE		256

#include "main.h"
#define _DIVIDER_LINE_	("---------------------------------------------\r\n")

extern const unsigned char completeVersion[];
/*
 * The function that registers the commands that are defined within this file.
 */
void vRegisterSampleCLICommands( void );

/*
 * Implements the task-stats command.
 */
//static BaseType_t prvTaskStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/*
 * Implements the run-time-stats command.
 */
#if( configGENERATE_RUN_TIME_STATS == 1 )
	static BaseType_t prvRunTimeStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
#endif /* configGENERATE_RUN_TIME_STATS */

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	static BaseType_t prvQueryHeapCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
#endif

/*
 * Implements the "trace start" and "trace stop" commands;
 */
#if( configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1 )
	static BaseType_t prvStartStopTraceCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
#endif

#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )

static BaseType_t prvTaskStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
/* Structure that defines the "task-stats" command line command.  This generates
a table that gives information on each task in the system. */
static const CLI_Command_Definition_t xTaskStats =
{
	"task-stats", /* The command string to type. */
	"\r\ntask-stats:\r\n Displays a table showing the state of each FreeRTOS task\r\n",
	prvTaskStatsCommand, /* The function to run. */
	0 /* No parameters are expected. */
};
#endif

#if( configGENERATE_RUN_TIME_STATS == 1 )
	/* Structure that defines the "run-time-stats" command line command.   This
	generates a table that shows how much run time each task has */
	static const CLI_Command_Definition_t xRunTimeStats =
	{
		"run-time-stats", /* The command string to type. */
		"\r\nrun-time-stats:\r\n Displays a table showing how much processing time each FreeRTOS task has used\r\n",
		prvRunTimeStatsCommand, /* The function to run. */
		0 /* No parameters are expected. */
	};
#endif /* configGENERATE_RUN_TIME_STATS */

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	/* Structure that defines the "query_heap" command line command. */
	static const CLI_Command_Definition_t xQueryHeap =
	{
		"query-heap",
		"\r\nquery-heap:\r\n Displays the free heap space, and minimum ever free heap space.\r\n",
		prvQueryHeapCommand, /* The function to run. */
		0 /* The user can enter any number of commands. */
	};
#endif /* configQUERY_HEAP_COMMAND */

#if configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1
	/* Structure that defines the "trace" command line command.  This takes a single
	parameter, which can be either "start" or "stop". */
	static const CLI_Command_Definition_t xStartStopTrace =
	{
		"trace",
		"\r\ntrace [start | stop]:\r\n Starts or stops a trace recording for viewing in FreeRTOS+Trace\r\n",
		prvStartStopTraceCommand, /* The function to run. */
		1 /* One parameter is expected.  Valid values are "start" and "stop". */
	};
#endif /* configINCLUDE_TRACE_RELATED_CLI_COMMANDS */

/*-----------------------------------------------------------*/
#if 1
static BaseType_t prvDeviceResetCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	( void ) pcWriteBuffer;

	HAL_NVIC_SystemReset();

	return pdFALSE;
}

static const CLI_Command_Definition_t xDeviceReset =
{
	"device-reset",
	"\r\ndevice-reset:\r\n Reset device\r\n",
	prvDeviceResetCommand,
	0 /* No parameters are expected. */
};
#endif
/*-----------------------------------------------------------*/
static BaseType_t prvDeviceInfoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	( void ) pcCommandString;
	( void ) xWriteBufferLen;

	configASSERT( pcWriteBuffer );

	char* tmp = pvPortMalloc(MAX_MSG_SIZE);

	memset(pcWriteBuffer,0,xWriteBufferLen);

	memset(tmp,          0, MAX_MSG_SIZE);

	sprintf(pcWriteBuffer,_DIVIDER_LINE_);

	sprintf(tmp,"DEVICE ID   : %04lX\r\n",HAL_GetDEVID());
	strncat(pcWriteBuffer,tmp,strlen(tmp));

	sprintf(tmp,"DEVICE REV  : %04lX\r\n",HAL_GetREVID());
	strncat(pcWriteBuffer,tmp,strlen(tmp));

	sprintf(tmp,"DEVICE UUID : %04lX-%04lX-%04lX\r\n",HAL_GetUIDw0(),HAL_GetUIDw1(),HAL_GetUIDw2());
	strncat(pcWriteBuffer,tmp,strlen(tmp));

	strncat(pcWriteBuffer,_DIVIDER_LINE_,strlen(_DIVIDER_LINE_) +1 );

	sprintf(tmp,"HAL Version : %04lX\r\n",HAL_GetHalVersion());
	strncat(pcWriteBuffer,tmp,strlen(tmp));

	sprintf(tmp,"OS  Version : %d.%d.%d\r\n",tskKERNEL_VERSION_MAJOR,
			                                 tskKERNEL_VERSION_MINOR,
											 tskKERNEL_VERSION_BUILD);
	strncat(pcWriteBuffer,tmp,strlen(tmp));

	sprintf(tmp,"OS  Build   : %s\r\n",completeVersion);
	strncat(pcWriteBuffer,tmp,strlen(tmp));


	strncat(pcWriteBuffer,_DIVIDER_LINE_,strlen(_DIVIDER_LINE_) +1 );

	vPortFree(tmp);

	return pdFALSE;
}

static const CLI_Command_Definition_t xDeviceInfo =
{
	"device-info",
	"\r\ndevice-info:\r\n Display device information\r\n",
	prvDeviceInfoCommand,
	0 /* No parameters are expected. */
};
/*-----------------------------------------------------------*/
static BaseType_t prvFlashReadCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	const char *pcParameter;
	BaseType_t xParameterStringLength, xReturn;
	static UBaseType_t uxParameterNumber = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint32_t flash_address = 0;

	memset( pcWriteBuffer, 0x00, xWriteBufferLen );

	if( uxParameterNumber == 0 )
	{
		sprintf( pcWriteBuffer, "Flash Address 0x" );
		uxParameterNumber = 1U;
		xReturn = pdPASS;
	}
	else
	{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							uxParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
						);

		/* Sanity check something was returned. */
		configASSERT( pcParameter );

		/* Return the parameter string. */
		memset( pcWriteBuffer, 0x00, xWriteBufferLen );

		flash_address = strtol(pcParameter, NULL, 16);

		sprintf( pcWriteBuffer, "%lX : 0x%llX\r\n",flash_address,(*(__IO uint64_t *) flash_address));
		/* If this is the last of the three parameters then there are no more
		strings to return after this one. */

		/* If this is the last of the three parameters then there are no more strings to return after this one. */
		xReturn = pdFALSE;
		uxParameterNumber = 0;
	}

	return xReturn;
}

static const CLI_Command_Definition_t xFlashRead =
{
	"flash-read",
	"\r\nflash-read:\r\n Read data form Flash\r\n",
	prvFlashReadCommand,
	1 /* No parameters are expected. */
};

/*-----------------------------------------------------------*/
static uint32_t GetPage(uint32_t Address)
{
	uint32_t page = 0U;

	page = (Address - 0x08000000) / FLASH_PAGE_SIZE;

	return page;
}

static uint32_t EraseFlash(uint32_t PageAddress, uint32_t NumberPage) {
    FLASH_EraseInitTypeDef eraseinitstruct;
    uint32_t _pageError;

    eraseinitstruct.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseinitstruct.Page = GetPage(PageAddress);
    eraseinitstruct.NbPages = 1U;

    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    HAL_FLASHEx_Erase(&eraseinitstruct, &_pageError);
    HAL_FLASH_Lock();

    return _pageError;
}

static HAL_StatusTypeDef ProgramFlash(uint32_t Address, uint64_t Data) {
    HAL_StatusTypeDef _status;

    HAL_FLASH_Unlock();
    _status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, Data);
    HAL_FLASH_Lock();
    return _status;
}

static BaseType_t prvFlashWrite( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	const char *pcParameter;
	BaseType_t xParameterStringLength, xReturn;
	static UBaseType_t uxParameterNumber = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	static uint32_t flash_address = 0;
	static uint64_t flash_data    = 0;

	memset( pcWriteBuffer, 0x00, xWriteBufferLen );

	if( uxParameterNumber == 0 )
	{
		sprintf( pcWriteBuffer, "Write Flash Address 0x" );
		uxParameterNumber = 1U;
		xReturn = pdPASS;
	}
	else
	{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							uxParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
						);

		/* Sanity check something was returned. */
		configASSERT( pcParameter );

		memset( pcWriteBuffer, 0x00, xWriteBufferLen );

		switch (uxParameterNumber)
		{
			case 1:
				flash_address = strtol(pcParameter, NULL, 16);

				xReturn = pdTRUE;
				uxParameterNumber++;
				break;

			case 2:
				flash_data    = strtoll(pcParameter, NULL, 16);

				sprintf( pcWriteBuffer, "0x%lX : 0x%llX ",flash_address,flash_data);

				if (EraseFlash(flash_address, 1) == 0xFFFFFFFF) {
				    if (ProgramFlash(flash_address, flash_data) == HAL_OK) {
				        strncat(pcWriteBuffer,"Okay!\n",strlen("Okay!\n")+1);
				    } else {
				        strncat(pcWriteBuffer,"Error!\n",strlen("Error!\n")+1);
				    }
				} else {
				    //printf("Erase error: %x\n", (int)pageError);
				}

				flash_address = 0;
				flash_data    = 0;
				xReturn = pdFALSE;
				uxParameterNumber = 0;
				break;
		}
	}

	return xReturn;
}

static const CLI_Command_Definition_t xFlashWrite =
{
	"flash-write",
	"\r\nflash-write:\r\n Write data to Flash\r\n",
	prvFlashWrite,
	2 /* No parameters are expected. */
};
/*-----------------------------------------------------------*/
void vRegisterSampleCLICommands( void )
{
	/* Register all the command line commands defined immediately above. */
	FreeRTOS_CLIRegisterCommand( &xDeviceReset );
	FreeRTOS_CLIRegisterCommand( &xDeviceInfo );
	FreeRTOS_CLIRegisterCommand( &xFlashRead );
	FreeRTOS_CLIRegisterCommand( &xFlashWrite );
	FreeRTOS_CLIRegisterCommand( &xTaskStats );


	#if( configGENERATE_RUN_TIME_STATS == 1 )
	{
		FreeRTOS_CLIRegisterCommand( &xRunTimeStats );
	}
	#endif
	
	#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	{
		FreeRTOS_CLIRegisterCommand( &xQueryHeap );
	}
	#endif

	#if( configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1 )
	{
		FreeRTOS_CLIRegisterCommand( &xStartStopTrace );
	}
	#endif
}
/*-----------------------------------------------------------*/
#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )

static BaseType_t prvTaskStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	sprintf(pcWriteBuffer,_DIVIDER_LINE_);

	strncat(pcWriteBuffer,"Name\t\tState\tPri\tStack\tId\r\n",strlen("Name\t\tState\tPri\tStack\tId\r\n") +1 );

	strncat(pcWriteBuffer,_DIVIDER_LINE_,strlen(_DIVIDER_LINE_) +1 );

	vTaskList(pcWriteBuffer + strlen(pcWriteBuffer));

	strncat(pcWriteBuffer,_DIVIDER_LINE_,strlen(_DIVIDER_LINE_) +1 );

	return pdFALSE;
}
#endif /* ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) ) */

/*-----------------------------------------------------------*/

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )

	static BaseType_t prvQueryHeapCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		sprintf( pcWriteBuffer, "Current free heap %d bytes, minimum ever free heap %d bytes\r\n", ( int ) xPortGetFreeHeapSize(), ( int ) xPortGetMinimumEverFreeHeapSize() );

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}

#endif /* configINCLUDE_QUERY_HEAP */
/*-----------------------------------------------------------*/

#if( configGENERATE_RUN_TIME_STATS == 1 )
	
	static BaseType_t prvRunTimeStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
	const char * const pcHeader = "  Abs Time      % Time\r\n****************************************\r\n";
	BaseType_t xSpacePadding;

		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		/* Generate a table of task stats. */
		strcpy( pcWriteBuffer, "Task" );
		pcWriteBuffer += strlen( pcWriteBuffer );

		/* Pad the string "task" with however many bytes necessary to make it the
		length of a task name.  Minus three for the null terminator and half the
		number of characters in	"Task" so the column lines up with the centre of
		the heading. */
		for( xSpacePadding = strlen( "Task" ); xSpacePadding < ( configMAX_TASK_NAME_LEN - 3 ); xSpacePadding++ )
		{
			/* Add a space to align columns after the task's name. */
			*pcWriteBuffer = ' ';
			pcWriteBuffer++;

			/* Ensure always terminated. */
			*pcWriteBuffer = 0x00;
		}

		strcpy( pcWriteBuffer, pcHeader );
		vTaskGetRunTimeStats( pcWriteBuffer + strlen( pcHeader ) );

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}
	
#endif /* configGENERATE_RUN_TIME_STATS */

/*-----------------------------------------------------------*/

#if configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1

	static BaseType_t prvStartStopTraceCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
	const char *pcParameter;
	BaseType_t lParameterStringLength;

		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							1,						/* Return the first parameter. */
							&lParameterStringLength	/* Store the parameter string length. */
						);

		/* Sanity check something was returned. */
		configASSERT( pcParameter );

		/* There are only two valid parameter values. */
		if( strncmp( pcParameter, "start", strlen( "start" ) ) == 0 )
		{
			/* Start or restart the trace. */
			vTraceStop();
			vTraceClear();
			vTraceStart();

			sprintf( pcWriteBuffer, "Trace recording (re)started.\r\n" );
		}
		else if( strncmp( pcParameter, "stop", strlen( "stop" ) ) == 0 )
		{
			/* End the trace, if one is running. */
			vTraceStop();
			sprintf( pcWriteBuffer, "Stopping trace recording.\r\n" );
		}
		else
		{
			sprintf( pcWriteBuffer, "Valid parameters are 'start' and 'stop'.\r\n" );
		}

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}

#endif /* configINCLUDE_TRACE_RELATED_CLI_COMMANDS */
