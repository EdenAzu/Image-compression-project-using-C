#pragma once
#ifndef _TREE
#define _TREE

/******************* Includes ***********************/
#include <stdio.h>
#include <stdlib.h>
#include "ImgPos.h"

/******************* Defines ***********************/

#define MIN_NEIGHBORS 1
#define MAX_NEIGHBORS 9

/******************* Typedefs ***********************/


typedef struct _treeNode {
	imgPos position;
	struct _treeNode** similar_neighbors; /*Children array.*/
} treeNode;

typedef struct _segment {
	treeNode* root;
	unsigned int size; /*Total number of tree nodes.*/
} Segment;


/******************* Function Prototypes ***********************/

/*Finds a single segment recursively.*/
Segment findSingleSegment(grayImage* img, imgPos kernel, unsigned char threshold);

/*Uses the same approach as findSingleSegment, but with a given flags_array so it could ignore existing segment parts.*/
Segment f_findSingleSegment(grayImage* img, int*** flags_array, imgPos kernel, unsigned char threshold);



#endif