#ifndef _LINKED_LIST_
#define _LINKED_LIST_
#pragma once

/******************* Defines ***********************/



/******************* Includes ***********************/

#include "ImgPos.h"
#include "IO.h"


/******************* Typedefs ***********************/



/******************* Function Prototypes ***********************/

/*Turns a given segment into an inmPosCell list. Returns the first (or last - depends on where you're looking from) cell in the list.*/
imgPosCell* turnSegmentToImgPosCell(Segment* seg);

/*Sorts an imgPosCell list.*/
void mergeSort(imgPosCell** list_start);

/*Returns the length of an imgPosCell list.*/
int listLen(imgPosCell** list_start);

/*Sorts an imgPosCell list of voters.*/
void sortListOfLists(imgPosCell*** list, int size);

#endif