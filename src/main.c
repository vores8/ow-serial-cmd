/*
 * main.c
 *
 *  Created on: 16 марта 2014 г.
 *      Author: dbkrasn
 */

#include <uart.h>
#include <polled/OWIBitFunctions.h>
#include <polled/OWIHighLevelFunctions.h>
#include <common_files/OWIcrc.h>

#include "constants.h"

#define NUM_BUSES 	3

int currentBus = 0;
unsigned char BUSES = 0;
int numBuses = 0;
int _presence = 0;

unsigned char ROM_NO[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int LastDiscrepancy;
int LastFamilyDiscrepancy;
int LastDeviceFlag;
unsigned char crc8;

unsigned char buses[5] =
		{ OWI_PIN_3, OWI_PIN_4, OWI_PIN_5, OWI_PIN_6, OWI_PIN_7 };

#define MAX_DATABLOCK	64

void closeConnection(void) {
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterReset(void) {
	uart_putc(RET_SUCCESS);
	uart_flush();
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

void adapterSetPowerDuration(void) {
	uart_getc();
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterStartPowerDelivery(void) {
	uart_getc();
	uart_putc(RET_SUCCESS);
	uart_putc(FALSE);
	uart_flush();
}

void adapterSetProgramPulseDuration(void) {
	uart_getc();
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterStartProgramPulse(void) {
	uart_getc();
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterStartBreak(void) {
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterSetPowerNormal(void) {
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterSetSpeed(void) {
	uart_getc();
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterGetSpeed(void) {
	uart_putc(RET_SUCCESS);
	uart_putc(0); //always return SPEED_REGULAR
	uart_flush();
}

void adapterBeginExclusive(void) {
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterEndExclusive(void) {
	uart_putc(RET_SUCCESS);
	uart_flush();
}

int OWSearch() {

	if (LastDeviceFlag) {
		// reset the search
		LastDiscrepancy = 0;
		LastDeviceFlag = FALSE;
		LastFamilyDiscrepancy = 0;
		currentBus = 0;
		return FALSE;
	}

	while (!(_presence & buses[currentBus])) {
		currentBus++;
	}

	if (currentBus == numBuses) {
		LastDeviceFlag = TRUE;
		return FALSE;
	} else {

		OWI_DetectPresence(buses[currentBus]);
		LastDiscrepancy = OWI_SearchRom(ROM_NO, LastDiscrepancy,
				buses[currentBus]);
		if (LastDiscrepancy == OWI_ROM_SEARCH_FINISHED) {
			currentBus++;
		}
	}
	return OWI_CheckRomCRC(ROM_NO) == OWI_CRC_OK;
}

void adapterFindFirstDevice(void) {
	// reset the search state
	LastDiscrepancy = 0;
	LastDeviceFlag = FALSE;
	LastFamilyDiscrepancy = 0;
	currentBus = 0;
	_presence = OWI_DetectPresence(BUSES);
	uart_putc(RET_SUCCESS);
	uart_putc(OWSearch());
	uart_flush();
}

void adapterFindNextDevice(void) {
	uart_putc(RET_SUCCESS);
	uart_putc(OWSearch());
	uart_flush();
}

void adapterGetAddress(void) {
	uart_putc(RET_SUCCESS);
	for (int i = 0; i < 8; i++) {
		uart_putc(ROM_NO[i]);
	}
	uart_flush();
}

void adapterSetSearchOnlyAlarmingDevices(void) {
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterSetNoResetSearch(void) {
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterSetSearchAllDevices(void) {
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterTargetAllFamilies(void) {
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterTargetFamily(void) {
	uart_getc();
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterExcludeFamily(void) {
	uart_getc();
	uart_putc(RET_SUCCESS);
	uart_flush();
}

void adapterCanBreak(void) {
	uart_putc(RET_SUCCESS);
	uart_putc(FALSE);
	uart_flush();
}

void adapterCanDeliverPower(void) {
	uart_putc(RET_SUCCESS);
	uart_putc(FALSE);
	uart_flush();
}

void adapterCanDeliverSmartPower(void) {
	uart_putc(RET_SUCCESS);
	uart_putc(FALSE);
	uart_flush();
}

void adapterCanFlex(void) {
	uart_putc(RET_SUCCESS);
	uart_putc(FALSE);
	uart_flush();
}

void adapterCanHyperdrive(void) {
	uart_putc(RET_SUCCESS);
	uart_putc(FALSE);
	uart_flush();
}

void adapterCanOverdrive(void) {
	uart_putc(RET_SUCCESS);
	uart_putc(FALSE);
	uart_flush();
}

void adapterCanProgram(void) {
	uart_putc(RET_SUCCESS);
	uart_putc(FALSE);
	uart_flush();
}

int main(void) {

	uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(9600, F_CPU));

	BUSES = 0;
	numBuses = NUM_BUSES > 5 ? 5 : NUM_BUSES;
	for (int i = 0; i < numBuses; i++)
		BUSES |= buses[i];
	OWI_Init(BUSES);

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
