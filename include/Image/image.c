/*  Departamento de Informatica, PUC-Rio, INF1761 Computer Graphhics
*    
*   @file cor.h TAD: digital image (implementation).
*   @author Marcelo Gattass and others
*
*   @date
*         Last versio:     08/2014.
*
*   @version 5.1
* 
*   @Copyright/License
*   DI PUC-Rio Educational Software
*   All the products under this license are free software: they can be used for both academic and commercial purposes at absolutely no cost. 
*   There are no paperwork, no royalties, no GNU-like "copyleft" restrictions, either. Just download and use it. 
*   They are licensed under the terms of the MIT license reproduced below, and so are compatible with GPL and also qualifies as Open Source software. 
*   They are not in the public domain, PUC-Rio keeps their copyright. The legal details are below. 
* 
*   The spirit of this license is that you are free to use the libraries for any purpose at no cost without having to ask us. 
*   The only requirement is that if you do use them, then you should give us credit by including the copyright notice below somewhere in your product or its documentation. 
*   
*   Copyright � 2010-2011 DI PUC-Rio Educational Software
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software 
*   without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sub license, and/or sell copies of the Software, and to permit 
*   persons to whom the Software is furnished to do so, subject to the following conditions: 
*   
*   The above copyright notice and this permission notice shall be included in all copies or suavlantial portions of the Software. 
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
*
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <memory.h>

#include "image.h"

#define ROUND(_) (int)floor( (_) + 0.5 )
#define N_CORES   256
#define MAIOR_COR 255



struct _image {
   int dcs;        /* define a dim do espaco de cor (dimension of the color space): 3=RGB, 1=luminancia */
   int width;      /* numero de pixels na direcao horizontal da imagem */
   int height;     /* numero de pixels na direcao vertical da imagem   */
   float *buf;      /* vetor de dimensao dcs*width*height que armazena consecutivamente as componentes de cor */
                   /* de cada pixel a partir do canto inferior esquerdo da imagem.  */
                   /* A posicao das componentes de cor do pixel (x,y) fica armazenada */
                   /* a partir da posicao: (y*width*dcs) + (x*dcs)  */
};


/************************************************************************/
/* Definicao das Funcoes Privadas                                       */
/************************************************************************/

/*  getuint e putuint:
* Funcoes auxiliares para ler e escrever inteiros na ordem (lo-hi)
* Note  que  no Windows as variaveis tipo "unsigned short int" sao 
* armazenadas  no disco em dois bytes na ordem inversa. Ou seja, o 
* numero  400, por exemplo, que  pode ser escrito como 0x190, fica 
* armazenado  em dois bytes consecutivos 0x90 e 0x01. Nos sistemas 
* UNIX e Mac este mesmo inteiro seria armazenado  na  ordem 0x01 e 
* 0x90. O armazenamento do Windows e' chamado  de  "little endian"  
* (i.e., lowest-order byte stored first), e  no  sitemas  Unix sao
* "big-endian" (i.e., highest-order byte stored first). 
*/

/***************************************************************************
* Reads an unsigned integer from input                                     *
***************************************************************************/
static int getuint(unsigned short *uint, FILE *input)
{
   int got;
   unsigned char temp[2];
   unsigned short tempuint;

   got = (int) fread(&temp, 1, 2, input);
   if (got != 2) return 0;

   tempuint = ((unsigned short)(temp[1])<<8) | ((unsigned short)(temp[0]));

   *uint = tempuint;

   return 1;
}

/***************************************************************************
* Writes an unsigned integer in output                                     *
***************************************************************************/
static int putuint(unsigned short uint, FILE *output)
{
   int put;
   unsigned char temp[2];

   temp[0] = uint & 0xff;
   temp[1] = (uint >> 8) & 0xff;

   put = (int) fwrite(&temp, 1, 2, output);
   if (put != 2) return 0;

   return 1;
}

/***************************************************************************
* Reads a long integer from input                                          *
***************************************************************************/ 
static int getlong(FILE *input, long int *longint)
{
   int got;
   unsigned char temp[4];
   long int templongint;

   got = (int)fread(&temp, 1, 4, input);
   if (got != 4) return 0;

   templongint = ((long int)(temp[3])<<24) | ((long int)(temp[2])<<16) 
      | ((long int)(temp[1])<<8) | ((long int)(temp[0]));

   *longint = templongint;

   return 1;
}

/***************************************************************************
* Writes a long integer in output                                          *
***************************************************************************/ 
static int putlong(FILE *output, long int longint)
{
   int put;
   unsigned char temp[4];

   temp[0] = (unsigned char)longint & 0xff;
   temp[1] = (unsigned char)(longint >> 8) & 0xff;
   temp[2] = (unsigned char)(longint >> 16) & 0xff;
   temp[3] = (unsigned char)(longint >> 24) & 0xff;

   put = (int)fwrite(&temp, 1, 4, output);

   if (put != 4) return 0;

   return 1;
}

/***************************************************************************
* Reads a word from input                                                  *
***************************************************************************/ 
static int getword(FILE *input, unsigned short int *word)
{
   int got;
   unsigned char temp[2];
   unsigned short int tempword;

   got = (int)fread(&temp, 1, 2, input);
   if (got != 2) return 0;

   tempword = ((unsigned short int)(temp[1])<<8) | ((unsigned short int)(temp[0]));

   *word = tempword;

   return 1;
}

/***************************************************************************
* Writes a word in output                                                  *
***************************************************************************/ 
static int putword(FILE *output, unsigned short int word)
{
   int put;
   unsigned char temp[2];

   temp[0] = word & 0xff;
   temp[1] = (word >> 8) & 0xff;

   put = (int)fwrite(&temp, 1, 2, output);
   if (put != 2) return 0;

   return 1;
}

/***************************************************************************
* Reads a double word from input                                           *
***************************************************************************/ 
static int getdword(FILE *input, unsigned long int *dword)
{
   int got;
   unsigned char temp[4];
   unsigned long int tempdword;

   got = (int)fread(&temp, 1, 4, input);
   if (got != 4) return 0;

   tempdword = ((unsigned long int)(temp[3])<<24) | ((unsigned long int)(temp[2])<<16) 
      | ((unsigned long int)(temp[1])<<8) | ((unsigned long int)(temp[0]));

   *dword = tempdword;

   return 1;
}

/***************************************************************************
* Writes a double word in output                                           *
***************************************************************************/ 
static int putdword(FILE *output, unsigned long int dword)
{
   int put;
   unsigned char temp[4];

   temp[0] = (unsigned char) (dword & 0xff);
   temp[1] = (unsigned char) ((dword >>  8) & 0xff);
   temp[2] = (unsigned char) ((dword >> 16) & 0xff);
   temp[3] = (unsigned char) ((dword >> 24) & 0xff);

   put = (int)fwrite(&temp, 1, 4, output);

   if (put != 4) return 0;

   return 1;
}

static float luminance(float red, float green, float blue)
{
 return 0.2126f*red +0.7152f*green+0.0722f*blue;
}


/************************************************************************/
/* Definicao das Funcoes Exportadas                                     */
/************************************************************************/

Image* imgCreate(int w, int h, int dcs)
{
   Image* image = (Image*) malloc (sizeof(Image));
   assert(image);
   image->width  = w;
   image->height = h;
   image->dcs = dcs;
   image->buf = (float*) calloc (w * h * dcs , sizeof(float));
   assert(image->buf);
   return image;
}

void imgDestroy(Image* image)
{
   if (image)
   {
      if (image->buf) free (image->buf);
      free(image);
   }
}

Image* imgCopy(Image* original)
{
   int w   = imgGetWidth(original);
   int h   = imgGetHeight(original);
   int dcs = imgGetDimColorSpace(original);
   Image* copy = imgCreate(w,h,dcs);

   memcpy(copy->buf,original->buf,w*h*dcs*sizeof(float));
/* less efficient
   int x,y,i;
   for (y=0;y<h;y++){
      for (x=0;x<w;x++) {
		  for (i=0;i<dcs;i++) {
			  copy->buf[y*w*dcs+x*dcs+i]=original->buf[y*w*dcs+x*dcs+i];

		  }
      }
   }
*/

   return copy;
}



Image* imgGrey(Image* image)
{
   int w = imgGetWidth(image);
   int h = imgGetHeight(image);
   Image* img1=imgCreate(w,h,1);
   int x,y;
   float rgb[3],grey[3];

   for (y=0;y<h;y++){
      for (x=0;x<w;x++) {
         imgGetPixel3fv(image,x,y,rgb);
         grey[0]=luminance(rgb[0],rgb[1],rgb[2]);
         grey[1]=grey[0];
         grey[2]=grey[0];
         imgSetPixel3fv(img1,x,y,grey);
      }
   }
   return img1;
}

Image* imgResize(Image* img0, int w1, int h1) 
{
   Image* img1 = imgCreate(w1,h1,img0->dcs);
   float w0 = (float) img0->width;  /* passa para float para fazer contas */
   float h0 = (float) img0->height;

   int x0,y0,x1,y1;
   float color[3];

   for (y1=0;y1<h1;y1++)
      for (x1=0;x1<w1;x1++)
      {
         x0=ROUND(w0*x1/w1);   /* pega a cor do pixel mais proxima */
         y0=ROUND(h0*y1/h1);
         imgGetPixel3fv(img0,x0,y0,color);
         imgGetPixel3fv(img1,x1,y1,color);
      }
      return img1;
}


float imgDif(Image*img0, Image*img1, float gamma)
{
   int w = imgGetWidth(img0);
   int h = imgGetHeight(img0);
   int x,y;
   float rgb[3],rgb0[3],rgb1[3],avg=0.f,tot=0.f;

   for (y=0;y<h;y++){
      for (x=0;x<w;x++) {
            float lum,new_lum,ratio;
         
            imgGetPixel3fv(img0,x,y,rgb0);
            imgGetPixel3fv(img1,x,y,rgb1);

            /* calcula o modulo da diferenca */
            rgb[0]=(rgb1[0]>rgb0[0])? rgb1[0]-rgb0[0] : rgb0[0]-rgb1[0] ;
            rgb[1]=(rgb1[1]>rgb0[1])? rgb1[1]-rgb0[1] : rgb0[1]-rgb1[1] ;
            rgb[2]=(rgb1[2]>rgb0[2])? rgb1[2]-rgb0[2] : rgb0[2]-rgb1[2] ;

            /* acumula a soma */
            avg+=(rgb[0]+rgb[1]+rgb[2]);
            tot+=(rgb0[0]+rgb0[1]+rgb0[2]);

            /* corrige por gamma */
            lum = luminance(rgb[0],rgb[1],rgb[2]);
            new_lum = (float) pow(lum,1./gamma);
            ratio = new_lum/lum;
            rgb[0]*=ratio; rgb[1]*=ratio; rgb[2]*=ratio;

            imgSetPixel3fv(img0,x,y,rgb);
      }
   }
    avg=100*avg/tot;
    return avg;
}

float imgErr(Image*img0, Image*img1)
{
   int w = imgGetWidth(img0);
   int h = imgGetHeight(img0);
   int x,y;
   float rgb0[3],rgb1[3],delta_rgb[3],avg=0.f,tot=0.f;

   for (y=0;y<h;y++){
      for (x=0;x<w;x++) {
            imgGetPixel3fv(img0,x,y,rgb0);
            imgGetPixel3fv(img1,x,y,rgb1);

            /* calcula o modulo da diferenca */
            delta_rgb[0]=(rgb1[0]>rgb0[0])? rgb1[0]-rgb0[0] : rgb0[0]-rgb1[0] ;
            delta_rgb[1]=(rgb1[1]>rgb0[1])? rgb1[1]-rgb0[1] : rgb0[1]-rgb1[1] ;
            delta_rgb[2]=(rgb1[2]>rgb0[2])? rgb1[2]-rgb0[2] : rgb0[2]-rgb1[2] ;

            /* acumula a soma */
            avg+=(delta_rgb[0]+delta_rgb[1]+delta_rgb[2]);
            tot+=(rgb0[0]+rgb0[1]+rgb0[2]);
      }
   }
   avg=avg/tot;
   return 100*avg;
}

int imgGetWidth(Image* image)
{
   return image->width;
}

int imgGetHeight(Image* image)
{
   return image->height;
}

int imgGetDimColorSpace(Image* image)
{
   return image->dcs;
}

float* imgGetData(Image* image)
{
   return image->buf;
}

void imgSetPixel3fv(Image* image, int x, int y, float*  color)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   switch (image->dcs) {
      case 3:
         image->buf[pos  ] = color[0];
         image->buf[pos+1] = color[1];
         image->buf[pos+2] = color[2];
         break;
      case 1:
         image->buf[pos  ] = luminance(color[0],color[1],color[2]);
         break;
      default:
         break;
   }
}

void imgSetPixel3f(Image* image, int x, int y, float R, float G, float B)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   switch (image->dcs) {
      case 3:
         image->buf[pos  ] = R;
         image->buf[pos+1] = G;
         image->buf[pos+2] = B;
         break;
      case 1:
         image->buf[pos  ] = luminance(R,G,B);
         break;
      default:
         break;
   }
}

void imgGetPixel3fv(Image* image, int x, int y, float* color)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   switch (image->dcs) {
      case 3:
         color[0] = image->buf[pos  ];
         color[1] = image->buf[pos+1];
         color[2] = image->buf[pos+2];
         break;
      case 1:
         color[0] = image->buf[pos  ];
         color[1] = color[0];
         color[2] = color[0];
         break;
      default:
         break;
   }
}


void imgGetPixel3f(Image* image, int x, int y, float* R, float* G, float* B)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   switch (image->dcs) {
      case 3:
         *R = image->buf[pos  ];
         *G = image->buf[pos+1];
         *B = image->buf[pos+2];
         break;
      case 1:
         *R = image->buf[pos  ];
         *G = *R;
         *B = *R;
         break;
      default:
         break;
   }
}

void imgSetPixel3ubv(Image* image, int x, int y, unsigned char * color)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   switch (image->dcs) {
      case 3:
         image->buf[pos  ] = (float)(color[0]/255.);
         image->buf[pos+1] = (float)(color[1]/255.);
         image->buf[pos+2] = (float)(color[2]/255.);
         break;
      case 1:
         image->buf[pos  ] = luminance((float)(color[0]/255.),(float)(color[1]/255.),(float)(color[2]/255.));
         break;
      default:
         break;
   }
}

void imgGetPixel3ubv(Image* image, int x, int y, unsigned char *color)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   int r,g,b;
   switch (image->dcs) {
      case 3:
         r= ROUND(255*image->buf[pos]);
         g= ROUND (255*image->buf[pos+1]);
         b= ROUND (255*image->buf[pos+2]);
         color[0] = (unsigned char)(r<256) ? r : 255 ;
         color[1] = (unsigned char)(g<256) ? g : 255 ;
         color[2] = (unsigned char)(b<256) ? b : 255 ;
         break;
      case 1:
         r=g=b= ROUND(255*image->buf[pos]);
         color[0] = (unsigned char)(r<256) ? r : 255 ;
         color[1] = (unsigned char)(g<256) ? g : 255 ;
         color[2] = (unsigned char)(b<256) ? b : 255 ;
         break;
      default:
         break;
   }
}


static void ConvertToFloat(const unsigned char* image_data_byte, float* image_data, int size)
{
	int i;
	for (i = size - 1; i >= 0; i--)
	{
		/* start by the last pixel so we can do it in-place */
		image_data[i] = ((float)image_data_byte[i]) / 255.0f;
	}
}

Image* imgFromColorBuffer(int width, int height, int dcs, unsigned char* image_data_byte)
{
	Image* image = (Image*)malloc(sizeof(Image));
	assert(image);
	image->width = width;
	image->height = height;
	image->dcs = dcs;
	image->buf = (float*)malloc(width * height * dcs * sizeof(float));
	assert(image->buf);
	ConvertToFloat(image_data_byte, image->buf, width*height*dcs); /* converto to FLOAT */
	return image;
}

void imgDataBufferByte(Image* image, unsigned char* image_data_byte)
{
	int i, size = image->width*image->height*image->dcs;
	for (i = 0; i<size; i++)
	{
		/* start by the first pixel so we can do it in-place */
		image_data_byte[i] = (unsigned char)(255 * image->buf[i]);
	}
}







/* Compiler dependent definitions */
typedef unsigned char BYTE;       
typedef unsigned short int USHORT;  
typedef unsigned short int WORD;  
typedef long int LONG;            
typedef unsigned long int DWORD;


Image* imgReadBMP(char *filename)
{
   FILE  *filePtr;            /* ponteiro do arquivo */
   Image*image;            /* imagem a ser criada */
   BYTE *linedata;

   USHORT  bfType;             /* "BM" = 19788           */
   LONG    biWidth;            /* image width in pixels  */
   LONG    biHeight;           /* image height in pixels */
   WORD    biBitCount;         /* bitmap color depth     */
   DWORD   bfSize;

   USHORT  ushortSkip;         /* dado lixo USHORT */
   DWORD   dwordSkip;         /* dado lixo DWORD  */
   LONG    longSkip;         /* dado lixo LONG   */
   WORD    wordSkip;         /* dado lixo WORD   */

   LONG i, j, k, l, linesize, got;

   /* abre o arquivo com a imagem BMP */
   fopen_s(&filePtr, filename, "rb");
   assert(filePtr);

   /* verifica se eh uma imagem bmp */
   getuint(&bfType, filePtr);
   assert(bfType == 19778);

   /* pula os 12 bytes correspondentes a bfSize, Reserved1 e Reserved2 */
   getdword(filePtr, &bfSize);
   getuint(&ushortSkip, filePtr);  /* Reserved1, deve ter valor 0 */
   assert(ushortSkip == 0);
   getuint(&ushortSkip, filePtr);  /* Reserved2, deve ter valor 0 */
   assert(ushortSkip == 0);

   /* pula os 4 bytes correspondentes a bfOffBits, que deve ter valor 54 */
   getdword(filePtr, &dwordSkip);
   assert(dwordSkip == 54);

   /* pula os 4 bytes correspondentes a biSize, que deve ter valor 40 */
   getdword(filePtr, &dwordSkip);
   assert(dwordSkip == 40);

   /* pega largura e altura da imagem */
   getlong(filePtr, &biWidth);
   getlong(filePtr, &biHeight);

   /* verifica que o numero de quadros eh igual a 1 */
   getword(filePtr, &wordSkip);
   assert(wordSkip == 1);

   /* Verifica se a imagem eh de 24 bits */
   getword(filePtr, &biBitCount);
   if(biBitCount != 24)
   {
      fprintf(stderr, "imgReadBMP: Not a bitmap 24 bits file.\n");      
      fclose(filePtr);
      return (NULL);
   }

   /* pula os demais bytes do infoheader */
   getdword(filePtr, &dwordSkip);
   assert(dwordSkip == 0);
   getdword(filePtr, &dwordSkip);
   getlong(filePtr, &longSkip);
   getlong(filePtr, &longSkip);
   getdword(filePtr, &dwordSkip);
   getdword(filePtr, &dwordSkip);

   image = imgCreate(biWidth, biHeight,3);

   /* a linha deve terminar em uma fronteira de dword */
   linesize = 3*image->width;
   if (linesize & 3) {
      linesize |= 3;
      linesize++;
   }

   /* aloca espaco para a area de trabalho */ 
   linedata = (BYTE *) malloc(linesize);
   if (linedata == NULL) {
      fprintf(stderr, "get24bits: Not enough memory.\n");
      return 0;
   }

   /* pega as componentes de cada pixel */
   for (k=0, i=0; i<image->height; i++) {
      got = (unsigned long int)fread(linedata, linesize, 1, filePtr);
      if (got != 1) {
         free(linedata);
         fprintf(stderr, "get24bits: Unexpected end of file.\n");
      }
      for (l=1, j=0; j<image->width; j++, l=l+3) {
         image->buf[k++] = (float)(linedata[l+1]/255.);
         image->buf[k++] = (float)(linedata[l  ]/255.); 
         image->buf[k++] = (float)(linedata[l-1]/255.);
      }
   }

   free(linedata);
   fclose(filePtr);
   return image;
}

int imgWriteBMP(char *filename, Image* bmp)
{
   FILE          *filePtr;         /* ponteiro do arquivo */
   unsigned char *filedata;
   DWORD bfSize;
   int i, k, l;

   int linesize, put;

   if (!bmp) return 0;

   /* cria um novo arquivo binario */
   fopen_s(&filePtr, filename, "wb");
   assert(filePtr);

   /* a linha deve terminar em uma double word boundary */
   linesize = bmp->width * 3;
   if (linesize & 3) {
      linesize |= 3;
      linesize ++;
   }

   /* calcula o tamanho do arquivo em bytes */
   bfSize = 14 +                     /* file header size */
      40 +                     /* info header size */
      bmp->height * linesize;       /* image data  size */

   /* Preenche o cabe�alho -> FileHeader e InfoHeader */
   putuint(19778, filePtr);              /* type = "BM" = 19788                             */
   putdword(filePtr, bfSize);            /* bfSize -> file size in bytes                    */
   putuint(0, filePtr);                  /* bfReserved1, must be zero                       */
   putuint(0, filePtr);                  /* bfReserved2, must be zero                       */
   putdword(filePtr, 54);                /* bfOffBits -> offset in bits to data             */

   putdword(filePtr, 40);                /* biSize -> structure size in bytes                 */
   putlong(filePtr, bmp->width);         /* biWidth -> image width in pixels                  */         
   putlong(filePtr, bmp->height);        /* biHeight -> image height in pixels                */
   putword(filePtr, 1);                  /* biPlanes, must be 1                               */
   putword(filePtr, 24);                 /* biBitCount, 24 para 24 bits -> bitmap color depth */
   putdword(filePtr, 0);                 /* biCompression, compression type -> no compression */
   putdword(filePtr, 0);                 /* biSizeImage, nao eh usado sem compressao          */
   putlong(filePtr, 0);                  /* biXPelsPerMeter                                   */
   putlong(filePtr, 0);                  /* biYPelsPerMeter                                   */
   putdword(filePtr, 0);                 /* biClrUsed, numero de cores na palheta             */
   putdword(filePtr, 0);                 /* biClrImportant, 0 pq todas sao importantes        */

   /* aloca espacco para a area de trabalho */
   filedata = (unsigned char *) malloc(linesize);
   assert(filedata);

   /* a linha deve ser zero padded */
   for (i=0; i<(linesize-(3*bmp->width)); i++) 
      filedata[linesize-1-i] = 0;

   for (k=0; k<bmp->height;k++)
   {
      l = 1;
      /* coloca as componentes BGR no buffer */    
      for (i=0; i<bmp->width; i++) {
         unsigned char color[3];
         int r,g,b;
         imgGetPixel3ubv(bmp,i,k,color);
         r= color[0];
         g= color[1];
         b= color[2];
         filedata[l-1] = (unsigned char)(b<256) ? b : 255 ;
         filedata[l  ] = (unsigned char)(g<256) ? g : 255 ;
         filedata[l+1] = (unsigned char)(r<256) ? r : 255 ;
         l+=3;
      }

      /* joga para o arquivo */
      put = (int)fwrite(filedata, linesize, 1, filePtr);
      if (put != 1) {
         fprintf(stderr, "put24bits: Disk full.");
         free(filedata);
         return 0;
      }
   }

   /* operacao executada com sucesso */
   fprintf(stdout,"imgWriteBMP: %s successfuly generated\n",filename);
   free(filedata);
   fclose(filePtr);
   return 1;
}

Image* imgRead(char* filename)
{
  FILE * fp = NULL;
  fopen_s(&fp, filename, "rb");
  Image* img;
  int w,h,dcs;
 
  if (fp == NULL) {
    fprintf(stdout,"*** Cannot open file %s\n",filename);
    return 0;
  }

  /* the .img file header */
  fread(&w,sizeof(int),1,fp);  /* width = number of samples in x direction */
  fread(&h,sizeof(int),1,fp); /* height = number of samples in y direction */
  fread(&dcs,sizeof(int),1,fp);    /* dcs = depth of color space = number of chanels (floats) per pixel */

  img = imgCreate(w,h,dcs);
  if (img==NULL) {
	  fprintf(stdout,"Cannot write a %dx%dx%d image\n",w,h,dcs);
	  return NULL;
  }

  fread( img->buf, sizeof(float), img->dcs*img->width*img->height, fp );  /* from lower left pixel up to upper wright, by rows */

  fprintf(stdout,"imgRead: %s successfuly loaded\n",filename);
  fclose(fp);
  return img;
}

int imgWrite(char * filename, Image* img)
{
  FILE * fp = NULL;
  fopen_s(&fp, filename, "wb");
 
  if (fp == NULL) {
    fprintf(stdout,"*** Cannot open file %s\n",filename);
    return 0;
  }

  /* the .img file header */
  fwrite(&img->width,sizeof(int),1,fp);  /* width = number of samples in x direction */
  fwrite(&img->height,sizeof(int),1,fp); /* height = number of samples in y direction */
  fwrite(&img->dcs,sizeof(int),1,fp);    /* dcs = depth of color space = number of chanels (floats) per pixel */

  /* the image data file */
  fwrite( img->buf, img->dcs*img->width*img->height, sizeof(float), fp );  /* from lower left pixel up to upper wright, by rows */

  fprintf(stdout,"imgWriteIMG: %s successfuly created\n",filename);
  fclose(fp);
  return 1;
}





static int comparaCor3(const void * p1, const void * p2)
{
   int *c1 = (int *) p1;  /* aponta para o vermelho quantizado da cor 1 */
   int *c2 = (int *) p2;  /* aponta para o vermelho quantizado da cor 2 */

   /* compara o canal vermelho */
   if (*c1 < *c2) return -1;
   if (*c1 > *c2) return  1;

   /* compara  o canal verde, uma vez que o vermelho e' igual */
   c1++; c2++;
   if (*c1 < *c2) return -1;
   if (*c1 > *c2) return  1;

   /* compara o canal azul, uma vez que o vermelho e o azul sao iguais */
   c1++; c2++;
   if (*c1 < *c2) return -1;
   if (*c1 > *c2) return  1;

   /* sao iguais */
   return 0;
}

static int comparaCor1(const void * p1, const void * p2)
{
   int *c1 = (int *) p1;  /* aponta para a luminosidade quantizada da cor 1 */
   int *c2 = (int *) p2;  /* aponta para a luminosidade quantizada da cor 2 */

   /* compara o canal de luminosidade */
   if (*c1 < *c2) return -1;
   if (*c1 > *c2) return  1;

   /* sao iguais */
   return 0;
}

int imgCountColor(Image * img, float tol)
{
   int numCor = 1;
   int w = imgGetWidth(img);
   int h = imgGetHeight(img);
   int dcs = imgGetDimColorSpace(img);
   float* buf=imgGetData(img);
   int *vet=(int*) malloc(3*w*h*sizeof(int));
   int i;


   /* copia o buffer da imagem no vetor de floats fazendo 
    uma quantizacao para (1/tol) tons de cada componente de cor */
   for (i=0;i<dcs*w*h;i++) 
      vet[i]  = (int)(buf[i]/tol+0.5);

   /* ordena o vetor */
   if (dcs==3) 
      qsort(vet,w*h,3*sizeof(int),comparaCor3);
   else 
      qsort(vet,w*h,sizeof(int),comparaCor1);



   /* conta o numero de cores diferentes */
   if (dcs==3) {
	   int freq=1;
	   printf("%d %d %d ",vet[0],vet[1],vet[2]);
	   for (i=3; i<3*w*h; i+=3){
		  freq++;
		  if (comparaCor3(&vet[i-3],&vet[i])!=0) {
			  printf("%d\n %d %d %d ",freq,vet[i],vet[i+1],vet[i+2]);
			  freq=1;
			  numCor++;
		  }
	   }
		  printf("%d\n",freq);
   } else {
      for (i=1; i<w*h; i+=1)
         if (comparaCor1(&vet[i-1],&vet[i])!=0) numCor++;
   }


   free(vet);
   return numCor;
}




static float apply(float c[9], float v[9]) {
    return c[0]*v[0]+c[1]*v[1]+c[2]*v[2]+c[3]*v[3]+c[4]*v[4]+c[5]*v[5]+c[6]*v[6]+c[7]*v[7]+c[8]*v[8];
}

static float applyV(float c[9], float v[3*9], float out[3]) {
    out[0]= c[0]*v[0*3  ]+c[1]*v[1*3  ]+c[2]*v[2*3  ]+c[3]*v[3*3  ]+c[4]*v[4*3  ]+c[5]*v[5*3  ]+c[6]*v[6*3  ]+c[7]*v[7*3  ]+c[8]*v[8*3  ];
    out[0]= c[0]*v[0*3+1]+c[1]*v[1*3+1]+c[2]*v[2*3+1]+c[3]*v[3*3+1]+c[4]*v[4*3+1]+c[5]*v[5*3+1]+c[6]*v[6*3+1]+c[7]*v[7*3+1]+c[8]*v[8*3+1];
    out[0]= c[0]*v[0*3+2]+c[1]*v[1*3+2]+c[2]*v[2*3+2]+c[3]*v[3*3+2]+c[4]*v[4*3+2]+c[5]*v[5*3+2]+c[6]*v[6*3+2]+c[7]*v[7*3+2]+c[8]*v[8*3+2];
}

static float gauss[9]={1.f/16, 2.f/16, 1.f/16,  2.f/16, 4.f/16, 2.f/16,  1.f/16, 2.f/16, 1.f/16 };
static float sobel_x[9]={-1, 0, 1,  -2, 0, 2,  -1, 0, 1 };
static float sobel_y[9]={1, 2, 1,  0, 0, 0,  -1, -2, -1 };


void imgGauss(Image* img_dst, Image* img_src) 
{
    int w   = imgGetWidth(img_dst);
    int h   = imgGetHeight(img_dst);
    int dcs = imgGetDimColorSpace(img_dst);

    float* src_buffer = imgGetData(img_src);
    float* dst_buffer = imgGetData(img_dst);
    int x,y;


    if (dcs==1) {
        for (y=1;y<h-1;y++){
            for (x=1;x<w-1;x++) {
                int k = y*w+x;
                float v[9] = {src_buffer[k+w-1], src_buffer[k+w],src_buffer[k+w+1],
                    src_buffer[k-1],src_buffer[k],src_buffer[k+1],
                    src_buffer[k-w-1],src_buffer[k-w],src_buffer[k-w+1]};
                    dst_buffer[k] = apply(gauss,v);
            }
        }
    } else {
        for (y=1;y<h-1;y+=1){
            for (x=1;x<w-1;x+=1) {
                int k0 = y*w*3+x*3;
                int k1 = k0+1;
                int k2 = k0+2;
                float r[9] = {	src_buffer[k0+3*w-3],src_buffer[k0+3*w],src_buffer[k0+3*w+3],
								src_buffer[k0-3]    ,src_buffer[k0]    ,src_buffer[k0+3],
								src_buffer[k0-3*w-3],src_buffer[k0-3*w],src_buffer[k0-3*w+3]};
                float g[9] = {src_buffer[k1+3*w-3],src_buffer[k1+3*w],src_buffer[k1+3*w+3],
                    src_buffer[k1-3]    ,src_buffer[k1]    ,src_buffer[k1+3],
                    src_buffer[k1-3*w-3],src_buffer[k1-3*w],src_buffer[k1-3*w+3]};
                float b[9] = {src_buffer[k2+3*w-3],src_buffer[k2+3*w],src_buffer[k2+3*w+3],
                    src_buffer[k2-3]    ,src_buffer[k2]    ,src_buffer[k2+3],
                    src_buffer[k2-3*w-3],src_buffer[k2-3*w],src_buffer[k2-3*w+3]};

                dst_buffer[k0] = apply(gauss,r);
                dst_buffer[k1] = apply(gauss,g);
                dst_buffer[k2] = apply(gauss,b);
            }
        }
    }
}



/*
* The following routines have been built from knowledge gathered
* around the Web. I am not aware of any copyright problem with
* them, so use it as you want.
* N. Devillard - 1998
*/
typedef float pixelvalue ;
#define PIX_SORT(a,b) { if ((a)>(b)) PIX_SWAP((a),(b)); }
#define PIX_SWAP(a,b) { pixelvalue temp=(a);(a)=(b);(b)=temp; }

/*----------------------------------------------------------------------------
Function : opt_med9()
In : pointer to an array of 9 pixelvalues
Out : a pixelvalue
Job : optimized search of the median of 9 pixelvalues
Notice : in theory, cannot go faster without assumptions on the
signal.
Formula from:
XILINX XCELL magazine, vol. 23 by John L. Smith
The input array is modified in the process
The result array is guaranteed to contain the median
value
in middle position, but other elements are NOT sorted.
---------------------------------------------------------------------------*/
static pixelvalue opt_med9(pixelvalue * p)
{
    PIX_SORT(p[1], p[2]) ; PIX_SORT(p[4], p[5]) ; PIX_SORT(p[7], p[8]) ;
    PIX_SORT(p[0], p[1]) ; PIX_SORT(p[3], p[4]) ; PIX_SORT(p[6], p[7]) ;
    PIX_SORT(p[1], p[2]) ; PIX_SORT(p[4], p[5]) ; PIX_SORT(p[7], p[8]) ;
    PIX_SORT(p[0], p[3]) ; PIX_SORT(p[5], p[8]) ; PIX_SORT(p[4], p[7]) ;
    PIX_SORT(p[3], p[6]) ; PIX_SORT(p[1], p[4]) ; PIX_SORT(p[2], p[5]) ;
    PIX_SORT(p[4], p[7]) ; PIX_SORT(p[4], p[2]) ; PIX_SORT(p[6], p[4]) ;
    PIX_SORT(p[4], p[2]) ; return(p[4]) ;
}


#undef PIX_SORT
#undef PIX_SWAP


void imgMedian(Image* image) 
{
    int w = imgGetWidth(image);
    int h = imgGetHeight(image);
    int dcs = imgGetDimColorSpace(image);
    Image* img = imgCopy(image);  
    float* image_buf = imgGetData(image);
    float* img_buf = imgGetData(img);
    int x,y;

    if (dcs==1) {
        for (y=1;y<h-1;y++){
            for (x=1;x<w-1;x++) {
                int k = y*w+x;
                float v[9] = {img_buf[k+w-1], img_buf[k+w],img_buf[k+w+1],
                    img_buf[k-1],img_buf[k],img_buf[k+1],
                    img_buf[k-w-1],img_buf[k-w],img_buf[k-w+1]};
                image_buf[k]= opt_med9(v);
            }
        }
    } else {
        for (y=1;y<h-1;y+=1){
            for (x=1;x<w-1;x+=1) {
                int k0 = y*w*3+x*3;
                int k1 = k0+1;
                int k2 = k0+2;
                float r[9] = {img_buf[k0+3*w-3],img_buf[k0+3*w],img_buf[k0+3*w+3],
                    img_buf[k0-3]    ,img_buf[k0]    ,img_buf[k0+3],
                    img_buf[k0-3*w-3],img_buf[k0-3*w],img_buf[k0-3*w+3]};
                float g[9] = {img_buf[k1+3*w-3],img_buf[k1+3*w],img_buf[k1+3*w+3],
                    img_buf[k1-3]    ,img_buf[k1]    ,img_buf[k1+3],
                    img_buf[k1-3*w-3],img_buf[k1-3*w],img_buf[k1-3*w+3]};
                float b[9] = {img_buf[k2+3*w-3],img_buf[k2+3*w],img_buf[k2+3*w+3],
                    img_buf[k2-3]    ,img_buf[k2]    ,img_buf[k2+3],
                    img_buf[k2-3*w-3],img_buf[k2-3*w],img_buf[k2-3*w+3]};
                image_buf[k0] = opt_med9(r);
                image_buf[k1] = opt_med9(g);
                image_buf[k2] = opt_med9(b);
            }
        }
    }
    imgDestroy(img);
}

Image* imgEdges(Image* imgIn)
{
    int w = imgGetWidth(imgIn);
    int h = imgGetHeight(imgIn);
    int dcs = imgGetDimColorSpace(imgIn);
    Image* imgOut = imgCreate(w,h,1);
    int x,y;
    Image* img;
    float max=0,inv;

    float* imgOut_buf = imgGetData(imgOut);

    float* img_buf;
    if (dcs==1) 
        img = imgIn;
    else 
        img = imgGrey(imgIn);
    img_buf=imgGetData(img);

    for (y=1;y<h-1;y++){
        for (x=1;x<w-1;x++) {
            int k = y*w+x;
            float v[9] = {img_buf[k+w-1], img_buf[k+w],img_buf[k+w+1],
                img_buf[k-1],img_buf[k],img_buf[k+1],
                img_buf[k-w-1],img_buf[k-w],img_buf[k-w+1]};
            float dx = apply(sobel_x,v);
            float dy = apply(sobel_y,v);
            float val = (float)sqrt(dx*dx+dy*dy);
            max = (max>val)?max:val;
            imgOut_buf[k] = val;
        }
    }

    inv = (max==0)? 1.f : 1.f/max;
    /* arruma a imagem */
    for (y=0;y<h;y++) {
        for (x=0;x<w;x++) {
            int k=y*w+x;
            if ((y==0)||(y==(h-1))||(x==0)||(x==(w-1))) /* borda */
                imgOut_buf[x]=0.f;
            else
                imgOut_buf[x]/=inv;
        }
    }
    return imgOut;
}

/* implementacao da reducao de cores por median cut */
typedef struct
{
    float r, g, b;
} color;

typedef struct
{
    color max;     /* maximos R's, G's e B's */
    color min;	   /* minimos R's, G's e B's */
    float var;	   /* variacao em cada cubo  */
    int ini;	   /* final do cubo  */
    int fim;       /* inicio do cubo */
    int maiorDim;  /* R=0, G=1, B=2  */
    int checked;
} colorCube;

static int comparaR(const void * p1, const void * p2)
{
    color *color1 = (color *) p1;
    color *color2 = (color *) p2;

    color c1 = *color1;
    color c2 = *color2;

    if (c1.r < c2.r) return -1;
    if (c1.r > c2.r) return  1;

    return 0;
}

static int comparaG(const void * p1, const void * p2)
{
    color *color1 = (color *) p1;
    color *color2 = (color *) p2;

    color c1 = *color1;
    color c2 = *color2;

    if (c1.g < c2.g) return -1;
    if (c1.g > c2.g) return  1;

    return 0;
}

static int comparaB(const void * p1, const void * p2)
{
    color *color1 = (color *) p1;
    color *color2 = (color *) p2;

    color c1 = *color1;
    color c2 = *color2;

    if (c1.b < c2.b) return -1;
    if (c1.b > c2.b) return  1;

    return 0;
}

static void caixaEnvolvente(colorCube* cube, color* colorVec)
{
    float r,g,b;
    int i; 

    /* inicializa com o pior caso */
    cube->min.r = cube->min.g = cube->min.b = 1.0; 
    cube->max.r = cube->max.g = cube->max.b = 0.0;

    /* percorre o cubo ajustando o dom�nio das cores */
    for (i=cube->ini;i<=cube->fim;i++){
        r = colorVec[i].r;
        if (r > cube->max.r) cube->max.r = r;
        if (r < cube->min.r) cube->min.r = r;
        g = colorVec[i].g;
        if (g > cube->max.g) cube->max.g = g;
        if (g < cube->min.g) cube->min.g = g;
        b = colorVec[i].b;
        if (b > cube->max.b) cube->max.b = b;
        if (b < cube->min.b) cube->min.b = b;
    }
}

static Image* bestColor(Image *img0, color* pal, Image* img1, int pal_size)
{
    int w = imgGetWidth(img0);
    int h = imgGetHeight(img0);
    float rgb[3];
    int x,y,i;

    for (y=0;y<h;y++){
        for (x=0;x<w;x++){
            int i_menor;
            float m_menor;
            float rgb1[3];

            imgGetPixel3fv(img0,x,y,rgb);

            m_menor = FLT_MAX;
            i_menor = 0;

            for(i=0;i<pal_size;i++)
            {
                float dif[3];
                float m;

                dif[0] = rgb[0] - pal[i].r;
                dif[1] = rgb[1] - pal[i].g;
                dif[2] = rgb[2] - pal[i].b;

                m = (float)((dif[0]*dif[0]) + (dif[1]*dif[1]) + (dif[2]*dif[2]));

                if (m < m_menor)
                {
                    i_menor = i;
                    m_menor = m;

                    rgb1[0]=pal[i_menor].r;
                    rgb1[1]=pal[i_menor].g;
                    rgb1[2]=pal[i_menor].b;
                }
            }

            imgSetPixel3fv(img1,x,y,rgb1);
        }
    }
    return img1;
}

static void paleta(color* pal, colorCube* cubeVec, color* colorVec, int numCubos)
{
    float sumvar[3];
    int count;
    int i,j;

    for(i=0;i<numCubos;i++)
    {
        sumvar[0] = sumvar[1] = sumvar[2] = 0.0;
        count = 0;

        for(j=cubeVec[i].ini;j<=cubeVec[i].fim;j++)
        {
            sumvar[0] += colorVec[j].r;
            sumvar[1] += colorVec[j].g;
            sumvar[2] += colorVec[j].b;
            count++;
        }

        pal[i].r = sumvar[0]/count;
        pal[i].g = sumvar[1]/count;
        pal[i].b = sumvar[2]/count;
    }
}

static void ordenaMaiorDim(colorCube* cubeVec, color* colorVec, int i)
{
    size_t numElem;
    size_t tamElem;
    float var[3];

    numElem = (size_t)(cubeVec[i].fim - cubeVec[i].ini + 1);
    tamElem = (size_t)(sizeof(color));

    var[0] = cubeVec[i].max.r - cubeVec[i].min.r;
    var[1] = cubeVec[i].max.g - cubeVec[i].min.g;
    var[2] = cubeVec[i].max.b - cubeVec[i].min.b;

    /* procura a maior dimensao em um cubo */
    if((var[0]>=var[1])&&(var[0]>=var[2])) cubeVec[i].maiorDim = 0;
    else
        if((var[1]>=var[0])&&(var[1]>=var[2])) cubeVec[i].maiorDim = 1;
        else
            if((var[2]>var[0]) && (var[2]>var[1])) cubeVec[i].maiorDim = 2;

    /* ordena de acordo com a maior dimensao da caixa */
    if(cubeVec[i].maiorDim == 0)
    {
        qsort(&colorVec[cubeVec[i].ini],numElem,tamElem,comparaR);
        cubeVec[i].var = var[0];
    }
    if(cubeVec[i].maiorDim == 1)
    {
        qsort(&colorVec[cubeVec[i].ini],numElem,tamElem,comparaG);
        cubeVec[i].var = var[1];
    }
    if(cubeVec[i].maiorDim == 2)
    {
        qsort(&colorVec[cubeVec[i].ini],numElem,tamElem,comparaB);
        cubeVec[i].var = var[2];
    }
}


static void cortaCubo(colorCube* cubeVec, int posCorte, int numCubos)
{
    /* divide o cubo */
    int ini = cubeVec[posCorte].ini;
    int fim = cubeVec[posCorte].fim;

    if((fim - ini)%2 != 0) /* numero par de elementos */
    {
        cubeVec[numCubos].ini = ini + (fim - ini + 1)/2;
        cubeVec[numCubos].fim = fim;
        cubeVec[posCorte].fim = cubeVec[numCubos].ini - 1;
    }
    else /* numero impar de elementos */
    {
        cubeVec[numCubos].ini = ini + (fim - ini)/2;
        cubeVec[numCubos].fim = fim;
        cubeVec[posCorte].fim = cubeVec[numCubos].ini - 1;
    }
}

static int cuboCorte(colorCube* cubeVec, int numCubos)
{
    /* escolhe o cubo a ser cortado */
    float maiorVar = -1;
    int posCorte = -1;
    int k;
    for(k=0;k<numCubos;k++)
    {
        /* cubo com uma unica cor */
        if((cubeVec[k].max.r == cubeVec[k].min.r)&&(cubeVec[k].max.g == cubeVec[k].min.g)&&(cubeVec[k].max.b == cubeVec[k].min.b))
            continue;

        if(cubeVec[k].var>maiorVar)
        {
            maiorVar = cubeVec[k].var;
            posCorte = k;
        }
    }

    return posCorte;
}


void imgReduceColors(Image * img0, Image* img1, int maxCores)
{
    int w = imgGetWidth(img0);
    int h = imgGetHeight(img0);

    int x,y,i,j,numCubos = 0;
    int posCorte = -1;
    float rgb[3];

    colorCube* cubeVec = (colorCube*)malloc(maxCores*sizeof(colorCube)); /* vetor de cubos */
    color* colorVec = (color*)malloc(w*h*sizeof(color)); /* vetor  de cores */
    color* pal = (color*)malloc(maxCores*sizeof(color)); /* paleta de cores */

    /* guarda as cores nos vetores (com repeticao) */
    i = 0;
    for (y=0;y<h;y++){
        for (x=0;x<w;x++){
            imgGetPixel3fv(img0,x,y,rgb);
            colorVec[i].r = rgb[0];
            colorVec[i].g = rgb[1];
            colorVec[i].b = rgb[2];
            i++;
        }
    }

    /* cria o cubo inicial */
    cubeVec[0].checked = 0;
    cubeVec[0].fim = w*h-1;
    cubeVec[0].ini = 0;

    numCubos = 1;

    for(j=0;j<maxCores-1;j++)
    {
        for(i=0;i<numCubos;i++)
        {
            if(cubeVec[i].checked) continue;

            /* calcula os maximos e os minimos */
            caixaEnvolvente(&cubeVec[i], colorVec);

            /* ordena de acordo com a maior dimensao */
            ordenaMaiorDim(cubeVec, colorVec, i);

            cubeVec[i].checked = 1;
        }

        /* escolhe o cubo a ser cortado */
        posCorte = cuboCorte(cubeVec, numCubos);

        if(posCorte == -1) break;

        /* divide o cubo */
        cortaCubo(cubeVec, posCorte, numCubos);

        cubeVec[numCubos].checked = 0;
        cubeVec[posCorte].checked = 0;

        numCubos++;
    }

    /* cria a paleta de cores */
    paleta(pal, cubeVec, colorVec, numCubos);

    /* preenche a imagem com as cores da paleta */
    bestColor(img0,&pal[0],img1,maxCores);

    free(colorVec);
    free(cubeVec);
    free(pal);

    printf("fim\n");

    return;
}


static float  MAX_LUM[] = { 1.0, 1.0, 1.0 };
static float  MIN_LUM[] = { 0.0, 0.0, 0.0 };

/******************************************************************************************************/
/* A partir de uma imagem, calcula o histograma correspondente ao retangulo delimitado pelos pontos
/*   (x0, y0) e (x1, y1), onde x1 > x0 e y1 > y0                                                   
/******************************************************************************************************/
static void CalculateGreyHistogram( int histograma[], Image* greyImage, int x0, int y0, int x1, int y1 )
{
	int   i, x, y;
	float cor[3];
	int   luminosidade;

	for( i = 0; i < N_CORES; i++ )
		histograma[i] = 0;

	for( y = y0; y < y1; y++ ) 
	{
		for( x = x0; x < x1; x++ )
		{
			imgGetPixel3fv( greyImage, x, y, cor );
			luminosidade = (int) ( cor[0] * MAIOR_COR );

			histograma[luminosidade]++;
		}
	}
}

/******************************************************************************************************/
/* Calcula o valor medio de um histograma, usando uma media ponderada                                 */
/******************************************************************************************************/
static int GetMiddleValue( int histograma[] )
{
	int soma   = 0;
	int pontos = 0;
	int i;

	for( i = 0; i < N_CORES; i++ )
	{
		soma   += i * histograma[i];
		pontos += histograma[i];
	}

	return soma / pontos;
}

/******************************************************************************************************/
/* Baseado no artigo "A Threshold Selection Method from Gray-Level Histograms",
/*   de N. Otsu, dispon�vel em http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=04310076
/* Artigo citado em "Automatic Recognition Algorithm of Quick Response Code Based on Embedded System",
/*   de Y. Liu e M. Liu, dispon�vel em http://portal.acm.org/citation.cfm?id=1173502
/******************************************************************************************************/
Image* imgBinOtsu( Image* greyImage )
{
	float  probabilidade[N_CORES], omega[N_CORES], mu[N_CORES];
	int    histograma[N_CORES];
	int    w, h, numberOfPixels;
	int    threshold;
	float  thresholdF;
	float  cor[3];
	int    i, x, y;
	float  maxSigma, sigma, numerador, quociente;
	Image* imgFinal;

	/* Calculando o histograma */
	w = imgGetWidth( greyImage );
	h = imgGetHeight( greyImage );
	CalculateGreyHistogram( histograma, greyImage, 0, 0, w, h );

	/* Calculando a probabilidade (eq. 1) */
	numberOfPixels = w * h;
	for( i = 0; i < N_CORES; i++ )
		probabilidade[i] =  (float) histograma[i] / (float) numberOfPixels;

	/* Calculando omega e mu (eqs. 6 e 7)  */
	omega[0] = probabilidade[0];
	mu[0]    = 0.0;
	for( i = 1; i < N_CORES; i++ )
	{
		omega[i] = omega[i-1] + probabilidade[i];
		mu[i]    = mu[i-1] + (i * probabilidade[i]);
	}

	/* Calculando e achando o maior sigma (eq. 18) */
	threshold = 0;
	maxSigma  = 0.0;
	for( i = 0; i < MAIOR_COR; i++ ) {
		sigma = 0.0;

		if( omega[i] >= 0.0 && omega[i] <= 1.0 )
		{
			numerador = mu[MAIOR_COR] * omega[i] - mu[i];
			quociente = omega[i] * (1.0f - omega[i]);
			sigma = (numerador * numerador) / quociente;
		}

		if( sigma > maxSigma )
		{
			maxSigma  = sigma;
			threshold = i;
		}
	}

	/* Criando a nova imagem  */
	imgFinal   = imgCreate(w, h, 3);
	thresholdF = (float) threshold / (float) MAIOR_COR;
	for( y = 0; y < h; y++ ) 
	{
		for( x = 0; x < w; x++ )
		{
			imgGetPixel3fv( greyImage, x, y, cor );

			if( cor[0] > thresholdF )
				imgSetPixel3fv( imgFinal, x, y, MAX_LUM );
			else
				imgSetPixel3fv( imgFinal, x, y, MIN_LUM );
		}
	}

	return imgFinal;
}

/******************************************************************************************************/
/* Baseado no artigo "Barcode Readers using the Camera Device in Mobile Phones", de E. Ohbuchi,       */
/*   dispon�vel em http://portal.acm.org/citation.cfm?id=1033123                                      */
/* Artigo citado em "Automatic Recognition Algorithm of Quick Response Code Based on Embedded System",*/
/* de Y. Liu e M. Liu, dispon�vel em http://portal.acm.org/citation.cfm?id=1173502                    */
/******************************************************************************************************/
Image* imgBinOhbuchi( Image* greyImage )
{
	int    i, j;
	int    x, y, x0;
	int    w, h;
	int    tamanhoQuadradoCentral, tamanhoBloco;
	int    thresholdGlobal, valorMedio;
	float  thresholdGlobalF;
	Image* imgFinal;
	int    histograma[256];
	float  cor[3];

	/* Como o algoritmo original de Ohbuchi considera o quadrado central de dimensoes 60x60   */
	/*   para uma imagem de tamanho 320x240, para generalizar o algoritmo, pega-se o quadrado */
	/*   central cujas dimens�es sejam 1/4 da menor dimens�o da imagem.                       */
	w = imgGetWidth( greyImage );
	h = imgGetHeight( greyImage );
	if( w > h )
		tamanhoQuadradoCentral = h;
	else
		tamanhoQuadradoCentral = w;

	tamanhoQuadradoCentral  = tamanhoQuadradoCentral / 4; /* tamanho da area central  */

	/* Tendo o quadrado central, cada um dos 9 blocos a serem analisados tem dimensao igual a 1/3 */
	/*   do quadrado central.                */                                                  
	tamanhoBloco = tamanhoQuadradoCentral / 3; /* tamanho de cada bloco */

	/* Percorre-se os 9 blocos, montando o histograma para cada um deles e calculando a media.
	/* A menor das medias sera o histograma global da imagem  */
	thresholdGlobal = MAIOR_COR + 1;         /*  inicia a variavel com o maior valor  */
		x0 = ( w - tamanhoQuadradoCentral ) / 2; /* ponto inicial do 1o bloco na coordenada X */
	y  = ( h - tamanhoQuadradoCentral ) / 2; /* ponto inicial do 1o bloco na coordenada Y  */
	for( i = 0; i < 3; i++ )                 /* percorrendo na coordenada Y, incrementando y com tamanhoBloco a cada itera��o */
	{
		x = x0;                              /* x inicia sempre no valor menor  */
		for( j = 0; j < 3; j++ )             /* percorrendo na coordenada X, incrementando x com tamanhoBloco a cada itera��o  */
		{
			CalculateGreyHistogram( histograma, greyImage, x, y, x + tamanhoBloco, y + tamanhoBloco );
			valorMedio = GetMiddleValue( histograma );

			if( valorMedio < thresholdGlobal )
				thresholdGlobal = valorMedio;

			x += tamanhoBloco;
		}
		y += tamanhoBloco;
	}

	/* Tendo o valor threshold, cria-se a nova imagem, resultante da binarizacao */
	imgFinal = imgCreate(w, h, 3);
	thresholdGlobalF = (float) thresholdGlobal / (float) MAIOR_COR;
	for( y = 0; y < h; y++ ) 
	{
		for( x = 0; x < w; x++ )
		{
			imgGetPixel3fv( greyImage, x, y, cor );

			if( cor[0] > thresholdGlobalF )
				imgSetPixel3fv( imgFinal, x, y, MAX_LUM );
			else
				imgSetPixel3fv( imgFinal, x, y, MIN_LUM );
		}
	}

	return imgFinal;
}


