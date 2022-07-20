#define _CRT_SECURE_NO_WARNINGS

/******************* Includes ***********************/
#include "Tree.h"
#include "IO.h"

/******************* Defines ***********************/
#define ALLOCATION_ERROR -1
#define allocation_error(NAME){									\
	fprintf(stderr, "Memory allocation of %s failed.\n", NAME);	\
	exit(ALLOCATION_ERROR);										\
}

#define REALLOCATION_ERROR -2
#define reallocation_error(NAME){									\
	fprintf(stderr, "Memory reallocation of %s failed.\n", NAME);	\
	exit(REALLOCATION_ERROR);										\
}


/******************* Static Function Prototypes ***********************/

/*Initializes a single segment.*/
static void allocateSegment(Segment* seg, imgPos root);

/*Finds a segment recursively.*/
static void findSegmentAux(int*** flags_array, grayImage* img, imgPos kernel, imgPos curr_kernel, imgPos prev_kernel, unsigned char threshold, Segment* seg);

/*Checks to see whether a pixel's value is in the given threshold or not. Returns TRUE if in threshold, FALSE if not.*/
static BOOL isInThreshold(grayImage* img, imgPos kernel, imgPos curr_kernel, unsigned char threshold);

/*Checks whether a pixel is in the image or not.*/
static BOOL isInImage(grayImage* img, imgPos pos_to_check);

/*Initializes a new tree node.*/
static treeNode* createNewNode(imgPos root);

/*Inserts curr_kernel into prev_kernel's similar_neighbors array. Assumes prev_kernel is already in the segment.*/
static void insertIntoSegment(Segment* seg, imgPos curr_kernel, imgPos prev_kernel);

/*Searches the given tree for a place to insert a new node.*/
static treeNode* findLocationInSegment(treeNode* root, imgPos kernel);


/******************* Static Function Implementations ***********************/


static void findSegmentAux(int*** flags_array, grayImage* img, imgPos kernel, imgPos curr_kernel, imgPos prev_kernel, unsigned char threshold, Segment* seg)
{
	int i, j;
	imgPos temp = { curr_kernel[0], curr_kernel[1] };

	if((*flags_array)[(curr_kernel[0])][(curr_kernel[1])] == 0)
		{
		insertIntoSegment(seg, temp, prev_kernel);
		(*flags_array)[(curr_kernel[0])][(curr_kernel[1])] = 2;		 
		}

	/*First, inserts the neighbors surrounding prev_kernel into seg accordingly.*/
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			temp[0] += i;
			temp[1] += j;
			if (isInImage(img, temp) == TRUE && isInThreshold(img, kernel, temp, threshold) == TRUE && (*flags_array)[temp[0]][temp[1]] == 0)
			{
				insertIntoSegment(seg, temp, curr_kernel);
				(*flags_array)[(temp[0])][(temp[1])] = 1;
			}
			temp[0] -= i;
			temp[1] -= j;    
		}

	}

	/*Sets prev_kernel to curr_kernel and begins the recursive process.*/
	prev_kernel[0] = curr_kernel[0];
	prev_kernel[1] = curr_kernel[1];

	/*Opens a recursive call for every neighbor of prev_kernel (the original curr_kernel).*/
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			curr_kernel[0] += i;
			curr_kernel[1] += j;
			if (isInImage(img, curr_kernel) == TRUE && isInThreshold(img, kernel, curr_kernel, threshold) == TRUE && (*flags_array)[curr_kernel[0]][curr_kernel[1]] == 1)
			{
				(*flags_array)[curr_kernel[0]][curr_kernel[1]] = 2;
				findSegmentAux(flags_array, img, kernel, curr_kernel, prev_kernel, threshold, seg);
			}
			curr_kernel[0] -= i;
			curr_kernel[1] -= j;
		}

	}
	
}


static void insertIntoSegment(Segment* seg, imgPos curr_kernel, imgPos prev_kernel)
{
	int i;
	treeNode* node;

	if (curr_kernel[0] == prev_kernel[0] && curr_kernel[1] == prev_kernel[1])
	{
		seg->root = createNewNode(prev_kernel);
	}
	else
	{
		node = findLocationInSegment(seg->root, prev_kernel);
		for (i = 0; (node->similar_neighbors)[i] != NULL; i++);
		i++;

		node->similar_neighbors = (treeNode**)realloc((node->similar_neighbors), (i+1)*sizeof(treeNode*));	
		if (!(node->similar_neighbors))
			reallocation_error("node->similar_neighbors, in insertIntoSegment");

		(node->similar_neighbors)[i - 1] = createNewNode(curr_kernel);
		(node->similar_neighbors)[i] = NULL;
		seg->size += 1;

	}
}

static treeNode* findLocationInSegment(treeNode* root, imgPos kernel) 
{
	int i; 

	if(root->position[0]==kernel[0] && root->position[1]==kernel[1])
	{
		return root;
	}
	else
	{
		for (i = 0; i < MAX_NEIGHBORS && root->similar_neighbors[i] != NULL; i++)	
			return findLocationInSegment(root->similar_neighbors[i], kernel);
	}
}

static BOOL isInThreshold(grayImage* img, imgPos kernel, imgPos curr_kernel, unsigned char threshold)
{
	if (img->pixels[curr_kernel[0]][curr_kernel[1]] <= (img->pixels[kernel[0]][kernel[1]] + threshold) && img->pixels[curr_kernel[0]][curr_kernel[1]] >= (img->pixels[kernel[0]][kernel[1]] - threshold))
		return TRUE;
	else return FALSE;
}

static BOOL isInImage(grayImage* img, imgPos pos_to_check)
{
	if (pos_to_check[0] < 0 || pos_to_check[1] < 0)
		return FALSE;
	else if (pos_to_check[0] >= img->rows || pos_to_check[0] >= img->cols || pos_to_check[1] >= img->rows || pos_to_check[1] >= img->cols)
		return FALSE;
	else
		return TRUE;

}


static void allocateSegment(Segment* seg, imgPos root) 
{
	seg->size = 1;
	seg->root = createNewNode(root);
}


static treeNode* createNewNode (imgPos root)
{
	treeNode* node = (treeNode*)malloc(sizeof(treeNode));
	if (!node)
		allocation_error("node, in createNewNode");
	node->position[0] = root[0];
	node->position[1] = root[1];

	node->similar_neighbors = (treeNode**)calloc( MIN_NEIGHBORS , sizeof(treeNode*));
	return node;
}



/******************* Function Implementations ***********************/

Segment findSingleSegment(grayImage* img, imgPos kernel, unsigned char threshold)
{
	int i;
	Segment seg;
	int** flags_array;
	flags_array= (int**)calloc((img->rows),sizeof(int*));
	if (!flags_array)
		allocation_error("flags_array, in findSingleSegment");
	for (i = 0; i < img->rows; i++)
	{
		flags_array[i] = (int*)calloc(img->cols, sizeof(int));
		if (!(flags_array[i]))
			allocation_error("flags_array[i], in findSinlgeSegment");
	}
	allocateSegment(&seg, kernel);

	imgPos k1 = { 0,0 };
	memcpy(&k1, kernel, sizeof(imgPos));
	imgPos k2 = { 0,0 };
	memcpy(&k2, kernel, sizeof(imgPos));
	imgPos k3 = { 0,0 };
	memcpy(&k3, kernel, sizeof(imgPos));

	findSegmentAux(&flags_array, img, k1, k2, k3, threshold, &seg);
	return seg;
}


Segment f_findSingleSegment(grayImage* img, int*** flags_array, imgPos kernel, unsigned char threshold)
{
	int i;
	Segment seg;

	allocateSegment(&seg, kernel);

	imgPos k1 = { 0,0 };
	memcpy(&k1, kernel, sizeof(imgPos));
	imgPos k2 = { 0,0 };
	memcpy(&k2, kernel, sizeof(imgPos));
	imgPos k3 = { 0,0 };
	memcpy(&k3, kernel, sizeof(imgPos));

	findSegmentAux(flags_array, img, k1, k2, k3, threshold, &seg);
	return seg;
}

