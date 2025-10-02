/*
 * cb350m.h
 *
 * Created: 2025-09-10 오후 3:32:11
 *  Author: YJKIM
 */ 


#ifndef CB350M_H_
#define CB350M_H_

#include "can.h"

#define CANID_CB350M_1	301
#define CANID_CB350M_2	302
#define CANID_CB350M_3	303
#define CANID_CB350M_4	304

#define UNITFLAG	6

enum {INDEX_CB350M_1, INDEX_CB350M_2, INDEX_CB350M_3, INDEX_CB350M_4};

struct CB350M
{
	boolean unit;
	long current;
	};
	
struct CB350M cb350m[4];

void saveCb350m(unsigned int index, char *data);
#endif /* CB350M_H_ */