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
#include "BmpReader.h"


/************************************************/
/*	Structure of a 24-bit BMP File				*/
/************************************************/
struct BMPR::_bmpFile
{
	BYTE signature[3];
	WORD fileSize;
	WORD reserved;
	WORD dataAddress;
	WORD dibHeaderSize;
	WORD width;
	WORD height;
	SWORD colorPlanes;
	SWORD bitsPerPixel;
	WORD compression;
	WORD dataSize;
	WORD ignored[4];
	SWORD padding;
	WORD imageDataSize;
	float *data;
};


/************************************************/
/*	Instantiates a BMPR::BmpFile struct
/*	
/*	@return A BMPR::BmpFile structure.
/************************************************/
BMPR::BmpFile createBmpStruct ()
{
	BMPR::BmpFile bmpFile = (BMPR::BmpFile)malloc(sizeof(BMPR::_bmpFile));
	return bmpFile;
}


/************************************************/
/*	Allocates memory for image data
/*	
/*	@return A BMPR::BmpFile structure.
/************************************************/
void dataAlloc (BMPR::BmpFile bmpFile)
{
	bmpFile->data = (float*)malloc(bmpFile->dataSize*sizeof(float));
}


/************************************************/
/*	Reads a BMP file and stores it in the
/* described structure above.
/* 
/*	@param A complete path to the BMP file.
/*	
/*	@return A BMPR::BmpFile structure filled with
/* BMP file data.
/************************************************/
BMPR::BmpFile readBmpFile (char * fileName)
{
	BMPR::BmpFile bmpFile = createBmpStruct();
	std::ifstream file( fileName, std::ios::binary);
	
	// BMP FILE HEADER
	file.read((char*)(bmpFile->signature), 2 * sizeof(BMPR::BYTE));
	bmpFile->signature[2] = '\0';
	file.read((char*)(&bmpFile->fileSize), sizeof(BMPR::WORD));
	file.read( (char*)(&bmpFile->reserved), sizeof(int));
	file.read((char*)(&bmpFile->dataAddress), sizeof(BMPR::WORD));

	// DIB HEADER
	file.read((char*)(&bmpFile->dibHeaderSize), sizeof(BMPR::WORD));
	file.read((char*)(&bmpFile->width), sizeof(BMPR::WORD));
	file.read((char*)(&bmpFile->height), sizeof(BMPR::WORD));
	file.read((char*)(&bmpFile->colorPlanes), sizeof(BMPR::SWORD));
	file.read((char*)(&bmpFile->bitsPerPixel), sizeof(BMPR::SWORD));
	file.read((char*)(&bmpFile->compression), sizeof(BMPR::WORD));
	file.read((char*)(&bmpFile->dataSize), sizeof(BMPR::WORD));
	file.read((char*)(bmpFile->ignored), 4 * sizeof(BMPR::WORD));

	// Calculate padding
	bmpFile->padding = bmpFile->width % (sizeof(BMPR::WORD));
	bmpFile->imageDataSize = bmpFile->width * bmpFile->height * PIXEL_COMPONENTS;
	bmpFile->dataSize = bmpFile->imageDataSize + bmpFile->padding * bmpFile->height;

	// DATA
	BMPR::BYTE* data = (BMPR::BYTE*)malloc(bmpFile->dataSize*sizeof(BMPR::BYTE));
	file.read((char*)data, bmpFile->dataSize*sizeof(BMPR::BYTE));
	
	bmpFile->data = (float*)malloc(bmpFile->dataSize*sizeof(float));
	for (int i=0; i<bmpFile->dataSize; i++)
	{
		bmpFile->data[i] = (float)data[i];
	}	
	
	file.close();
	return bmpFile;
}


/************************************************/
/*	Copies a BMPR::BmpFile header to another BMPR::BmpFile.
/************************************************/
void copyHeader ( BMPR::BmpFile dest, BMPR::BmpFile src )
{
	if ( dest == NULL || src == NULL )
	{
		return;
	}

	dest->signature[0] = src->signature[0];
	dest->signature[1] = src->signature[1];
	dest->signature[2] = src->signature[2];
	dest->fileSize = src->fileSize;
	dest->reserved = src->reserved;
	dest->dataAddress = src->dataAddress;
	dest->dibHeaderSize = src->dibHeaderSize;
	dest->width = src->width;
	dest->height = src->height;
	dest->colorPlanes = src->colorPlanes;
	dest->bitsPerPixel = src->bitsPerPixel;
	dest->compression = src->compression;
	dest->dataSize = src->dataSize;
	dest->ignored[0] = src->ignored[0];
	dest->ignored[1] = src->ignored[1];
	dest->ignored[2] = src->ignored[2];
	dest->ignored[3] = src->ignored[3];
	dest->padding = src->padding;
	dest->imageDataSize = src->imageDataSize;
}


/************************************************/
/*	Creates a BMPR::BmpFile copy, except for its image
/* data.
/*	
/*	@return A BMPR::BmpFile structure.
/************************************************/
BMPR::BmpFile copyBmpfileBase (BMPR::BmpFile bmpFile)
{
	BMPR::BmpFile file = createBmpStruct();
	copyHeader(file, bmpFile);
	dataAlloc(file);
	return file;
}


/************************************************/
/*	Prints information about BMP file: signature
/* file size, width, height, data size
/* and pits per pixel.
/* 
/*	@param A filled BMP file structure.
/************************************************/
void printBmpFile (BMPR::BmpFile bmpFile)
{
	printf(" Signature: %s \n", bmpFile->signature);
	printf(" FileSize: %.2fMb \n", (bmpFile->fileSize/1024.0)/1024.0);
	printf(" Width: %u \n", bmpFile->width);
	printf(" Height: %u \n", bmpFile->height);
	printf(" Data Size: %u \n", bmpFile->dataSize);
	printf(" Bits por pixel: %u \n\n", bmpFile->bitsPerPixel);
}


/************************************************/
/*	Destroys a BMP file structure.
/* 
/*	@param A filled BMP file structure.
/************************************************/
void destroyBmpFile (BMPR::BmpFile bmpFile)
{
	if (bmpFile != NULL)
	{
		if (bmpFile->data != NULL)
		{
			free(bmpFile->data);
		}
		free(bmpFile);
	}
}


/************************************************/
/*	Increments a data buffer with a BMP data.
/* 
/*	@param A filled BMP file structure.
/*	@param A data buffer to be incremented with
/* bmp data file.
/************************************************/
void incrementDataBufferWithBmpData (BMPR::BmpFile bmpFile, double* dataBuffer)
{
	BMPR::WORD dataSize = bmpFile->dataSize;
	float* data = bmpFile->data;

	for( int i=0; i<dataSize; i++)
	{
		if ( isPadding(bmpFile, i) )
		{
			continue;
		}
		dataBuffer[i] += (double)data[i];
	}
}


/************************************************/
/*	Gets BMP image data file size.
/* 
/*	@param A filled BMP file structure.
/*	
/*	@return The BMP data file size.
/************************************************/
BMPR::WORD getDataSize(BMPR::BmpFile bmpFile)
{
	return bmpFile->dataSize;
}


/************************************************/
/*	Gets BMP image data file size without padding.
/* 
/*	@param A filled BMP file structure.
/*	
/*	@return The BMP data file size.
/************************************************/
BMPR::WORD getImageDataSize(BMPR::BmpFile bmpFile)
{
	return bmpFile->imageDataSize;
}


/************************************************/
/*	Gets BMP image data file.
/* 
/*	@param A filled BMP file structure.
/*	
/*	@return The BMP data file.
/************************************************/
float* getData (BMPR::BmpFile bmpFile)
{
	return bmpFile->data;
}


/************************************************/
/*	Sets BMP image data file.
/* 
/*	@param A filled BMP file structure.
/*	@param A data buffer with the new data file.
/************************************************/
void setData (BMPR::BmpFile bmpFile, float* data)
{
	bmpFile->data = data;
}


/************************************************/
/*	Gets BMP image width.
/* 
/*	@param A filled BMP file structure.
/*
/*	@return The BMP image width
/************************************************/
BMPR::WORD getWidth(BMPR::BmpFile bmpFile)
{
	return bmpFile->width;
}


/************************************************/
/*	Gets BMP image height.
/* 
/*	@param A filled BMP file structure.
/*
/*	@return The BMP image height
/************************************************/
BMPR::WORD getHeight(BMPR::BmpFile bmpFile)
{
	return bmpFile->height;
}


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
void getPixel (BMPR::BmpFile bmpFile, int x, int y, float* r, float* g, float* b)
{
	float* data = bmpFile->data;
	BMPR::WORD width = bmpFile->width;
	BMPR::WORD height = bmpFile->height;
	BMPR::SWORD padding = bmpFile->padding;
	BMPR::SWORD wLength = width*PIXEL_COMPONENTS + padding;

	int idx = y*wLength + PIXEL_COMPONENTS*x;
	*b = data[idx];
	*g = data[idx + 1];
	*r = data[idx + 2];
}


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
void setPixel (BMPR::BmpFile bmpFile, int x, int y, float r, float g, float b)
{
	float* data = bmpFile->data;
	BMPR::WORD width = bmpFile->width;
	BMPR::WORD height = bmpFile->height;
	BMPR::SWORD padding = bmpFile->padding;
	BMPR::SWORD wLength = width*PIXEL_COMPONENTS + padding;

	int idx = y*wLength + PIXEL_COMPONENTS*x;
	data[idx] = b;
	data[idx + 1] = g;
	data[idx + 2] = r;
}


/************************************************/
/*	Compose a BMP image file based on a BMP
/* structure.
/* 
/*	@param A filled BMP file structure.
/*	@param The new file name.
/************************************************/
void writeBmpFile (BMPR::BmpFile bmpFile, char* fileName)
{
	FILE* fp;
	fopen_s(&fp, fileName, "wb");
	
	fwrite(bmpFile->signature, 1, 2, fp);
	fwrite(&bmpFile->fileSize, 1, 4, fp);
	fwrite(&bmpFile->reserved, 1, 4, fp);
	fwrite(&bmpFile->dataAddress, 1, 4, fp);
	fwrite(&bmpFile->dibHeaderSize, 1, 4, fp);
	fwrite(&bmpFile->width, 1, 4, fp);
	fwrite(&bmpFile->height, 1, 4, fp);
	fwrite(&bmpFile->colorPlanes, 1, 2, fp);
	fwrite(&bmpFile->bitsPerPixel, 1, 2, fp);
	fwrite(&bmpFile->compression, 1, 4, fp);
	fwrite(&bmpFile->dataSize, 1, 4, fp);
	fwrite(&bmpFile->ignored, 1, 16, fp);

	BMPR::BYTE* data = (BMPR::BYTE*)malloc(bmpFile->dataSize*sizeof(BMPR::BYTE));
	for (int i=0; i<bmpFile->dataSize; i++)
	{
		data[i] = (BMPR::BYTE) bmpFile->data[i];
	}
	fwrite(data, 1, bmpFile->dataSize, fp);

	fclose(fp);
}


/************************************************/
/*	Verify a BMP content is padding or image data.
/* 
/*	@param A filled BMP file structure.
/*	@param The content position.
/*
/*	@return 1 if is padding content and 0 if
/* it is a image data content
/************************************************/
BMPR::SWORD isPadding(BMPR::BmpFile bmpFile, BMPR::SWORD pos)
{
	float* data = bmpFile->data;
	BMPR::SWORD padding = bmpFile->padding;
	BMPR::SWORD width = bmpFile->width;
	BMPR::SWORD wLength = width*PIXEL_COMPONENTS;
	BMPR::SWORD height = bmpFile->height;
	
	BMPR::SWORD y = pos/wLength;
	if ( pos >= (y+1)*wLength - padding )
	{
		return 1;
	}

	return 0;
}