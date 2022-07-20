#define _CRT_SECURE_NO_WARNINGS

/******************* Includes ***********************/
#include "ImgPos.h"
#include "IO.h"
#include "Linked_List.h"
#include "Tree.h"

/******************* Defines ***********************/

#define ALLOCATION_ERROR -1
#define allocation_error(NAME, FUNC_NAME){										\
	fprintf(stderr, "Memory allocation of %s failed at %s.", NAME, FUNC_NAME);	\
	exit(ALLOCATION_ERROR);														\
}
#define REALLOCATION_ERROR -2
#define reallocation_error(NAME, FUNC_NAME){										\
	fprintf(stderr, "Memory reallocation of %s failed at %s.", NAME, FUNC_NAME);	\
	exit(REALLOCATION_ERROR);														\
}

/******************* Typedefs ***********************/



/******************* Static Function Prototypes ***********************/

/*Returns an imgPos with the location of the pixel with the lowest value in the image.
  Ignores pixels which belong to an existing segment.*/
static imgPos* findKernel(grayImage* img, int*** flags_array);

/*Reallocates a list of linked lists.*/
static void listOfListsRealloc(imgPosCell*** list_of_lists, int desired_size);

/*Reallocates an array of integers.*/
static void intArrayRealloc(int** int_array, int desired_size);

/*Returns the gray value of a given segment according to the formula in question 3.*/
static unsigned char findSegmentGrayValue(int num_of_segments, int segment_index);

/*Sorts a list of linked lists using the bubble sort algorithm.*/
static void bubbleSort(imgPosCell*** list_of_lists, int** sizes_array, int size);

/*Swaps between two imgPosCell pointers.*/
static void swapImgPosCell(imgPosCell** ptr1, imgPosCell** ptr2);

/*Swaps 2 int arguments*/
static void swapInt(int* size1, int* size2);

/*Sorts two lists of linked lists using the merge sort algorithm.*/
static void mergeSortList(imgPosCell*** res, imgPosCell*** list1, int size1, imgPosCell*** list2, int size2);


/******************* Static Function Implementations ***********************/

static imgPos* findKernel(grayImage* img, int*** flags_array)
{
	int i, j, check;
	BOOL found = FALSE;
	imgPos* kernel = (imgPos*)calloc(1, sizeof(imgPos));
	
	/*If we've already passed through (0,0) in a former call, assigns a new kernel to start the search with.*/
	for (i = 0; i < img->rows; i++) 
	{
		for (j = 0; j < img->cols; j++) 
		{
			if ((*flags_array)[kernel[0][0]][kernel[0][1]] == 0)
			{
				found = TRUE;
				break;
			}
			else
			{
				kernel[0][0] = i;
				kernel[0][1] = j;
			}
		}
		if (found == TRUE)
			break;
	}

	i = j = check = 0;
	for (; i < img->rows; i++)
	{
		for (j = 0; j < img->cols; j++)
		{
			/*If the value we're standing on is lower than the value in kernel's position, stores the current position in kernel.*/
			if ( ((*flags_array)[i][j] == 0) && ((int)((img->pixels)[i][j]) <= (int)((img->pixels)[kernel[0][0]][kernel[0][1]])) )
			{
				check++;
				kernel[0][0] = i;
				kernel[0][1] = j;
			}
		}
	}

	/*Returns NULL if the image is empty or if no kernel was found.*/
	if ((i == 0 && j == 0) || check == 0)
		return NULL;
	
	/*Otherwise - returns kernel.*/
	return kernel;
}

static void listOfListsRealloc(imgPosCell*** list_of_lists, int desired_size)
{
	*list_of_lists = (imgPosCell**)realloc(*list_of_lists, sizeof(imgPosCell*) * desired_size);
	if (!(*list_of_lists))
		reallocation_error("list_of_lists", "listOfListsRealloc");
}

static void intArrayRealloc(int** int_array, int desired_size)
{
	*int_array = (int*)realloc(*int_array, desired_size * sizeof(int));
	if (!(*int_array))
		reallocation_error("*int_array", "intArrayRealloc");
}

static unsigned char findSegmentGrayValue(int num_of_segments, int segment_index)
{
	unsigned char res;

	res = (segment_index * MAX_VALUE) / (num_of_segments - 1);

	return res;
}

static void bubbleSort(imgPosCell*** list_of_lists, int** sizes_array, int size)
{
	int i, j;
	for (i = 0; i < size-1; i++) {
		for (j = 0; j <size-i-1; j++)
		{
			if ((*sizes_array)[j] < (*sizes_array)[j+1])
			{
				swapImgPosCell(&(*list_of_lists)[j] ,&(*list_of_lists)[j+1] );
				swapInt(&(*sizes_array)[j], &(*sizes_array)[j + 1]);
			}
		}
	}
}

static void swapImgPosCell(imgPosCell** ptr1, imgPosCell** ptr2)
{
	imgPosCell* temp;
	temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
}

static void swapInt(int* size1, int* size2)
{
	int res = *size1;
	*size1 = *size2;
	*size2 = res;
}


/******************* Function Implementations ***********************/

unsigned int findAllSegments(grayImage* img, unsigned char threshold, imgPosCell*** segments)
{
	int i, j, lol_size = 1, array_size = 1;
	int** flags_array;
	int* sizes_array;
	Segment seg;
	imgPos* kernel;
	imgPosCell* list;

	listOfListsRealloc(segments, lol_size);
	
	sizes_array = (int*)malloc(sizeof(int));
	if (!sizes_array)
		allocation_error("sizes_array", "findAllSegments");
	
	flags_array = (int**)malloc((img->rows)*sizeof(int*));
	if (!flags_array)
		allocation_error("flags_array","findSingleSegment");
	/*Allocates and resets flags_array.*/
	for (i = 0; i < img->rows; i++)
	{
		flags_array[i] = (int*)calloc(img->cols, sizeof(int));
		if (!(flags_array[i]))
			allocation_error("flags_array[i]", "findSinlgeSegment");
	}

	for (i = 0; (kernel = findKernel(img, &flags_array)) != NULL; i++)
	{

		if (i >= array_size)
		{
			array_size *= 2;
			intArrayRealloc(&sizes_array, array_size);
		}
		if (i >= lol_size)
		{
			lol_size *= 2;
			listOfListsRealloc(segments, lol_size);
		}

		seg = f_findSingleSegment(img, &flags_array, kernel[0], threshold);
		sizes_array[i] = seg.size;
		/*Turns seg into an imgPosCell list, sorts it, then inserts the sorted list into the segments array.*/
		list = turnSegmentToImgPosCell(&seg);
		mergeSort(&list);
		(*segments)[i] = list;
	}
	intArrayRealloc(&sizes_array, i);
	listOfListsRealloc(segments, i);
	/*No need for a NULL cell at the end, for we return the list's size.*/
	for (j = 0; j < (img->rows); j++)
		free(*(flags_array + j));
	free(flags_array);
	bubbleSort(segments, &sizes_array, i);

	return i;
}

void allocateGrayImage(grayImage* img, unsigned short rows, unsigned short cols)
{
	int i;

	img->pixels = (unsigned char**)malloc(rows * sizeof(unsigned char*));
	if (!(img->pixels))
		allocation_error("img->pixels", "allocateGrayImage");

	for (i = 0; i < rows; i++)
	{
		(img->pixels)[i] = (unsigned char*)malloc(cols * sizeof(unsigned char));
		if (!(img->pixels)[i])
			allocation_error("(img->pixels)[i]", "allocateGrayImage");
	}

	img->cols = cols;
	img->rows = rows;
}

static void mergeSortList(imgPosCell*** res, imgPosCell*** list1, int size1, imgPosCell*** list2, int size2)
{
	int i = 0, j = 0;
	while ((i < size1) && (j < size2))
	{
		if (listLen(list1[i]) > listLen(list2[j]))
		{
			**(res + i + j) = **(list1 + i);
			i++;
		}
		else
		{
			**(res + i + j) = **(list2 + j);
			j++;
		}
	}
	while (i < size1)
	{
		**(res + i + j) = **(list1 + i);
		i++;
	}
	while (j < size2)
	{
		**(res + i + j) = **(list2 + j);
		j++;
	}
}

grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size)
{
	int i, index;
	grayImage* NewImg = (grayImage*)malloc(sizeof(grayImage));
	allocateGrayImage(NewImg, img->rows, img->cols);
	imgPosCell* temp;
	

	for (i = 0; i < size; i++)
	{
		temp = segments[i];
		index = 0;
		while (temp != NULL)
		{
			(NewImg->pixels)[temp->position[0]][temp->position[1]] = findSegmentGrayValue(size, i);
			temp = temp->next;
			index++;
		}
	}
	
	return NewImg;
}

void freeGrayImage(grayImage* img)
{
	int i, j;

	for (i=0;i<img->cols;i++)
	{
		free((img->pixels)[i]);
	}
	free(img->pixels);
}