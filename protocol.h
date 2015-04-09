/*
 * Protocol.h
 *
 *  Created on: 22 Nov 2014
 *      Author: sash
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_


/*******************************
 Protocl Structure
 *******************************/

typedef struct Protocol_struct
{
	unsigned char Pin_1;
	unsigned char Pin_2;
	unsigned char Pin_3;
	unsigned char Pin_4;

	unsigned char Function;

	unsigned char Language;
	unsigned char Difficulty;

	unsigned int Exhibit;

}Protocol_struct;

#endif
