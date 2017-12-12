/*
 * coordinates.c
 *
 * Created: 2017-11-30 08:21:30
 *  Author: A243403
 */ 

#include

static posStruct coordIndexList_object[N_POS_INDEX];
static posStruct SHcoordIndexList_object[N_SH_POS];

void initiateCoordIndexList(void){
	auto posStruct coord;
	auto int i;
	for(i=0; i<N_POS_INDEX; i++){
		coord.pos_x = x_coord_index[i];
		coord.pos_y = y_coord_index[i];
		coordIndexList_object[i] = coord; //do this work, actually copys, no pointer larv..?
	}
}
void initiateSHcoordIndexList(void){
	auto posStruct coord;
	auto int i;
	for(i=0; i<N_SH_POS; i++){
		coord.pos_x = x_coord_sh_index[i];
		coord.pos_y = y_coord_sh_index[i];
		SHcoordIndexList_object[i] = coord; //do this work, actually copys, no pointer larv..?
	}
}

posStruct index2coord(int index){
	posStruct result = coordIndexList_object[index];
	return result;
}

posStruct SHindex2coord(int sh_index){
	posStruct result = SHcoordIndexList_object[sh_index];
	return result;
}


