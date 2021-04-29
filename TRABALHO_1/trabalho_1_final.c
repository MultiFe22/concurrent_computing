#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

const int iXmax = 1000; //Size in pixels of the x axis
const int iYmax = 1000; //Size in pixels of the y axis
const double CxMin=-2.5;
const double CxMax=1.5;
const double CyMin=-2.0;
const double CyMax=2.0;
unsigned char color[1000][1000][3];
const int MaxColorComponentValue=255; // color component (RGB) is coded from 0 to 255
const int IterationMax=10000; //Number of max iterations, higher is better for precision
int nthreads = 4;

void * mandelbrot(void * arg){
	long int id = (long int) arg;
	//printf("%ld\n",blocSize);
	int iX,iY;
    /* world ( double) coordinate = parameter plane*/
    double Cx,Cy;
    double PixelWidth=(CxMax-CxMin)/iXmax;
    double PixelHeight=(CyMax-CyMin)/iYmax;
       
    /* Z=Zx+Zy*i  ;   Z0 = 0 */
    double Zx, Zy;
    double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
    /*  */
    int Iteration;
    
    /* bail-out value , radius of circle ;  */
    /*Because no complex number with a real or imaginary part greater than 2 can be part of the set, 
    a common bailout is to escape when either coefficient exceeds 2*/
    const double EscapeRadius=2;
    double ER2=EscapeRadius*EscapeRadius;
 
    /* compute image data bytes*/
    for(iY=id;iY<1000;iY+=nthreads)
    {
         Cy=CyMin + iY*PixelHeight;
         if (fabs(Cy)< PixelHeight/2) Cy=0.0; /* Main antenna */
         for(iX=0;iX<iXmax;iX++)
         {         
                    Cx=CxMin + iX*PixelWidth;
                    /* initial value of orbit = critical point Z= 0 */
                    Zx=0.0;
                    Zy=0.0;
                    Zx2=Zx*Zx;
                    Zy2=Zy*Zy;
                    /* */
                    for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
                    {
                        Zy=2*Zx*Zy + Cy;
                        Zx=Zx2-Zy2 +Cx;
                        Zx2=Zx*Zx;
                        Zy2=Zy*Zy;
                    }
                    /* compute  pixel color (24 bit = 3 bytes) */
                    if (Iteration==IterationMax)
                    { /*  interior of Mandelbrot set = black */
                    	color[iY][iX][0]=0;
                    	color[iY][iX][1]=0;
                    	color[iY][iX][2]=0;                           
                    }
                 else 
                    { /* exterior of Mandelbrot set = white */
                        color[iY][iX][0]=255;/* Red */
                        color[iY][iX][1]=255;/* Green */
                        color[iY][iX][2]=255;/* Blue */
                    }
                    
            }
            
    }
    pthread_exit(NULL);
}


int main(int argc, char const *argv[])
{
	double start, end, deltaConc;
	pthread_t *tid; //Creates the threads id
	GET_TIME(start);
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if (tid == NULL){
		fprintf(stderr, "ERROR-malloc\n");
		return 2;
	}
	for(long int i = 0; i<nthreads; i++){
		if(pthread_create(tid+i, NULL, mandelbrot, (void *) i)){
			fprintf(stderr, "ERROR-pthread_create\n");
			return 3;
		}
	}
	for(long int i = 0; i < nthreads; i++){ 
		if(pthread_join(*(tid+i), NULL)){
			fprintf(stderr, "ERROR-pthread_join\n");
			return 3;
		}
	}
	GET_TIME(end);
	deltaConc = end-start;
	printf("Tempo: %.10lf\n", deltaConc);
	FILE * fp;
    char *filename="4x10000.ppm";
    char *comment="# ";/* comment should start with # */
    /*create new file,give it a name and open it in binary mode  */
    fp= fopen(filename,"wb"); /* b -  binary mode */
    /*write ASCII header to the file*/
    fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,iXmax,iYmax,MaxColorComponentValue);
    /*write color to the file*/
    GET_TIME(start);
    for(int iY=0;iY<iYmax;iY++)
    {
        for(int iX=0;iX<iXmax;iX++)
        { 
            fwrite(color[iY][iX],1,3,fp);
    }   }
    GET_TIME(end);
    deltaConc = end-start;
    printf("Tempo: %.10lf\n", deltaConc);

    fclose(fp);
    free(tid);

	return 0;
}