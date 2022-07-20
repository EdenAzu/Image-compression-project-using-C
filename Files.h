#pragma once
#ifndef _FILES_
#define _FILES_


/******************* Includes ***********************/
#include <stdio.h>
#include <stdlib.h>
#include "ImgPos.h"
#include "IO.h"
#include "Linked_List.h"
#include "Tree.h"

/******************* Defines ***********************/
#define BITS_TO_READ 5
#define INCORRECT_ENDING -5
#define MAX_CHARS 70

/******************* Typedefs ***********************/


/******************* Function Prototypes ***********************/

/*Stores a grayImage's values in a binary file. 
  The first value represents the number of rows in the image, and the second represents the number of columns.
  Returns a pointer to the file.*/
FILE* storeImageInFile(char* desired_file_name, grayImage* img);

/*Reads values from a binary file and inserts them into the given grayImage.*/
void getImageFromFile(char* file_name, grayImage* img);

/*Saves a grey image's values in a compressed binary file, according to the formula.*/
void saveCompressed(char* file_name, grayImage* image, unsigned char reduced_grey_levels);

/*Checks a file's ending. Terminates the program if the file's ending is wrong.*/
void checkFileEnding(char* file_name, char* correct_ending);

/*Extracts a grey image's values from a compressed binary file. Inserts the decompressed values into a new PGM file.*/
void convertCompressedImageToPGM(char* compressed_file_name, char* pgm_file_name);

/*Reads values from a PGM file. Returns a pointer to the image.*/
grayImage* readPGM(char* fname);

/*Turns a gray image into a PGM file.*/
void imageToPGM(char* fname, grayImage* image);

#endif 