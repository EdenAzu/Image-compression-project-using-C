#pragma once
#define _CRT_SECURE_NO_WARNINGS

/******************* Includes ***********************/
#include "Files.h"
#include "Linked_List.h"
#include "IO.h"
#include "ImgPos.h"
#include "Tree.h"

/******************* Defines ***********************/
#define ERROR_OPENING_FILE -3
#define error_opening_file(FILE_NAME, FUNC){						\
	fprintf(stderr, "Failed to open %s in %s.", FILE_NAME, FUNC);	\
	exit(ERROR_OPENING_FILE);										\
}

#define ALLOCATION_ERROR -1
#define allocation_error(NAME, FUNC_NAME){										\
	fprintf(stderr, "Memory allocation of %s failed at %s.", NAME, FUNC_NAME);	\
	exit(ALLOCATION_ERROR);														\
}

#define REDUCE_GREY_LEVEL(PIXEL, NEWMAXVAL) (((PIXEL)*(NEWMAXVAL-1))/(MAX_VALUE))

#define RECOVER_GREY_LEVEL(PIXEL, NEWMAXVAL) (((MAX_VALUE)*(PIXEL))/((NEWMAXVAL)-1))


/******************* Typedefs ***********************/



/******************* Static Function Prototypes ***********************/

/*** Receive several pixels and convert them according to the format - 5 bits per pixel. ***/
static unsigned char form_53(unsigned char pixel1, unsigned char pixel2, unsigned char new_max_value);
static unsigned char form_251(unsigned char pixel1, unsigned char pixel2, unsigned char pixel3, unsigned char new_max_value);
static unsigned char form_44(unsigned char pixel1, unsigned char pixel2, unsigned char new_max_value);
static unsigned char form_152(unsigned char pixel1, unsigned char pixel2, unsigned char pixel3, unsigned char new_max_value);
static unsigned char form_35(unsigned char pixel1, unsigned char pixel2, unsigned char new_max_value);

/*Sets the pixels in pixel[] (given by reference) using i, j, and image->pixels. 
  Used to replace one repeating 'for' loop in storeImageInFile() for improved readability.*/
static void setPixels(grayImage* image, unsigned char** pixel, int max_pixels, int* i, int* j);

/*** Receive a compressed pixel and (as needed) a part of a previous pixel (all by reference). Returns (by reference, too) the uncompressed pixels. 
     Note that usually one pixel will be returned fragmented, to be used as a part of a whole pixel in the next rec_form function. ***/
static void rec_form_53(unsigned char* curr_pixel, unsigned char* next_pixel, unsigned char new_max_value);
static void rec_form_251(unsigned char* prev_pixel, unsigned char* curr_pixel, unsigned char* next_pixel, unsigned char new_max_value);
static void rec_form_44(unsigned char* prev_pixel, unsigned char* curr_pixel, unsigned char new_max_value);
static void rec_form_152(unsigned char* prev_pixel, unsigned char* curr_pixel, unsigned char* next_pixel, unsigned char new_max_value);
static void rec_form_35(unsigned char* prev_pixel, unsigned char* curr_pixel, unsigned char new_max_value);

/*Inserts a character into a PGM file.*/
static void insertCharToPGM(char* fname, int* char_count, unsigned char char_to_insert);

/******************* Static Function Implementations ***********************/

static unsigned char form_53(unsigned char pixel1, unsigned char pixel2, unsigned char new_max_value)
{
	unsigned char res = 0x00;
	
	pixel1 = REDUCE_GREY_LEVEL((pixel1), (new_max_value));
	pixel2 = REDUCE_GREY_LEVEL((pixel2), (new_max_value));

	pixel1 <<= 3;
	pixel2 >>= 2;
	res |= (pixel1 | pixel2);

	return res;
}

static unsigned char form_251(unsigned char pixel1, unsigned char pixel2, unsigned char pixel3, unsigned char new_max_value)
{
	unsigned char res = 0x00;
	
	pixel1 = REDUCE_GREY_LEVEL((pixel1), (new_max_value));
	pixel2 = REDUCE_GREY_LEVEL((pixel2), (new_max_value));
	pixel3 = REDUCE_GREY_LEVEL((pixel3), (new_max_value));

	pixel1 <<= 6;	
	pixel2 <<= 1;
	pixel3 >>= 4;
	res |= (pixel1 | pixel2 | pixel3);
	
	return res;
}

static unsigned char form_44(unsigned char pixel1, unsigned char pixel2, unsigned char new_max_value)
{
	unsigned char res = 0x00;

	pixel1 = REDUCE_GREY_LEVEL((pixel1), (new_max_value));
	pixel2 = REDUCE_GREY_LEVEL((pixel2), (new_max_value));

	pixel1 <<= 4;
	pixel2 >>= 1;
	res |= (pixel1 | pixel2);

	return res;
}

static unsigned char form_152(unsigned char pixel1, unsigned char pixel2, unsigned char pixel3, unsigned char new_max_value)
{
	unsigned char res = 0x00;

	pixel1 = REDUCE_GREY_LEVEL((pixel1), (new_max_value));
	pixel2 = REDUCE_GREY_LEVEL((pixel2), (new_max_value));
	pixel3 = REDUCE_GREY_LEVEL((pixel3), (new_max_value));

	pixel1 <<= 7;
	pixel2 <<= 2;
	pixel3 >>= 3;
	res |= (pixel1 | pixel2 | pixel3);
	
	return res;
}

static unsigned char form_35(unsigned char pixel1, unsigned char pixel2, unsigned char new_max_value)
{
	unsigned char res = 0x00;

	pixel1 = REDUCE_GREY_LEVEL((pixel1), (new_max_value));
	pixel2 = REDUCE_GREY_LEVEL((pixel2), (new_max_value));

	pixel1 <<= 5;
	res |= (pixel1 | pixel2);
	
	return res;
}

static void setPixels(grayImage* image, unsigned char** pixel, int max_pixels, int* i, int* j)
{
	int pixel_count;

	for (pixel_count = 0; pixel_count < max_pixels && (*i) < image->rows; pixel_count++, (*j)++)
	{
		if (*j == image->cols)
		{
			(*i)++;
			if ((*i) >= image->rows)
				break;
			*j = 0;
		}

		(*pixel)[pixel_count] = (image->pixels)[(*i)][(*j)];
	}

	if (*j != 0)
		(*j)--;
	else
	{
		(*i)--;
		*j = image->cols - 1;
	}
}

static void rec_form_53(unsigned char* curr_pixel, unsigned char* next_pixel, unsigned char new_max_value)
{
	unsigned char res;
	
	/*To get rid of the 5 bits we've 'taken' into res. Basically does the opposite of the >>2 we did in form_53.*/
	*next_pixel = (*curr_pixel & 0x07) << 2;

	res = *curr_pixel >> 3;
	*curr_pixel = RECOVER_GREY_LEVEL((res), (new_max_value));

}

static void rec_form_251(unsigned char* prev_pixel, unsigned char * curr_pixel,  unsigned char* next_pixel, unsigned char new_max_value)
{
	unsigned char res;
	/*Combines the 3 bits from form53 and the 2 from curr_pixel to recover the pixel.*/
	*prev_pixel |= (*curr_pixel >> 6);
	*prev_pixel = RECOVER_GREY_LEVEL((*prev_pixel), (new_max_value));
	
	/*Removes the irrelevant bits to get a full (5) pixel..*/
	res = ((*curr_pixel << 2) >> 3);
	res = RECOVER_GREY_LEVEL((res), (new_max_value));

	/*Saves the last bit of curr_pixel.*/
	*next_pixel = (0x01 & *curr_pixel) << 4;
	 
	*curr_pixel = res;
}

static void rec_form_44(unsigned char* prev_pixel, unsigned char* curr_pixel, unsigned char new_max_value)
{
	unsigned char res;
	
	res = (*prev_pixel | (*curr_pixel >> 4));
	*prev_pixel = RECOVER_GREY_LEVEL((res), (new_max_value));

	*curr_pixel = (0x0F & (*curr_pixel)) << 1;
	
}

static void rec_form_152(unsigned char* prev_pixel, unsigned char* curr_pixel, unsigned char* next_pixel, unsigned char new_max_value)
{
	unsigned char res;
 
	res = *prev_pixel | ((0x80 & *curr_pixel)>>7);
	*prev_pixel = RECOVER_GREY_LEVEL((res), (new_max_value));
	 
	res = (*curr_pixel << 1) >> 3;
	res = RECOVER_GREY_LEVEL((res), (new_max_value));
	
	*next_pixel = (*curr_pixel & 0x03) << 3;

	*curr_pixel = res; 
}

static void rec_form_35(unsigned char* prev_pixel, unsigned char* curr_pixel, unsigned char new_max_value)
{
	unsigned char res;

	res = *prev_pixel | (*curr_pixel >> 5);
	*prev_pixel = RECOVER_GREY_LEVEL((res), (new_max_value));

	res = (*curr_pixel & 0x1F);
	*curr_pixel = RECOVER_GREY_LEVEL((res), (new_max_value));
}

static void insertCharToPGM(char* fname, int* char_count, unsigned char char_to_insert)
{
	
	if(*char_count == MAX_CHARS)
	{
		fprintf(fname, "\n");
		*char_count = 0;
		fprintf(fname, "%hhu ", char_to_insert);
		(*char_count) += 2;
	}
	else
	{
		fprintf(fname, "%hhu", char_to_insert);
		(*char_count)++;

		if (*char_count == MAX_CHARS)
		{
			putc('\n', fname);
			*char_count = 0;
		}
		else
		{
			putc(' ', fname);
			(*char_count)++;
		}
	}
}

/******************* Function Implementations ***********************/

FILE* storeImageInFile(char* desired_file_name, grayImage* img)
{
	int i, j;
	FILE* file_ptr;

	file_ptr = fopen(desired_file_name, "wb");
	if (!file_ptr)
		error_opening_file("file_ptr", "storeImageInFile");
	
	/*First, writes the image's numbers of rows and columns into the file.*/
	fwrite(&(img->rows), sizeof(unsigned short), 1, file_ptr);
	fwrite(&(img->cols), sizeof(unsigned short), 1, file_ptr);
	
	/*Writes the content of the image into the file.*/
	for (i = 0; i < img->rows; i++)
	{
		for (j = 0; j < img->cols; j++)
		{
			fwrite(&(img->pixels)[i][j], sizeof(unsigned char), 1, file_ptr);
		}
	}

	fclose(file_ptr);
	return file_ptr;
}

void getImageFromFile(char* file_name, grayImage* img)
{
	int i, j;
	FILE* file;

	file = fopen(file_name, "rb");
	if (!file)
		error_opening_file("file", "getImageFromFile");

	/*Reads the numbers of rows and colums from the file and inserts them into the grayImage struct.*/
	fread(&(img->rows), sizeof(unsigned short), 1, file);
	fread(&(img->cols), sizeof(unsigned short), 1, file);

	allocateGrayImage(img, img->rows, img->cols);

	/*Reads the gray values from the file and inserts them into the image.*/
	for (i = 0; i < img->rows; i++)
	{
		for (j = 0; j < img->cols; j++)
		{
			fread(&(img->pixels)[i][j], sizeof(unsigned char), 1, file);
		}
	}

	fclose(file);
}

grayImage* readPGM(char* fname)
{
	grayImage* imgPGM = (grayImage*)malloc(sizeof(grayImage));
	FILE* pgmFile;
	unsigned char fileType[3];
	unsigned char temp;
	unsigned char line[MAX_CHARS];
	int rows, cols;
	int pixel;

	int i, j;

	checkFileEnding(fname, ".pgm");

	/*Opens the file.*/
	pgmFile = fopen(fname, "r");
	if (!pgmFile)
		error_opening_file("PGMptr", "readPGM");

	/*Gets the file's magic number.*/
	fgets(fileType, 3 * sizeof(unsigned char), pgmFile);

	/*Gets the first \n character, then reads until there are no more comments.*/
	temp = fgetc(pgmFile);
	while ((temp = fgetc(pgmFile)) == '#')
	{
		while (temp != '\n')
		{
			temp = fgetc(pgmFile);

		}
	}
	/*Returns the last character read to the file.*/
	ungetc(temp, pgmFile);

	/*Gets the rows and cols.*/
	fgets(line, MAX_CHARS, pgmFile);
	sscanf(line, "%d %d", &rows, &cols);

	/*Skips the maximal value.*/
	fgets(line, MAX_CHARS, pgmFile);

	/*Allocates the image with the values from the file.*/
	allocateGrayImage(imgPGM, rows, cols);

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			fscanf(pgmFile, "%s", line);
			sscanf(line, "%d", &pixel);
			imgPGM->pixels[i][j] = pixel;
		}
	}

	fclose(pgmFile);
	return imgPGM;
}

void saveCompressed(char* file_name, grayImage* image, unsigned char reduced_grey_levels)
{
	FILE* res;
	unsigned char *pixel, comp_pixel;
	unsigned short rows = image->rows;
	unsigned short cols = image->cols;
	int i, j, count, pixel_count;

	checkFileEnding(file_name, ".bin");

	pixel = (unsigned char*)malloc(3 * sizeof(unsigned char));
	if (!pixel)
		allocation_error("pixel", "saveCompressed");

	
	res = fopen(file_name, "wb");
	if (!res)
		error_opening_file(file_name, "saveCompressed");

	/*Inserts the number of rows, the number of columns and the new grey level to the file.*/
	fwrite(&rows, sizeof(unsigned short), 1, res);
	fwrite(&cols, sizeof(unsigned short), 1, res);
	fwrite(&(reduced_grey_levels), sizeof(unsigned char), 1, res);


	/*Runs throughout the image and writes the compressed pixels into the file.*/
	for (count = 0, i = 0; i < image->rows; i++)
	{
		for (j = 0; j < image->cols; j++)
		{
			/*In case one pixel cannot be read (end of image).*/
			pixel[0] = pixel[1] = pixel[2] = 0x00;

			/*Since the compression pattern changes every 5 runs (8 bit-wise), compresses the pixels according to the pattern and the format,
			  So that each pixel will be compressed into 5 bits.*/
			switch ((count % 5))
			{

			case 0:
				/*Solves the case when the image contains only one pixel.*/
				if (image->rows == 1 && image->cols == 1)
				{
					comp_pixel = REDUCE_GREY_LEVEL(((image->pixels)[i][j]), (reduced_grey_levels));
					comp_pixel <<= 3;
					break;
				}

				setPixels(image, &pixel, 2, &i, &j);
				comp_pixel = form_53(pixel[0], pixel[1], reduced_grey_levels);
				fwrite(&comp_pixel, sizeof(unsigned char), 1, res);
				break;

			case 1:
				if(j!=0)
					j--;
				else
				{
					i--;
					j = image->cols - 1;
				}
				setPixels(image, &pixel, 3, &i, &j);
				comp_pixel = form_251(pixel[0], pixel[1], pixel[2], reduced_grey_levels);
				fwrite(&comp_pixel, sizeof(unsigned char), 1, res);
				break;

			case 2:
				if (j != 0)
					j--;
				else
				{
					i--;
					j = image->cols - 1;
				}
				setPixels(image, &pixel, 2, &i, &j);
				comp_pixel = form_44(pixel[0], pixel[1], reduced_grey_levels);
				fwrite(&comp_pixel, sizeof(unsigned char), 1, res);
				break;

			case 3:
				if (j != 0)
					j--;
				else
				{
					i--;
					j = image->cols - 1;
				}
				setPixels(image, &pixel, 3, &i, &j);
				comp_pixel = form_152(pixel[0], pixel[1], pixel[2], reduced_grey_levels);
				fwrite(&comp_pixel, sizeof(unsigned char), 1, res);
				break;

			case 4:
				if (j != 0)
					j--;
				else
				{
					i--;
					j = image->cols - 1;
				}
				setPixels(image, &pixel, 2, &i, &j);
				comp_pixel = form_35(pixel[0], pixel[1], reduced_grey_levels);
				fwrite(&comp_pixel, sizeof(unsigned char), 1, res);
				break;
			}
			count++;
		}
	}

	fclose(res);
}

void convertCompressedImageToPGM(char* compressed_file_name, char* pgm_file_name)
{
	FILE* comp_file;
	FILE* pgm_file;
	int i, j, pixel_count, char_count;
	unsigned short rows, cols, curr_max;
	unsigned char new_max_value;
	unsigned char* pixels = (unsigned char*)malloc(3 * sizeof(unsigned char));
	if (!pixels)
		allocation_error("pixels", "convertCompressedImageToPGM");

	checkFileEnding(compressed_file_name, ".bin");
	checkFileEnding(pgm_file_name, ".pgm");

	comp_file = fopen(compressed_file_name, "rb");
	if (!comp_file)
		error_opening_file(compressed_file_name, "convertCompressedImageToPGM");

	pgm_file = fopen(pgm_file_name, "w");
	if (!pgm_file)
		error_opening_file(pgm_file_name, "convertCompressedImageToPGM");

	fread(&rows, sizeof(unsigned short), 1, comp_file);
	fread(&cols, sizeof(unsigned short), 1, comp_file);
	fread(&new_max_value, sizeof(unsigned char), 1, comp_file);

	fprintf(pgm_file, "P2\n");
	fprintf(pgm_file, "%u %u\n", rows, cols);
	fprintf(pgm_file, "%hhu\n", MAX_VALUE);

	for (char_count = 0, pixel_count = 0, i = 0; i < rows; i++)
	{
		for (j = 0; j < cols;)
		{
			switch ((pixel_count % 5))
			{
			case 0:
				fread(&(pixels[0]), sizeof(unsigned char), 1, comp_file);
				rec_form_53(&(pixels[0]), &(pixels[1]), new_max_value);
				insertCharToPGM(pgm_file, &char_count, pixels[0]);
				j++;
				break;
			case 1:
				fread(&(pixels[0]), sizeof(unsigned char), 1, comp_file);
				rec_form_251(&(pixels[1]), &(pixels[0]), &(pixels[2]), new_max_value);
				insertCharToPGM(pgm_file, &char_count, pixels[1]);
				insertCharToPGM(pgm_file, &char_count, pixels[0]);
				j++;
				if (j >= cols)
				{
					i++;
					j = 0;
					j++;
				}
				else
					j++;
				break;
			case 2:
				fread(&(pixels[0]), sizeof(unsigned char), 1, comp_file);
				rec_form_44(&(pixels[2]), &(pixels[0]), new_max_value);
				insertCharToPGM(pgm_file, &char_count, pixels[2]);
				j++;
				break;
			case 3:
				fread(&(pixels[1]), sizeof(unsigned char), 1, comp_file);
				rec_form_152(&(pixels[0]), &(pixels[1]), &(pixels[2]), new_max_value);
				insertCharToPGM(pgm_file, &char_count, pixels[0]);
				insertCharToPGM(pgm_file, &char_count, pixels[1]);
				j++;
				if (j >= cols)
				{
					i++;
					j = 0;
					j++;
				}
				else
					j++;
				break;
			case 4:
				fread(&(pixels[0]), sizeof(unsigned char), 1, comp_file);
				rec_form_35(&(pixels[2]), &(pixels[0]), new_max_value);
				insertCharToPGM(pgm_file, &char_count, pixels[2]);
				insertCharToPGM(pgm_file, &char_count, pixels[0]);
				j++;
				if (j >= cols)
				{
					i++;
					j = 0;
					j++;
				}
				else
					j++;
				break;
			}
			pixel_count++;
		}
	}

	fclose(comp_file);
	fclose(pgm_file);
}

void checkFileEnding(char* file_name, char* correct_ending) {
	int name_len;
	int ending_len;
	int res;

	name_len = strlen(file_name);
	ending_len = strlen(correct_ending);
	res = strcmp(file_name + (name_len - ending_len), correct_ending);
	if (res != 0) {
		fprintf(stderr, "\nIncorrect file ending.");
		exit(INCORRECT_ENDING);
	}
}

void imageToPGM(char* fname, grayImage* image)
{
	FILE* pgm_file;
	int i, j, char_count;

	pgm_file = fopen(fname, "w");
	if (!pgm_file)
		error_opening_file(fname, "imageToPGM");

	fprintf(pgm_file, "P2\n");
	fprintf(pgm_file, "%u %u\n", image->rows, image->cols);
	fprintf(pgm_file, "%hhu\n", MAX_VALUE);

	for (char_count = 0, i = 0; i < image->rows; i++)
	{
		for (j = 0; j < image->cols; j++)
		{
			fprintf(pgm_file, "%hhu", (image->pixels)[i][j]);
			char_count++;

			if (char_count == MAX_CHARS)
			{
				putc('\n', pgm_file);
				char_count = 0;
			}
			else
			{
				putc(' ', pgm_file);
				char_count++;
			}
		}
	}

	fclose(pgm_file);
}