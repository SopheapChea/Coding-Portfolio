/*
Sopheap Sok
8/30/18
Program implement Canny's edge detection. 

Compile:
gcc canny.c -lm

Run:
./a.out garb34.pgm mag.pgm peaks.pgm final.pgm 0.047 1

*/



#include <stdio.h>                  
#include <math.h>
#include <stdlib.h>

#define  PICSIZE 256
#define  MAXMASK 100
#define  ON 255
#define  OFF 0

double ival[PICSIZE][PICSIZE];


void printpic(double ival[PICSIZE][PICSIZE], double maxival, char *output, int megflag);
void magnitude(int mr, double xoutpic1[PICSIZE][PICSIZE], double youtpic1[PICSIZE][PICSIZE], char **argv);
void peaks(double xconv[PICSIZE][PICSIZE], double yconv[PICSIZE][PICSIZE], double ival[PICSIZE][PICSIZE], char **argv, int mr);
int automaticHI(char **argv);
void doubleThresholds(double peak[PICSIZE][PICSIZE], int high, double low, char **argv);




void main(int argc, char **argv)
{
	int pic[PICSIZE][PICSIZE];
    double xoutpic1[PICSIZE][PICSIZE], youtpic1[PICSIZE][PICSIZE];
    double xmask[MAXMASK][MAXMASK], ymask[MAXMASK][MAXMASK];
    double xconv[PICSIZE][PICSIZE], yconv[PICSIZE][PICSIZE];
    int i,j,p,q,s,t,mr,centx,centy, header;
    double xsum, ysum,sig,minival, xmaskvalue, ymaskvalue;
    FILE *fo1, *fo2,*fp1, *fopen();
    char *foobar;
	
	// Make sure the right syntax is inputed on the command line.
	if (argc < 7)
	{
		fprintf(stderr, "Proper syntax: %s <input file name> <magnitude output file> <peaks output file> <final output file> <threshhold percent> <value of sig>\n", argv[0]);
		return;
	}
	
    foobar = argv[1];
    fp1=fopen(foobar,"rb");
	
	// Check if input file exist and is not empty.
	if (fp1 == NULL)
	{
		printf("Can't open %s\n", argv[1]);
		return;
	}

    foobar = argv[6];
    sig = atof(foobar);

    mr = (int)(sig * 3);
    centx = (MAXMASK / 2);
    centy = (MAXMASK / 2);
	
	
	// Avoid scaning in header of input file to 2D array.
	for (i=0; i<15; i++)
		header = getc(fp1);

    for (i=0;i<256;i++)
    { 
		for (j=0;j<256;j++)
			pic[i][j]  =  getc (fp1);
    }
	
	fclose(fp1);
	
	// Perform first derivatives for both x and y axis.
    for (p=-mr;p<=mr;p++)
    {  
		for (q=-mr;q<=mr;q++)
        {
			xmaskvalue = q*(exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
			ymaskvalue = p*(exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
            
			xmask[p+centy][q+centx] = xmaskvalue;
			ymask[p+centy][q+centx] = ymaskvalue;
        }
    }
	// Perform convolution
    for (i=mr;i<=255-mr;i++)
    { 
		for (j=mr;j<=255-mr;j++)
        {
            xsum = 0;
			ysum = 0;
			
            for (p=-mr;p<=mr;p++)
            {
				for (q=-mr;q<=mr;q++)
                {
                   xsum += pic[i+p][j+q] * xmask[p+centy][q+centx];
                   ysum += pic[i+p][j+q] * ymask[p+centy][q+centx];
                }
            }
            xoutpic1[i][j] = xsum;
			youtpic1[i][j] = ysum;
			
            xconv[i][j] = xsum;
            yconv[i][j] = ysum;
        }
    }
	
	
	// Call function to calculate the magnitude
	magnitude(mr, xoutpic1, youtpic1, argv);
	
	// Call function to find peaks.
	peaks(xconv, yconv, ival, argv, mr);

}

// Magnitude calcultions from sobel
void magnitude(int mr, double xoutpic1[PICSIZE][PICSIZE], double youtpic1[PICSIZE][PICSIZE], char **argv)
{
   double maxival = 0;
   int i, j;
   
   for (i=mr;i<256-mr;i++)
   { 
		for (j=mr;j<256-mr;j++)
        {
			ival[i][j]= sqrt((double)((xoutpic1[i][j]*xoutpic1[i][j]) + (youtpic1[i][j]*youtpic1[i][j])));
            
			if (ival[i][j] > maxival)
                maxival = ival[i][j];
        }
    }
	
	// Call print function for each print image.
	printpic(ival, maxival, argv[2], 1);
	
	return;
	
}

// Function finds peaks using Canny's algorithm.
void peaks(double xconv[PICSIZE][PICSIZE], double yconv[PICSIZE][PICSIZE], double ival[PICSIZE][PICSIZE], char **argv, int mr)
{
	double edgeflag[PICSIZE][PICSIZE];
	int i, j, high;
	double slope, low;
	
	for (i=mr; i<256-mr; i++)
	{
		for (j=mr; j<256-mr; j++)
		{
			if (xconv[i][j] == 0.0)
				xconv[i][j] = 0.00001;
			
			slope = yconv[i][j]/xconv[i][j];
			
			if ((slope <= 0.4142) && (slope > -0.4142))
			{
				if ((ival[i][j] > ival[i][j-1]) && (ival[i][j] > ival[i][j+1]))
					edgeflag[i][j] = ON;
			}
			
			else if ((slope <= 2.4142) && (slope > 0.4142))
			{
				if ((ival[i][j] > ival[i-1][j-1]) && (ival[i][j] > ival[i+1][j+1]))
					edgeflag[i][j] = ON;
			}
			
			else if ((slope <= -0.4142) && (slope > -2.4142))
			{
				if ((ival[i][j] > ival[i+1][j-1]) && (ival[i][j] > ival[i-1][j+1]))
					edgeflag[i][j] = ON;
			}
			
			else 
			{
				if ((ival[i][j] > ival[i-1][j]) && (ival[i][j] > ival[i+1][j]))
					edgeflag[i][j] = ON;
			}
		}
	}
	
	// Print out peak image
	printpic(edgeflag, 0.0, argv[3], 0);
	
	// Call function to calculate high threshhold and calculate the low threshhold.
	high = automaticHI(argv);
	low = 0.35*high;
	
	// Call function to calculate the final edged image using the high and low threshholds.
	doubleThresholds(edgeflag, high, low, argv);
	return;
}

// Function calculate the high and low threshhold based on input percentage. 
int automaticHI(char **argv)
{
	double percent, cutoff, areaOfTop = 0;
	char *foobar;
	int i, j;
	
	double *histogram = calloc(256, sizeof(double));
	
	foobar = argv[5];
    percent = atof(foobar);
	
	cutoff = percent*(PICSIZE*PICSIZE);
	for (i=0;i<256;i++)
	{ 
		for (j=0;j<256;j++)
		{
			histogram[(int)ival[i][j]]++;
		}

	}
	
	for (i=256; i>1; i--)
	{
		areaOfTop += histogram[i];
		
		if (areaOfTop > cutoff)
			break;
	}

	printf("High = %d\n", i);
	printf("Low = %.2lf\n", 0.35*i);
	
	free(histogram);

	return i;
}

// Function take the peak image and the calculated high and low threshhold to produce the final outlined image. 
void doubleThresholds(double peak[PICSIZE][PICSIZE], int high, double low, char **argv)
{
	int i, j, moretodo, p, q;
	
	double final[PICSIZE][PICSIZE];
	
	for(i = 0; i < 256; i++)
		for(j = 0; j < 256; j++)
			final[i][j] = 0;
	
	for(i = 0; i < 256; i++)
    {
		for(j = 0; j < 256; j++)
		{
			if (peak[i][j] == ON)
			{
				if(ival[i][j] > high)
				{	
					peak[i][j] = OFF;
					final[i][j] = ON;
				}
				
				else if(ival[i][j] < low)
					peak[i][j] = final[i][j] = OFF;
			}
		}
	}
	
	moretodo = 1;
	while (moretodo)
	{
		moretodo = 0;
		
		for(i = 0; i < 256; i++)
		{
			for(j = 0; j < 256; j++)
			{
				if (peak[i][j] == ON)
				{
					for(p = -1; p <= 1; p++)
					{
						for(q = -1; q <= 1; q++)
						{
							if (final[i+p][j+q] == ON)
							{
								peak[i][j] = OFF;
								final[i][j] = ON;
								moretodo = 1;
							}
						}
					}
				}
			}
		}
		
	}
	
	printpic(final, 0.0, argv[4], 0);
	return;
}

// Output picture function.
void printpic(double print[PICSIZE][PICSIZE], double maxival, char *output, int megflag)
{
	FILE *fo1;
	int i, j;
	
	fo1=fopen(output,"wb");
	
	fprintf(fo1, "P5\n");
	fprintf(fo1, "%d %d\n", 256, 256);  
	fprintf(fo1, "255\n");
	
	for (i=0;i<256;i++)
	{ 
		for (j=0;j<256;j++)
		{
			if (megflag)
				print[i][j] = (print[i][j] / maxival) * 255; 
			
			fprintf(fo1,"%c",(char)((int)(print[i][j])));
		}
	}
	
	fclose(fo1);
	return;
}