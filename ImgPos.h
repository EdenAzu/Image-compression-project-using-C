#pragma once
#ifndef IMGPOS
#define IMGPOS


/******************* Includes ***********************/
#include <stdio.h>
#include <stdlib.h>

/******************* Defines ***********************/
#define MAX_VALUE 255

/******************* Typedefs ***********************/

typedef struct _grayImage {
	unsigned short rows, cols;
	unsigned char** pixels;
} grayImage; 

typedef unsigned short imgPos[2];

typedef struct _imgPosCell {
	imgPos position;
	struct _imgPosCell* next, * prev;
}imgPosCell;


/******************* Function Prototypes ***********************/

/*Finds all the segments in a given grey image. Returns the number of segments found.*/
unsigned int findAllSegments(grayImage* img, unsigned char threshold, imgPosCell*** segments);

/*Allocates a gray image.*/
void allocateGrayImage(grayImage* img, unsigned short rows, unsigned short cols);

/*Colours a grey image according to the formula. Returns a pointer to the new grayImage.*/
grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size);

void freeListOfLists(imgPosCell*** list, int list_size);

void freeGrayImage(grayImage* img);

#endif 