/*
 * common.h
 *
 *  Created on: Nov 23, 2017
 *      Author: root
 */

#ifndef COMMON_H_
#define COMMON_H_

typedef struct{
	char type[6];
	char time[10];
	char foo[2];
	char lat[12];
	char ns[3];
	char lon[12];
	char we[3];
	char speed[6];
	char angle[7];
	char date[7];
} GPS_frame;





#endif /* COMMON_H_ */
