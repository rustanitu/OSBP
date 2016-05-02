/************************************************/
/*	@author Rustam Mesquita.
/* 
/*	This module intend to provide functions so
/* its user can read, compose and obtain
/* information of BMP image files.
/*
/* @version 1.0 (03/04/2015)
/* @version 2.0 (13/04/2015)
/************************************************/
#ifndef BMPHEADER_H
#define BMPHEADER_H


#include <stdlib.h>
#include <string.h>
#include <fstream>


/************************************************/
/*	Constants definitions
/************************************************/
#define PIXEL_COMPONENTS 3
#define NEIGHBORHOOD 9


/************************************************/
/*	Type definitions
/************************************************/
namespace BMPR
{
  typedef unsigned char BYTE;
  typedef unsigned short int SWORD;
  typedef unsigned int WORD;
  typedef struct _bmpFile* BmpFile;
}

/************************************************/
/*	Instantiates a BMPR::BmpFile struct
/*	
/*	@return A BMPR::BmpFile structure.
/************************************************/
BMPR::BmpFile createBmpStruct ();


/************************************************/
/*	Allocates memory for image data
/*	
/*	@return A BMPR::BmpFile structure.
/************************************************/
void dataAlloc (BMPR::BmpFile bmpFile);


/************************************************/
/*	Reads a BMP file and stores it in the
/* described structure above.
/* 
/*	@param A complete path to the BMP file.
/*	
/*	@return A BMPR::BmpFile structure filled with
/* BMP file data.
/************************************************/
BMPR::BmpFile readBmpFile (char * fileName);


/************************************************/
/*	Copies a BMPR::BmpFile header to another BMPR::BmpFile.
/************************************************/
void copyHeader ( BMPR::BmpFile dest, BMPR::BmpFile src );


/************************************************/
/*	Creates a BMPR::BmpFile copy, except for its image
/* data.
/*	
/*	@return A BMPR::BmpFile structure.
/************************************************/
BMPR::BmpFile copyBmpfileBase (BMPR::BmpFile bmpFile);


/************************************************/
/*	Prints information about BMP file: signature
/* file size, width, height, data size
/* and pits per pixel.
/* 
/*	@param A filled BMP file structure.
/************************************************/
void printBmpFile (BMPR::BmpFile bmpFile);


/************************************************/
/*	Destroys a BMP file structure.
/* 
/*	@param A filled BMP file structure.
/************************************************/
void destroyBmpFile (BMPR::BmpFile bmpFile);


/************************************************/
/*	Increments a data buffer with a BMP data.
/* 
/*	@param A filled BMP file structure.
/*	@param A data buffer to be incremented with
/* bmp data file.
/************************************************/
void incrementDataBufferWithBmpData (BMPR::BmpFile bmpFile, double* dataBuffer);


/************************************************/
/*	Gets BMP image data file size.
/* 
/*	@param A filled BMP file structure.
/*	
/*	@return The BMP data file size.
/************************************************/
BMPR::WORD getDataSize(BMPR::BmpFile bmpFile);


/************************************************/
/*	Gets BMP image data file size without padding.
/* 
/*	@param A filled BMP file structure.
/*	
/*	@return The BMP data file size.
/************************************************/
BMPR::WORD getImageDataSize(BMPR::BmpFile bmpFile);


/************************************************/
/*	Gets BMP image data file.
/* 
/*	@param A filled BMP file structure.
/*	
/*	@return The BMP data file.
/************************************************/
float* getData (BMPR::BmpFile bmpFile);


/************************************************/
/*	Sets BMP image data file.
/* 
/*	@param A filled BMP file structure.
/*	@param A data buffer with the new data file.
/************************************************/
void setData (BMPR::BmpFile bmpFile, float* data);


/************************************************/
/*	Gets BMP image width.
/* 
/*	@param A filled BMP file structure.
/*
/*	@return The BMP image width
/************************************************/
BMPR::WORD getWidth(BMPR::BmpFile bmpFile);


/************************************************/
/*	Gets BMP image height.
/* 
/*	@param A filled BMP file structure.
/*
/*	@return The BMP image height
/************************************************/
BMPR::WORD getHeight(BMPR::BmpFile bmpFile);


/************************************************/
/*	Gets a BMP image pixel.
/* 
/*	@param A filled BMP file structure.
/*	@param A pixel x position.
/*	@param A pixel y position.
/*	@param A pointer to receive the pixel 
/* R component.
@param A pointer to receive the pixel 
/* G component.
@param A pointer to receive the pixel 
/* B component.
/************************************************/
void getPixel (BMPR::BmpFile bmpFile, int x, int y, float* r, float* g, float* b);


/************************************************/
/*	Sets a BMP image pixel.
/* 
/*	@param A filled BMP file structure.
/*	@param A pixel x position.
/*	@param A pixel y position.
/*	@param A pixel R component.
/*	@param A pixel G component.
/*	@param A pixel B component.
/************************************************/
void setPixel (BMPR::BmpFile bmpFile, int x, int y, float r, float g, float b);


/************************************************/
/*	Compose a BMP image file based on a BMP
/* structure.
/* 
/*	@param A filled BMP file structure.
/*	@param The new file name.
/************************************************/
void writeBmpFile (BMPR::BmpFile bmpFile, char* fileName);


/************************************************/
/*	Verify a BMP content is padding or image data.
/* 
/*	@param A filled BMP file structure.
/*	@param The content position.
/*
/*	@return 1 if is padding content and 0 if
/* it is a image data content
/************************************************/
BMPR::SWORD isPadding(BMPR::BmpFile bmpFile, BMPR::SWORD pos);

#endif