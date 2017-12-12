/*
 * destinationList.h
 *
 * Created: 2017-11-29 20:02:54
 *  Author: A243403
 */ 


#ifndef DESTINATIONLIST_H_
#define DESTINATIONLIST_H_


typedef enum {
	READ_BARCODE,
	READ_SH_CODE,
	FINISHED
}
OBJ_TYPE;

typedef enum {
	NORTH,
	NORTH_WEST,
	WEST,
	SOUTH_WEST,
	SOUTH,
	SOUTH_EAST,
	EAST,
	NOTH_EAST
}
DIR_TYPE;

void addDestination(posStruct pos, int sh_flag)

#endif /* DESTINATIONLIST_H_ */