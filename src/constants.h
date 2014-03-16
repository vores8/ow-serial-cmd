/*
 * constants.h
 *
 *  Created on: 16 марта 2014 г.
 *      Author: dbkrasn
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

   /** Default port for NetAdapter TCP/IP connection */
#define DEFAULT_PORT 6161

   /*------------------------------------------------------------*/
   /*----- Method Return codes ----------------------------------*/
   /*------------------------------------------------------------*/
#define RET_SUCCESS                       (uint8_t)0x0FF
#define RET_FAILURE                       (uint8_t)0x0F0
   /*------------------------------------------------------------*/

   /*------------------------------------------------------------*/
   /*----- Method command bytes ---------------------------------*/
   /*------------------------------------------------------------*/
#define CMD_CLOSECONNECTION               0x08
#define CMD_PINGCONNECTION                0x09
   /*------------------------------------------------------------*/
   /* Raw Data methods ------------------------------------------*/
#define CMD_RESET                         0x10
#define CMD_PUTBIT                        0x11
#define CMD_PUTBYTE                       0x12
#define CMD_GETBIT                        0x13
#define CMD_GETBYTE                       0x14
#define CMD_GETBLOCK                      0x15
#define CMD_DATABLOCK                     0x16
   /*------------------------------------------------------------*/
   /* Power methods ---------------------------------------------*/
#define CMD_SETPOWERDURATION              0x17
#define CMD_STARTPOWERDELIVERY            0x18
#define CMD_SETPROGRAMPULSEDURATION       0x19
#define CMD_STARTPROGRAMPULSE             0x1A
#define CMD_STARTBREAK                    0x1B
#define CMD_SETPOWERNORMAL                0x1C
   /*------------------------------------------------------------*/
   /* Speed methods ---------------------------------------------*/
#define CMD_SETSPEED                      0x1D
#define CMD_GETSPEED                      0x1E
   /*------------------------------------------------------------*/
   /* Network Semaphore methods ---------------------------------*/
#define CMD_BEGINEXCLUSIVE                0x1F
#define CMD_ENDEXCLUSIVE                  0x20
   /*------------------------------------------------------------*/
   /* Searching methods -----------------------------------------*/
#define CMD_FINDFIRSTDEVICE               0x21
#define CMD_FINDNEXTDEVICE                0x22
#define CMD_GETADDRESS                    0x23
#define CMD_SETSEARCHONLYALARMINGDEVICES  0x24
#define CMD_SETNORESETSEARCH              0x25
#define CMD_SETSEARCHALLDEVICES           0x26
#define CMD_TARGETALLFAMILIES             0x27
#define CMD_TARGETFAMILY                  0x28
#define CMD_EXCLUDEFAMILY                 0x29
   /*------------------------------------------------------------*/
   /* feature methods -------------------------------------------*/
#define CMD_CANBREAK                      0x2A
#define CMD_CANDELIVERPOWER               0x2B
#define CMD_CANDELIVERSMARTPOWER          0x2C
#define CMD_CANFLEX                       0x2D
#define CMD_CANHYPERDRIVE                 0x2E
#define CMD_CANOVERDRIVE                  0x2F
#define CMD_CANPROGRAM                    0x30
   /*------------------------------------------------------------*/


#endif /* CONSTANTS_H_ */
