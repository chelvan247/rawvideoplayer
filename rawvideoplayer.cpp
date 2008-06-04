/*****************************************************************************

File Name: 	<rawvideoplayer.cpp>

File Description:
RAW Video File Viewer

*****************************************************************************/

#include "yuvconverter.h"
#include "videoout.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>


char *infile1_name;
char *infile2_name;
FILE *infile1 = NULL;
FILE *infile2 = NULL;
int numseq = 0;
int bTwoFiles = 0;

int framewidth = 352;
int frameheight = 288;
int dispwidth = 0;
int dispheight = 0;
int inframesize;
int conv_mode = 0;
unsigned char *outputbuf;

extern float framerate;
extern int frameinterval;
extern int framenum;
extern unsigned char *rgba_buffer;


/*****************************************************************************

Routine: Init

        returns        	: Void 
  Description			: Initialization function called from main
*****************************************************************************/
void Init()
{
	/* User can write any Initialization code here */
	frameinterval = (int)(1000/framerate);
	if (dispwidth<=0) dispwidth = framewidth;
	if (dispheight<=0) dispheight = frameheight;
	outputbuf = (unsigned char *)malloc(framewidth*frameheight*3);
	
	if(outputbuf == NULL)
	{
		cout<<"\nAllocating Buffers failed! Not Enough Memory.";
		cout<<"\nExiting !!!";
		exit(2);
	}

}



/*****************************************************************************

Routine: GenOutputBuf

        returns        	: Void 
  Description			: Generate the output frame for display
*****************************************************************************/
void GenOutputBuf()
{
	/* User can write any code to generate the output here */
	/* Call GetRGBAFrame to convert the output to RGBA format */

	unsigned char *bufin;
	unsigned char *bufout;
	int width, height;

	bufin = outputbuf;
	bufout = rgba_buffer;
	width = framewidth;
	height = frameheight;

	fread(bufin, 1, inframesize, infile1);
	if(!feof(infile1))
	{
		GetRGBAFrame(bufin, bufout, width, height);
		if(bTwoFiles)
		{
			fread(bufin, 1, inframesize, infile2);
			if(!feof(infile2))
				GetRGBAFrame(bufin, (bufout+width*4), width, height);
		}

		framenum ++;
		frameinterval = (int)((float)1000*(framenum)/framerate)-(int)((float)1000*(framenum-1)/framerate);
	}

}



/*****************************************************************************

Routine: usage

        returns        	: void
  Description			: Output Message in case of Wrong Usage
*****************************************************************************/
void usage(char *str)
{
   printf("\n  Usage: ");
   printf("\n  rawvideoplayer [OPTION] [FILE1] [FILE2]");
   printf("\n  Play FILE on the screen");
   printf("\n");

   printf("\n  +-----------------------------------------------+");
   printf("\n  |  Options:                                     |");
   printf("\n  |      -w : Width of frame                      |");
   printf("\n  |      -h : Height of frame                     |");
   printf("\n  |      -l : Display Width                       |");
   printf("\n  |      -b : Display Height                      |");
   printf("\n  |      -f : Frame Rate                          |");
   printf("\n  |      -m : Input Mode                          |");
   printf("\n  +-----------------------------------------------+");
   printf("\n  |  Modes Supported                              |");
   printf("\n  +-----------------------------------------------+");
   printf("\n  |           Input Format           |    Mode    |");	
   printf("\n  +-----------------------------------------------+");
   printf("\n  |      YUV420 Prog Planar          |      0     |");
   printf("\n  |      YUV420 Int  Planar          |      1     |");
   printf("\n  |      YVU420 Prog Planar          |      2     |");
   printf("\n  |      YVU420 Int Planar           |      3     |");
   printf("\n  +-----------------------------------------------+");
   printf("\n  |      YUV422 Prog Planar          |     10     |");
   printf("\n  |      YUV422 Int Planar           |     11     |");
   printf("\n  |      YVU422 Prog Planar          |     12     |");
   printf("\n  |      YVU422 Int Planar           |     13     |");
   printf("\n  +-----------------------------------------------+");
   printf("\n  |      UYVY Prog                   |     20     |");
   printf("\n  |      UYVY Int                    |     21     |");
   printf("\n  |      YUYV Prog                   |     22     |");
   printf("\n  |      YUYV Int                    |     23     |");
   printf("\n  |      YVYU Prog                   |     24     |");
   printf("\n  |      YVYU Int                    |     25     |");
   printf("\n  +-----------------------------------------------+");
   printf("\n  |      YUV444 Prog Planar          |     30     |");
   printf("\n  |      YUV444 Int Planar           |     31     |");
   printf("\n  |      YUV444 Prog Planar          |     32     |");
   printf("\n  |      YUV444 Int Planar           |     33     |");
   printf("\n  |      YUV444 Prog Planar          |     34     |");
   printf("\n  |      YUV444 Int Planar           |     35     |");
   printf("\n  +-----------------------------------------------+");
   printf("\n  |      YUV444 Prog Packed          |     40     |");
   printf("\n  |      YUV444 Int Packed           |     41     |");
   printf("\n  |      YUV444 Prog Packed          |     42     |");
   printf("\n  |      YUV444 Int Packed           |     43     |");
   printf("\n  |      YUV444 Prog Packed          |     44     |");
   printf("\n  |      YUV444 Int Packed           |     45     |");
   printf("\n  +-----------------------------------------------+");
   printf("\n  |      RGB Prog Planar             |     50     |");
   printf("\n  |      RGB Prog Packed             |     51     |");
   printf("\n  |      BGR Prog Planar             |     52     |");
   printf("\n  |      BGR Prog Packed             |     53     |");
   printf("\n  +-----------------------------------------------+");
   printf("\n");

   exit(1);
}



/*****************************************************************************

Routine: ParseArgs

        returns        	: void
  Description			: Parse command line Arguments
*****************************************************************************/
void ParseArgs(int argc, char **argv)
{
	int i;

	for(i=1; i<argc; i++) {
    	if(argv[i][0] == '-')
		{
      		switch(argv[i++][1]) {
         		case 'w':
           			if (argv[i] == NULL)
               		usage(argv[0]);
           			framewidth = atoi(argv[i]);
           			break;
         		case 'h':
           			if (argv[i] == NULL)
            	   	usage(argv[0]);
           			frameheight = atoi(argv[i]);
           			break;
         		case 'f':
           			if (argv[i] == NULL)
               		usage(argv[0]);
           			framerate = atof(argv[i]);
           			break;
         		case 'm':
           			if (argv[i] == NULL)
               		usage(argv[0]);
           			conv_mode = atoi(argv[i]);
           			break;
         		case 'l':
           			if (argv[i] == NULL)
               		usage(argv[0]);
           			dispwidth = atoi(argv[i]);
           			break;
         		case 'b':
           			if (argv[i] == NULL)
               		usage(argv[0]);
           			dispheight = atoi(argv[i]);
           			break;
         		default:
           			usage(argv[0]);
      		}
    	}
		else
		{
			if(numseq==0)
			{
       			infile1_name = argv[i];
				numseq ++;
			}
			else
				{
					if(numseq==1)
					{
							infile2_name = argv[i];
							bTwoFiles = 1;
							numseq ++;
					}
					else
						{
							cout << "\nMaximum No. of sequences is 2";
							cout << "\nExiting !!!";
							exit(5);
						}
				}
    	}

  	}
}



/*****************************************************************************

Routine: main

        returns        	: int
  Description			: main() 
*****************************************************************************/
int main(int argc, char** argv) 
{


	if(argc<2)
		usage(argv[0]);

	ParseArgs(argc, argv);
    infile1 = fopen(infile1_name, "rb");
	if(bTwoFiles)
		infile2 = fopen(infile2_name, "rb");
	if(infile1 == NULL)
	{
		cout << "\nInput File Not found.";
		cout << "\nExiting !!!";
		exit(1);
	}
	if((bTwoFiles) && (infile2 == NULL))
	{
		cout << "\nInput File2 Not found.";
		cout << "\nExiting !!!";
		exit(1);
	}

	Init();
	InitConverter(framewidth, frameheight, conv_mode);

	/* Initialize VideoOutput and wait till exit */	
	VideoOutInit("RawVideoPlayer", argc, argv, (1+bTwoFiles)*framewidth, frameheight, (1+bTwoFiles)*dispwidth, dispheight);

	return 0;
}



