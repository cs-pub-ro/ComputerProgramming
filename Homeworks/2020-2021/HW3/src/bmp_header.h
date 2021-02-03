#ifndef BMP_HEADER_H
#define BMP_HEADER_H

/* Tells the compiler not to add padding for these structs. This may
   be useful when reading/writing to binary files.
   http://stackoverflow.com/questions/3318410/pragma-pack-effect
*/
#pragma pack(1)

typedef struct 
{
	unsigned char  fileMarker1; /* 'B' */
	unsigned char  fileMarker2; /* 'M' */
	unsigned int   bfSize;  /* File's size */
	unsigned short unused1; /* Aplication specific */
	unsigned short unused2; /* Aplication specific */
	unsigned int   imageDataOffset; /* Offset to the start of image data */
} bmp_fileheader;

typedef struct 
{
	unsigned int   biSize; /* Size of the info header - 40 bytes */
	signed int     width; /* Width of the image */
	signed int     height; /* Height of the image */
	unsigned short planes;
	unsigned short bitPix; /* Number of bits per pixel = 3 * 8 (for each channel R, G, B we need 8 bits */
	unsigned int   biCompression; /* Type of compression */
	unsigned int   biSizeImage; /* Size of the image data */
	int            biXPelsPerMeter;
	int            biYPelsPerMeter;
	unsigned int   biClrUsed;
	unsigned int   biClrImportant;
} bmp_infoheader;

#define BYTE unsigned char

/**
 * Structure that saves the information for one pixel as individual values
 */
typedef struct
{
	BYTE blue;
	BYTE green;
	BYTE red;
} pixel_t;


/**
 * Structure that agregates the info_header, file_header and bitmap into one
 */
typedef struct 
{
	bmp_fileheader fhdr;
	bmp_infoheader ihdr;
	pixel_t **bitmap;
} bmp_file_t;

/**
 * Bit masks for every possible digit and the wrong (default case)
 */
enum digit_masks {
	one		= 0b1000010000100001000010000,
	two 	= 0b1111100001111111000011111,
	three 	= 0b1111110000111111000011111,
	four 	= 0b1000010000111111000110001,
	five 	= 0b1111110000111110000111111,
	six 	= 0b1111110001111110000111111,
	seven 	= 0b1000010000100001000011111,
	eight 	= 0b1111110001111111000111111,
	nine 	= 0b1111110000111111000111111,
	wrong	= 0b1000101010001000101010001
};


#define SUDOKU_SIZE 9

// Colors
#define MAGENTA {255, 0, 255}
#define WHITE {255, 255, 255}
#define RED {0, 0, 255}

#pragma pack()
#endif