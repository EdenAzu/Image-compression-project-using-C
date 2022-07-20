#define _CRT_SECURE_NO_WARNINGS

/**************** Include *******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ImgPos.h"
#include "Linked_List.h"
#include "Tree.h"
#include "IO.h"
#include "Files.h"

/***************** Defines / main definitions ********************/

#define ERROR_OPENING_FILE -3
#define error_opening_file(FILE_NAME, FUNC){					\
	fprintf(stderr, "Failed to open %s in %s.", FILE_NAME, FUNC);	\
	exit(ERROR_OPENING_FILE);							\
}

/*Receives values straight from the keyboard.*/
void main1();

/*Receives a file's name and gets the values from there.*/
void main2();

/*Receives a file's name of PGM file, and gets the image from there.*/
void main3();

/*This main checks questions 5 and 6.*/
void main56();

void main7();

/****************** Main *********************/

void main()
{
	int num, i;
	/*Description for main1.*/
	char* description11 = "The function main1 (type either 1 or 2 to run) tests questions 1 and 2.\nFirst, the function will receive a grey image's values from the keyboard.\n";
	char* description12 = "Once the function had receieved all the data, it will proceed to print the grey image.\n";
	char* description13 = "Second, the function will receive the coordinates and the threshold for a kernel.\n";
	char* description14 = "Third, The function will build a segment, and immediately proceed to printf its values.\nThen, the function will turn the segment into a linked list.\n";
	char* description15 = "The function will proceed to sort and re-print the list.\nFourth, the function will find all the segments in the given image,\nconvert them into a large ";
	char* description16 = "list of (linked) lists, and print the list: first by coordinates, then by value.\n";

	/*Description for main2.*/
	char* description21 = "The function main2 (type 3 to run) tests question 3.\nIt receives an image from a binary file \n(according to a format explained in the function storeImageInFile(), in files.c), ";
	char* description22 = "then proceeds to print the image.\nThe function will the find all the segments in the image, print them (by coordinates, then by value), \n";
	char* description23 = "Then colour them all according to the format given in the project's description sheets.\n";
	char* description24 = "The function will then print the values in the new image, with the coloured segments.\n";

	/*Description for main3.*/
	char* description31 = "The function main3 (type 4 to run) tests question 4.\nIt will receive a PGM file's name, then extract a grey image's values from it.\nThe function will then ";
	char* description32 = "insert the values into a new grey image and print them to the screen.\n";

	/*Description for main56.*/
	char* description41 = "The function main56 (type either 5 or 6 to run) tests questions 5 and 6.\nThe function will extract a grey image from a file, then print it to the screen.\n";
	char* description42 = "The function will then proceed to compress the image into a PGM file,\nAccording to the format described in the project's description sheets.\n";
	char* description43 = "The function will then extract the image from the file, and insert its values into a new grey image.\n";
	char* description44 = "The function will then print the image to the screen.\n";

	/*Description for main7.*/
	char* description51 = "The function main7 (type 7 to run) tests all the aforementioned questions.\nThe function receieves the names of four files: \n";
	char* description52 = "complete PGM file (with values), 2 empty PGM files (for writing), and 1 binary file (for writing).\n";
	char* description53 = "The function reads the values from the full PGM file into a gray image, then proceeds to perform the following steps:\n";
	char* description54 = "A. The function will colour all the segments in the image, and insert them into the second PGM file.\n";
	char* description55 = "B. The function will compress the values of the original gray image into the given binary file.\n";
	char* description56 = "C. The function will extract the values from the binary file into the third PGM file.\n";


	/*Prints the description for main1.*/
	printf("%s%s%s%s%s%s\n", description11,description12,description13,description14, description15,description16); 

	/*Prints the description for main2.*/
	printf("%s%s%s%s\n", description21,description22,description23,description24); 

	/*Prints the description for main3.*/
	printf("%s%s\n", description31, description32); 

	/*Prints the description for main56.*/
	printf("%s%s%s%s\n", description41,description42,description43,description44); 

	/*Prints the description for main7.*/
	printf("%s%s%s%s%s%s\n", description51,description52,description53,description54,description55,description56); 

	printf("Enter the number of the main you wish to run: ");
	scanf("%d", &num);

	while (num != -1)
	{
		switch (num)
		{
		case 1:
			printf("\n\n****************** main1 ******************\n\n");
			main1();
			break;
		case 2:
			printf("\n\n****************** main1 ******************\n\n");
			main1();
			break;
		case 3:
			printf("\n\n****************** main2 ******************\n\n");
			main2();
			break;
		case 4:
			printf("\n\n****************** main3 ******************\n\n");
			main3();
			break;
		case 5:
			printf("\n\n****************** main56 ******************\n\n");
			main56();
			break;
		case 6:
			printf("\n\n****************** main56 ******************\n\n");
			main56();
			break;
		case 7:
			printf("\n\n****************** main7 ******************\n\n");
			main7();
			break;
		default:
			exit(0);
			break;
		}
	printf("\nDo you wish to run another main? If so, enter its number. If not, type -1: ");
	scanf("%d", &num);
	}
}


void main1() {
	int rows, cols, i, length, x, y;
	grayImage image;
	imgPos* kernel = (imgPos*)calloc(1, sizeof(imgPos));
	Segment seg;
	unsigned char threshold;
	imgPosCell* list;
	imgPosCell** list_o_lists;

	/*Possible test values to insert: 30 101 100 103 51 97 87 100 105 110 42 85 95 40 26 80 100 94 50 23 52 70 60 55 60*/
	image = getGrayImage(&rows, &cols);
	putchar('\n');
	printf("Image: \n");
	printGrayImage(&image);
	putchar('\n');

	printf("Enter the x and y values of the kernel: ");
	scanf("%d%d", &x, &y);

	printf("\nEnter the threshold: ");
	scanf("%hhu", &threshold);

	/*An imgPos* is an array that contains arrays of 2 shorts: an imgPos variable.
	 Hence when we wish to access the first kernel's value, we must first access the [0] row, then the [0] and the [1] cells in that row.
	 The same logic applies when we wish to access other kernels in the array.
	 You can have as many rows as you like, but only up to 2 ([0] and [1]) columns, for such is the length of imgPos.*/

	kernel[0][0] = x;
	kernel[0][1] = y;

	/*Imagine it this way: you pass kernel[0] (the row) to the function, and the function adds a [0] or a [1] to it to access the x or y position.
	  So, when the function decides that kernel[0] = 1, replace "kernel" with "kernel[0]" and you'll get kernel[0][0] = 1, as written above.*/
	seg = findSingleSegment(&image, kernel[0], threshold);

	printf("\nSegment: \n");
	printSegment(&seg, &image);

	list = turnSegmentToImgPosCell(&seg);

	printf("\nList: \n");
	printList(list);
	putchar('\n');

	mergeSort(&list);
	printf("\nSorted list: \n");
	printList(list);

	list_o_lists = (imgPosCell**)malloc(sizeof(imgPosCell*));

	/*Note how we call list_o_lists. list_o_lists is an array of pointers, each pointing to the beginning of an imgPosCell list.
	  The array simply contains pointers, which is why we (unfortunately) cannot store anything else but pointers within its cells.
	  The aforementioned reason is why we must resort to using listLen(); there is no place to store a single list's size within list_o_lists.*/

	list_o_lists[0] = list;
	printf("\n\nlist_o_lists[0]: \n");
	printList(list_o_lists[0]);
	putchar('\n');
	
	length = findAllSegments(&image, threshold, &list_o_lists);
	printf("\nSegments in list_o_lists:\n");
	for (i = 0; i< length; i++) 
	{
		printList(list_o_lists[i]);
		putchar('\n');
	}
	printf("\nValues of segments in list_o_lists:\n");
	for (i = 0; i< length; i++) 
	{
		printListValues(&image, list_o_lists[i]);
		putchar('\n');
	}

	freeListOfLists(&list_o_lists, length);

	/*Note the differences between all these forms.*/
	/*list_o_lists[0] = list;      
	list_o_lists[1] = list;      
	(list_o_lists)[0]->next = list; 
	(list_o_lists)[1]->next = list; */
	

	/***************** The original way. Note the difference above when kernel is imgPos*. ******************/
	/*int rows, cols, i;
	grayImage image;
	imgPos kernel;
	Segment seg;
	unsigned char threshold = 10;*/

	//image = getGrayImage(&rows, &cols);
	//printGrayImage(&image);
	//putchar('\n');
	//seg = findSingleSegment(&image, kernel, threshold);

	//printSegment(&seg, &image);

	//1 3 2 4 6 5 7 9 5 3 2 4 9 6 5 7 2 3 4 5 6 1 5 5 8
	
}



void main2()
{
	
	FILE* file;
	imgPosCell** list_o_lists;
	grayImage img;
	int length, threshold, i;
	grayImage* newImg;
	char* file_name = "test_1.bin";

	/*This has to be used once in order to initialize a test file (with a different name, of course).*/
	//img = getGrayImage(&rows, &cols);
	//file = storeImageInFile(file_name, &img);

	getImageFromFile(file_name, &img);

	printf("\nImage: \n");
	printGrayImage(&img);

	threshold = 10;
	list_o_lists = (imgPosCell**)malloc(sizeof(imgPosCell*));
	length = findAllSegments(&img, threshold, &list_o_lists);
	
	printf("\nSegments in list_o_lists:\n");
	for (i = 0; i < length; i++)
	{
		printList(list_o_lists[i]);
		putchar('\n');
	}
	printf("\nValues of segments in list_o_lists:\n");
	for (i = 0; i < length; i++)
	{
		printListValues(&img, list_o_lists[i]);
		putchar('\n');
	}

	newImg = colorSegments(&img, list_o_lists, length);
	printf("\nColoured image:\n");
	printGrayImage(newImg);
}



void main3()
{
	int rows, cols, seg_size;
	char* fname = "pgm_test_1.pgm";
	grayImage* img;
	
	img = readPGM(fname); 
	printf("Image from PGM:\n");
	printGrayImage(img);

}



void main56()
{
	FILE* file;
	FILE* pgm_file;
	char* fname = "comp_img1.bin";
	char* img_file_name = "test_1.bin";
	char* pgm_file_name = "pgm_test_2.pgm";
	grayImage img;
	grayImage* img_2;

	pgm_file = fopen(pgm_file_name, "w");
	if (!pgm_file)
		error_opening_file("pgm_file", "main56");

	getImageFromFile(img_file_name, &img);

	printf("\nOriginal image:\n");
	printGrayImage(&img);

	//saveCompressed(fname, &img, 32);		/*Already done that, so the file contains compressed pixels.*/

	convertCompressedImageToPGM(fname, pgm_file_name);

	img_2 = readPGM(pgm_file_name);

	printf("\nExtracted image:\n");
	printGrayImage(img_2);

	freeGrayImage(img_2);
	fclose(pgm_file);
}



void main7()
{
	grayImage* img;
	grayImage* img2;
	int list_size;
	unsigned char threshold, reduced_levels;
	char* full_pgm_fname;
	char* new_pgm_fname;
	char* new_pgm_fname2;
	char* bin_fname;
	
	imgPosCell** list = (imgPosCell**)malloc(sizeof(imgPosCell*));

	printf("Enter the PGM file's name (the file containing the image): ");
	getStr(&full_pgm_fname);
	putchar('\n');

	printf("Enter the first new PGM file's name (the file to write the first image into): ");
	getStr(&new_pgm_fname);
	putchar('\n');

	printf("Enter the second new PGM file's name (the file to write the second image into): ");
	getStr(&new_pgm_fname2);
	putchar('\n');

	printf("Enter the binary file's name: ");
	getStr(&bin_fname);
	putchar('\n');

	img = readPGM(full_pgm_fname);

	printf("Enter the threshold: ");
	scanf("%hhu", &threshold);
	putchar('\n');
	
	/*Colours the segments of img (saves the new values to img2), then inserts the values into the first PGM file.*/
	list_size = findAllSegments(img, threshold, &list);
	img2 = colorSegments(img, list, list_size);
	imageToPGM(new_pgm_fname, img2);

	/*Compresses the values in img to the binary file's name, the decompresses and inserts to the second PGM file.*/
	printf("Enter the reduced grey value: ");
	scanf("%hhu", &reduced_levels);
	putchar('\n');
	saveCompressed(bin_fname, img, reduced_levels);
	convertCompressedImageToPGM(bin_fname, new_pgm_fname2);

	freeListOfLists(&list, list_size);
	freeGrayImage(img);
	freeGrayImage(img2);
}