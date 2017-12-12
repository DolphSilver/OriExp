/*
 * coordinates.h
 *
 * Created: 2017-11-30 08:08:13
 *  Author: A243403
 */ 


#ifndef COORDINATES_H_
#define COORDINATES_H_


#define MAP_ROWS 3
#define MAP_COLS 5

#define N_POS_INDEX 2*MAP_ROWS + 2*MAP_COLS //is this allows (must be computed at compile time..?)
#define N_SH_POS 3

//											   1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16
static const int x_coord_index[N_POS_INDEX] = {0, 1, 2, 3, 4, 4, 4, 4, 4, 3, 2, 1, 0, 0, 0, 0};
static const int y_coord_index[N_POS_INDEX] = {2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2};
	
//												 1,   2,   3
static const float x_coord_sh_index[N_SH_POS] = {1.5, 3.5, 0.5};
static const float y_coord_sh_index[N_SH_POS] = {1.0, 0.0, 0.0};

#endif /* COORDINATES_H_ */