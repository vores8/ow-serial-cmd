/*
 * tmexapi.h
 *
 *  Created on: 16 марта 2014 г.
 *      Author: dbkrasn
 */

#ifndef TMEXAPI_H_
#define TMEXAPI_H_

// definitions
#define FALSE 0
#define TRUE  1

// method declarations
int  OWFirst();
int  OWNext();
int  OWVerify();
void OWTargetSetup(unsigned char family_code);
void OWFamilySkipSetup();
int  OWReset();
void OWWriteByte(unsigned char byte_value);
void OWWriteBit(unsigned char bit_value);
unsigned char OWReadBit();
int  OWSearch();
unsigned char docrc8(unsigned char value);


#endif /* TMEXAPI_H_ */
