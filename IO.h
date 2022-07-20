#pragma once
#ifndef ___IO___
#define ___IO___


/******************* Includes ***********************/
#include "ImgPos.h"
#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************* Defines ***********************/

#define TRUE 1
#define FALSE 0

/******************* Typedefs ***********************/

typedef int BOOL;


/******************* Function Prototypes ***********************/

/*Receives a gray image's values from the user. Returns the grey image.*/
grayImage getGrayImage(int* rows, int* cols);

/*Receieves a 2d array of characters with an unknown length from the user.*/
unsigned char** getStrArray(int rows, int cols);

/*Prints a 2d array.*/
void printArray(char** str, int rows, int cols);

/*Prints a segment's values.*/
void printSegment(Segment* seg, grayImage* img);

/*Prints the coordinates of all the cells in a linked list.*/
void printList(imgPosCell* list_start);

/*Prints the values of the image.*/
void printGrayImage(grayImage* img);

/*Prints the values of all the cells in a linked list.*/
void printListValues(grayImage* img, imgPosCell* list_start);

void getStr(char** str);

#endif 