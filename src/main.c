/*
 * main.c
 *
 *  Created on: 16 ����� 2014 �.
 *      Author: dbkrasn
 */

#include <uart.h>
#include <polled/OWIBitFunctions.h>
#include <polled/OWIHighLevelFunctions.h>

#include "constants.h"
#include "tmexapi.h"

int currentBus = 0;
unsigned char BUSES = 0;
int numBuses = 0;
int _presence = 0;

unsigned char buses[5] = { OWI_PIN_3, OWI_PIN_4, OWI_PIN_5, OWI_PIN_6, OWI_PIN_7 };

#define MAX_DATABLOCK	64

void closeConnection(void) {

}

void adapterReset(void) {

}

void adapterPutBit(void) {
	uint8_t val = uart_getc();
	if (val == 0) {
		OWI_WriteBit0(buses[currentBus]);
		uart_putc(RET_SUCCESS);
	} else if (val == 1) {
		OWI_WriteBit1(buses[currentBus]);
		uart_putc(RET_SUCCESS);
	} else {
		uart_putc(RET_FAILURE);
	}
	uart_flush();
}

void adapterPutByte(void) {
	uint8_t val = uart_getc();
	OWI_SendByte(val, buses[currentBus]);
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterGetBit(void) {
	unsigned char ret = OWI_ReadBit(buses[currentBus]);
	uart_putc(RET_SUCCESS);
	uart_putc(ret);
	uart_flush();
}

void adapterGetByte(void) {
	unsigned char ret = OWI_ReceiveByte(buses[currentBus]);
	uart_putc(RET_SUCCESS);
	uart_putc(ret);
	uart_flush();
}

void adapterGetBlock(void) {
	unsigned char len = uart_getc();

	uart_putc(RET_SUCCESS);

	for (int i = 0; i < len; i++) {
		OWI_SendByte(0xFF, buses[currentBus]);
	}
	for (int i = 0; i < len; i++) {
		unsigned char ret = OWI_ReceiveByte(buses[currentBus]);
		uart_putc(ret);
	}
	uart_flush();

}

void adapterDataBlock(void) {

	unsigned char dataBlock[MAX_DATABLOCK];
	unsigned char len = uart_getc();

	for (int i = 0; i < len; i++) {
		dataBlock[i] = uart_getc();
	}

	unsigned char pos = len - 1;
	while (dataBlock[pos] == 0xFF) {
		pos--;
	}

	pos++;

	for (int i = 0; i < pos; i++) {
		OWI_SendByte(dataBlock[i], buses[currentBus]);
	}

	unsigned char ans = len - pos;
	if (ans > 0) {
		for (int i = pos; i < len; i++) {
			dataBlock[i] = OWI_ReceiveByte(buses[currentBus]);
		}
	}

	uart_putc(RET_SUCCESS);
	for (int i = 0; i < len; i++) {
		uart_putc(dataBlock[i]);
	}
	uart_flush();
}

int main(void) {

	uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(9600, F_CPU));

	while (1) {

		if (uart_available() > 0) {

			uint8_t cmd = (uint8_t) uart_getc();

			switch (cmd) {
			/* Connection keep-alive and close commands */
			case CMD_CLOSECONNECTION:
				closeConnection();
				break;
				/* Raw Data commands */
			case CMD_RESET:
				adapterReset();
				break;
			case CMD_PUTBIT:
				adapterPutBit();
				break;
			case CMD_PUTBYTE:
				adapterPutByte();
				break;
			case CMD_GETBIT:
				adapterGetBit();
				break;
			case CMD_GETBYTE:
				adapterGetByte();
				break;
			case CMD_GETBLOCK:
				adapterGetBlock();
				break;
			case CMD_DATABLOCK:
				adapterDataBlock();
				break;
				/* Power methods */
			case CMD_SETPOWERDURATION:
				adapterSetPowerDuration();
				break;
			case CMD_STARTPOWERDELIVERY:
				adapterStartPowerDelivery();
				break;
			case CMD_SETPROGRAMPULSEDURATION:
				adapterSetProgramPulseDuration();
				break;
			case CMD_STARTPROGRAMPULSE:
				adapterStartProgramPulse();
				break;
			case CMD_STARTBREAK:
				adapterStartBreak();
				break;
			case CMD_SETPOWERNORMAL:
				adapterSetPowerNormal();
				break;
				/* Speed methods */
			case CMD_SETSPEED:
				adapterSetSpeed();
				break;
			case CMD_GETSPEED:
				adapterGetSpeed();
				break;
				/* Network Semaphore methods */
			case CMD_BEGINEXCLUSIVE:
				adapterBeginExclusive();
				break;
			case CMD_ENDEXCLUSIVE:
				adapterEndExclusive();
				break;
				/* Searching methods */
			case CMD_FINDFIRSTDEVICE:
				adapterFindFirstDevice();
				break;
			case CMD_FINDNEXTDEVICE:
				adapterFindNextDevice();
				break;
			case CMD_GETADDRESS:
				adapterGetAddress();
				break;
			case CMD_SETSEARCHONLYALARMINGDEVICES:
				adapterSetSearchOnlyAlarmingDevices();
				break;
			case CMD_SETNORESETSEARCH:
				adapterSetNoResetSearch();
				break;
			case CMD_SETSEARCHALLDEVICES:
				adapterSetSearchAllDevices();
				break;
			case CMD_TARGETALLFAMILIES:
				adapterTargetAllFamilies();
				break;
			case CMD_TARGETFAMILY:
				adapterTargetFamily();
				break;
			case CMD_EXCLUDEFAMILY:
				adapterExcludeFamily();
				break;
				/* feature methods */
			case CMD_CANBREAK:
				adapterCanBreak();
				break;
			case CMD_CANDELIVERPOWER:
				adapterCanDeliverPower();
				break;
			case CMD_CANDELIVERSMARTPOWER:
				adapterCanDeliverSmartPower();
				break;
			case CMD_CANFLEX:
				adapterCanFlex();
				break;
			case CMD_CANHYPERDRIVE:
				adapterCanHyperdrive();
				break;
			case CMD_CANOVERDRIVE:
				adapterCanOverdrive();
				break;
			case CMD_CANPROGRAM:
				adapterCanProgram();
				break;
			default:
				break;
			}
		}
	}
}
