#pragma once
#define _CRT_SECURE_NO_WARNINGS

/******************* Includes ***********************/

#include "Linked_List.h"
#include "IO.h"
#include "ImgPos.h"
#include "Tree.h"

/******************* Defines ***********************/

#define ALLOCATION_ERROR -1
#define allocation_error(NAME, FUNC_NAME){										\
	fprintf(stderr, "Memory allocation of %s failed at %s.", NAME, FUNC_NAME);	\
	exit(ALLOCATION_ERROR);														\
}

/******************* Typedefs ***********************/



/******************* Static Function Prototypes ***********************/


/*Allocates a single cell.*/
static imgPosCell* allocateListNode(treeNode* node_to_insert);

/*Inserts a node to the beginning of a given list. If list_start is NULL, creates a new node. Returns a pointer to the new beginning of the list.*/
static imgPosCell* insertToBeginningOfList(imgPosCell* list_start, treeNode* node_to_insert);

/*Recursively inserts the nodes of a given tree into a given list.*/
static imgPosCell* turnSegmentToImgPosCellAux(imgPosCell* list_start, treeNode* root);

/*Assists the function mergeSort by comparing and merging two lists.*/
static imgPosCell* sortList(imgPosCell* first_part, imgPosCell* second_part);

/*Splits a list into two parts.*/
static void splitList(imgPosCell* list_start, imgPosCell** first_part, imgPosCell** second_part);


/******************* Static Function Implementations ***********************/

static imgPosCell* allocateListNode(treeNode* node_to_insert)
{
	imgPosCell* cell = (imgPosCell*)malloc(sizeof(imgPosCell));

	cell->next = NULL;
	cell->prev = NULL;
	(cell->position)[0] = (node_to_insert->position)[0];
	(cell->position)[1] = (node_to_insert->position)[1];

	return cell;
}

static imgPosCell* insertToBeginningOfList(imgPosCell* list_start, treeNode* node_to_insert)
{
	imgPosCell* cell;

	if (!list_start)
	{
		cell = allocateListNode(node_to_insert);
		list_start = cell;
		return cell;
	}

	cell = allocateListNode(node_to_insert);
	cell->next = list_start;
	list_start->prev = cell;

	return cell;
}

static imgPosCell* turnSegmentToImgPosCellAux(imgPosCell* list_start, treeNode* root)
{
	int i;
	imgPosCell* head;

	head = insertToBeginningOfList(list_start, root);

	/*Inserts the current root's neighbors into the list.*/
	for (i = 0; root->similar_neighbors[i] != NULL; i++) {
		head = turnSegmentToImgPosCellAux(head, (root->similar_neighbors)[i]);
	}

	return head;

}

static void splitList(imgPosCell* list_start, imgPosCell** first_part, imgPosCell** second_part)
{
	imgPosCell* one_step_index;
	imgPosCell* two_steps_index; 
	one_step_index = list_start;
	two_steps_index = list_start->next;

	while (two_steps_index != NULL)
	{
		two_steps_index = two_steps_index->next;
		if (two_steps_index != NULL)
		{
			one_step_index = one_step_index->next;
			two_steps_index = two_steps_index->next;
		}
	}

	*first_part = list_start;
	*second_part = one_step_index->next;
	one_step_index->next = NULL;
}

static imgPosCell* sortList(imgPosCell* first_part, imgPosCell* second_part)
{
	imgPosCell* res;
	res = NULL;

	if (first_part == NULL)
		return (second_part);
	else if (second_part == NULL)
		return(first_part);

	/*If first_part's x position comes before second_part's x position - opens a recursive call with the next cell in first_part.*/
	if ((first_part->position)[1] < (second_part->position)[1])
	{
		res = first_part;
		res->next = sortList(first_part->next, second_part);
	}
	/*If first_part's x position equals second_part's x position - checks the y values and opens a recursive call accordingly.*/
	else if ((first_part->position)[1] == (second_part->position)[1])
	{
		if ((first_part->position)[0] < (second_part->position)[0])
		{
			res = first_part;
			res->next = sortList(first_part->next, second_part);
		}
		else
		{
			res = second_part;
			res->next = sortList(first_part , second_part->next);
		}
	}
	/*If second_part's x position comes before first_part's x position - opens a recursive call with the next cell in second_part.*/
	else
	{
		res = second_part;
		res->next = sortList(first_part, second_part->next);
	}

	return res;
}

static void freeList(imgPosCell** list)
{
	if (!(*list))
		return;
	else
	{
		freeList(&((*list)->next));
		free(*list);
	}
}

/******************* Function Implementations ***********************/


imgPosCell* turnSegmentToImgPosCell(Segment* seg)
{
	imgPosCell* list_start = NULL;

	/*Recursively use an auxiliary function to insert the segment's parts into the list.*/
	list_start = turnSegmentToImgPosCellAux(list_start, seg->root);

	return list_start;
}

void mergeSort(imgPosCell** list_start)
{
	imgPosCell* head = *list_start;
	imgPosCell* first_part;
	imgPosCell* second_part;

	if ((head == NULL) || (head->next == NULL))
		return;

	splitList(head, &first_part, &second_part);

	mergeSort(&first_part);
	mergeSort(&second_part);

	*list_start = sortList(first_part, second_part);
}

int listLen(imgPosCell** list_start)
{
	imgPosCell* index = *list_start;
	int res;

	for (res = 0; index->next != NULL; res++)
		index = index->next;
	res++;
	return res;
}

void freeListOfLists(imgPosCell*** list, int list_size)
{
	int i;
	imgPosCell** temp;

	temp = *list;

	for (i = 0; i < list_size; i++)
	{
		if(temp[i])
			freeList(&(temp[i]));
	}
	free(temp);
}