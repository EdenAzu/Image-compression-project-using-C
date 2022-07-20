#define _CRT_SECURE_NO_WARNINGS
#pragma once
/******************* Includes ***********************/
#include "IO.h"
#include "ImgPos.h"
#include"Linked_List.h"
#include "Tree.h"

/******************* Defines ***********************/
#define ALLOCATION_ERROR -2
#define allocation_error(NAME, FUNC_NAME){										\
	fprintf(stderr, "Memory allocation of %s failed at %s.", NAME, FUNC_NAME);	\
	exit(ALLOCATION_ERROR);														\
}

/******************* Typedefs ***********************/


/******************* Static Function Prototypes ***********************/

/*Prints the segments recursively.*/
static void printSegmentAux(treeNode* root, grayImage* img);

static void strRealloc(char** str, int desired_size);


/******************* Static Function Implementations ***********************/

static void strRealloc(char** str, int desired_size)
{
	*str = (char*)realloc((*str), desired_size * sizeof(char));
	if (!(*str))
		allocation_error("*str", "strRealloc");
}


/******************* Function Implementations ***********************/

grayImage getGrayImage(int* rows, int* cols) {

	grayImage img;

	printf("Enter the number of rows: ");
	scanf("%d", rows);
	printf("Enter the number of columns: ");
	scanf("%d", cols);

	img.cols = *cols;
	img.rows = *rows;
	img.pixels = getStrArray(*rows, *cols);

	return img;
}

unsigned char** getStrArray(int rows, int cols)
{
	unsigned char** startArr;
	int i, j;
	startArr = (unsigned char**)calloc(rows, sizeof(unsigned char*));
	if (!(startArr))
		allocation_error("startArr" , "getStrArray");

	printf("Enter the values to insert: ");

	for (i = 0; i < rows; i++)
	{
		startArr[i] = (unsigned char*)calloc(cols, sizeof(unsigned char));
		if (!startArr[i])
			allocation_error("startArr[i]", "startArr[i]");

		for (j = 0; j < cols; j++)
			scanf("%hhu", &startArr[i][j]);
	}
	
	return startArr;
}

void printArray(char** str, int rows, int cols)
{
	int i, j;

	for (i = 0; i < rows; i++)
	{

		for (j = 0; j < cols; j++)
		{
			printf("%hhu ", str[i][j]);
		}
		putchar('\n');
	}

}

void printSegment(Segment* seg, grayImage* img)
{
	printf("%d ", img->pixels[seg->root->position[0]][seg->root->position[1]]);
	printSegmentAux(seg->root, img);
	putchar('\n');
}

void printGrayImage(grayImage* img) {
	int i, j;

	for (i = 0; i < img->rows; i++) {
		for (j = 0; j < img->cols; j++) {
			printf("%4d ", img->pixels[i][j]);
		}
		putchar('\n');
	}
}

void printList(imgPosCell* list_start)
{
	imgPosCell* temp;

	temp = list_start;

	while (temp)
	{
		printf("(%d, %d) ", (temp->position)[0], (temp->position)[1]);
		temp = temp->next;
	}
}

void printListValues(grayImage* img, imgPosCell* list_start)
{
	imgPosCell* temp;

	temp = list_start;

	while (temp)
	{
		printf("%d, ", (img->pixels)[(temp->position)[0]][(temp->position)[1]]);
		temp = temp->next;
	}
}

static void printSegmentAux(treeNode* root, grayImage* img)
{
	int i;

	//printf("%d\n", img->pixels[root->position[0]][root->position[1]]);
	for (i = 0; root->similar_neighbors[i]!=NULL ;i++) {
		printf("%d ", img->pixels[root->similar_neighbors[i]->position[0]][root->similar_neighbors[i]->position[1]]);
	}

	for (i = 0; root->similar_neighbors[i] != NULL; i++) {
		printSegmentAux(root->similar_neighbors[i], img);
	}

}

void getStr(char** str)
{
	char c;
	int i, size = 2;
	*str = (char*)malloc(size * sizeof(char));
	if (!(*str))
		allocation_error("*str", "getStr");

	c = getchar();
	if (c != '\n')
		ungetc(c, stdin);
	c = getchar();
	for (i = 0; c != '\n'; i++)
	{
		if (i == size)
		{
			size = size * 2;
			*str = (char*)realloc(*str, size * sizeof(char));
			if (!(*str))
				allocation_error("*str", "getStr");
		}
		(*str)[i] = c;
		c = getchar();
	}
	*str = (char*)realloc(*str, (i + 1) * sizeof(char));
	if (!(*str))
		allocation_error("*str", "getStr");

	(*str)[i] = '\0';
}