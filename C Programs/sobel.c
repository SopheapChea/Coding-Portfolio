/*
Sopheap Sok
8/26/2018
Modifide sobel program from Dr. Niels da Vitoria Lobo.
Program implements thresholding on the gradient magnitude. Program outputs pic showing magnitude of the gradient, pic with low threshold, and pic with high threshold.

Command prompts:
gcc sobel.c -lm
./a.out face05.pgm facemeg.pgm facelow.pgm facehigh.pgm
*/

#include <stdio.h> 
#include <stdlib.h>                         
#include <math.h>

void printpic(double ival[256][256], double maxival, char *output, int flag);


int main(int argc, char **argv)
{
	int pic[256][256];
	int outpicx[256][256];
	int outpicy[256][256];
	int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
	double ival[256][256], lowVal[256][256], highVal[256][256], maxival;
    
	int i,j,p,q,mr,sum1,sum2, header;
    double low = 40, high = 110;
	FILE *fp1;
    char *foobar;

    foobar = argv[1];
    fp1=fopen(foobar,"rb");
	
	// Avoid scanning in header to 2D array.
	for (i=0; i<15; i++)
		header = getc(fp1);
	
    for (i=0;i<256;i++)
    { 
		for (j=0;j<256;j++)
        {
			pic[i][j] = getc(fp1);
            pic[i][j] &= 0377;
        }
    }
	
	fclose(fp1);
	
	// Edge Detection using scanning convolution 
    mr = 1;
    for (i=mr;i<256-mr;i++)
    { 
		for (j=mr;j<256-mr;j++)
        {
            sum1 = 0;
            sum2 = 0;
            
			for (p=-mr;p<=mr;p++)
            {
				for (q=-mr;q<=mr;q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                   sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
            }
			
			outpicx[i][j] = sum1;
            outpicy[i][j] = sum2;
			
        }
    }

	// Magnitude calculation 
    maxival = 0;
	
    for (i=mr;i<256-mr;i++)
    { 
		for (j=mr;j<256-mr;j++)
        {
			ival[i][j]= sqrt((double)((outpicx[i][j]*outpicx[i][j]) + (outpicy[i][j]*outpicy[i][j])));
            
			if (ival[i][j] > maxival)
                maxival = ival[i][j];			
        }
    }

	// Call print function for magnitude image.
	printpic(ival, maxival, argv[2], 1);
	
	 for (i=0;i<256;i++)
    { 
		for (j=0;j<256;j++)
		{	
			// Detect with low threshold
			if (ival[i][j] > low)
				lowVal[i][j] = 255;
			else
				lowVal[i][j] = 0;
			
			// Detect with high threshold
			if (ival[i][j] > high)
				highVal[i][j] = 255;
			else
				highVal[i][j] = 0;
				
			
		}
	}
	
	// Call print function for low and high image.
	printpic(lowVal, maxival, argv[3], 0);
	printpic(highVal, maxival, argv[4], 0);
	
	return 0;
}

void printpic(double ival[256][256], double maxival, char *output, int flag)
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
			if (flag)
				ival[i][j] = (ival[i][j] / maxival) * 255; 
			
			fprintf(fo1,"%c",(char)((int)(ival[i][j])));
        }
    }
	
	fclose(fo1);
	
	return;
}