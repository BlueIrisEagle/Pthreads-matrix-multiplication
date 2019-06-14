#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


#define MAX_THREADS 100

int **A;
int **B;
int **C;
int **D;
int x,y,r,q,i,j,a,z;

struct timeval  tv1, tv2, tv3, tv4;


///Type Defining For Passing Both Functions Arguments

typedef struct parameters
{

    int x,y;

} args;

typedef struct rowParameters
{
    int x;

} rowArgs;



///Function For Calculate Each Element in Result Matrix Used By Threads - - -//

void* mult(void* arg)
{

    args* p = arg;

    ///Calculating Each Element in Result Matrix Using Passed Arguments
    for(a=0; a<r; a++)
    {
        C[p->x][p->y] += A[p->x][a] * B[a][p->y];
    }



    //End Of Thread
    pthread_exit(0);
}

///Function For Calculate Each Row in Result Matrix Used By Threads - - -//
void* multRowByRow(void* arg)
{

    rowArgs* rp = arg;

    ///Calculating Each Row in Result Matrix Using Passed Arguments
    for(z=0; z<q; z++){///bigger loop is for result matrix2 row

    for(a=0; a<r; a++) /// for equating each element
    {
        //printf("\n&&%d&& ",
               D[rp->x][z] += A[rp->x][a] * B[a][z];
              // );
    }


    }

    //Sleep(3);
    ///End Of Thread
    pthread_exit(0);
}



int main(void)
{

    FILE* fptr;
    //char ch; ///buffer

    char ip_file_name[]="input.txt";
    char op_file_name[]="output.txt";

    fptr=fopen(ip_file_name,"r");

    if (fptr == NULL)
    {
        perror("Error while opening the file!\n");
        exit(EXIT_FAILURE);
    }



///FIRST MATRIX BUFFERING
    fscanf(fptr,"%d %d",&x,&y);

///Read the matrix dimensions
    A=(int**)malloc(sizeof(int*)*x);
    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            A[i]=(int*)malloc(sizeof(int)*y);
        }
    }
///allocate the memory
    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            fscanf(fptr,"%d",&A[i][j]);
        }
    }
    printf(" --- First Matrix ---\n\n");
    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            printf("%3d ",A[i][j]);
        }
        printf("\n");
    }

///SECOND MATRIX BUFFERING
    fscanf(fptr,"%d %d",&r,&q);

///Read the matrix dimensions
    B=(int**)malloc(sizeof(int*)*r);
    for(i=0; i<r; i++)
    {
        for(j=0; j<q; j++)
        {
            B[i]=(int*)malloc(sizeof(int)*q);
        }
    }

///allocate the memory
    for(i=0; i<r; i++)
    {
        for(j=0; j<q; j++)
        {
            fscanf(fptr,"%d",&B[i][j]);
        }
    }

    fclose(fptr);   ///Closing file

    printf(" --- Second Matrix ---\n\n");
    for(i=0; i<r; i++)
    {
        for(j=0; j<q; j++)
        {
            printf("%3d ",B[i][j]);
        }
        printf("\n");
    }


///DIMENTIONS CHECKING
    if(x != q)
    {
        printf("\nDimention error !\n");
        return -1;
    }


/// Result matrix allocation
    C=(int**)malloc(sizeof(int*)*x);
    for(i=0; i<x; i++)
    {
        for(j=0; j<q; j++)
        {
            C[i]=(int*)malloc(sizeof(int)*q);
        }
    }

///Initializing Result Matrix
    for(i=0; i<x; i++)
    {
        for(j=0; j<q; j++)
        {
            C[i][j]=0;
        }
    }



    /// Result matrix2 allocation
    D=(int**)malloc(sizeof(int*)*x);
    for(i=0; i<x; i++)
    {
        for(j=0; j<q; j++)
        {
            D[i]=(int*)malloc(sizeof(int)*q);
        }
    }

///Initializing Result Matrix2
    for(i=0; i<x; i++)
    {
        for(j=0; j<q; j++)
        {
            D[i][j]=0;
        }
    }




    ///Defining Threads For element by element
    pthread_t thread[MAX_THREADS];

    pthread_t rthread[MAX_THREADS];


    ///Counter For Thread Index
    int thread_number = 0;
    int rthread_number = 0;


    ///Defining p For Passing Parameters To Function "Mult" As Struct
    args p[x*q];

    rowArgs rp[x];


    ///Start Timer
    gettimeofday(&tv1, NULL);


    for(i=0; i<x; i++)
    {
        for(j=0; j<q; j++)
        {

            ///Initializing Parameters For Passing To Function
            p[thread_number].x=i;
            p[thread_number].y=j;


            ///Status Flag For Checking Errors
            int status;



            ///Create Specific Thread For Each Element In Result Matrix
            status = pthread_create(&thread[thread_number], NULL, mult, (void *) &p[thread_number]);



            ///Check For Error
            if(status!=0)
            {

                printf("Error In Threads");

                exit(0);

            }



            thread_number++;

        }

    }

///Taking Timer Lap
gettimeofday(&tv2, NULL);

    for(i=0; i<x; i++){  ///--------------------------------------------TASK2------------------------------------------
            rp[rthread_number].x=i;

            /*
            for(a=0;a<q;a++){
                rowbuffer[a]=A[i][a]
            }
            rp[thread_number].y=rowbuffer;
            */

            ///Status Flag For Checking Errors
            int status2;



            ///Create Specific Thread For Each Element In Result Matrix
            status2 = pthread_create(&rthread[rthread_number], NULL, multRowByRow, (void *) &rp[rthread_number]);



            ///Check For Error
            if(status2!=0)
            {

                printf("Error In Threads");

                exit(0);

            }

            rthread_number++;

            }

///Taking Timer Lap
gettimeofday(&tv3, NULL);





    ///Wait For All Threads Done//
    for(z=0; z<(x*q); z++)
        pthread_join(thread[z],NULL );

    for(z=0; z<x; z++)
        pthread_join(rthread[z],NULL );




    ///Print Multiplied Matrix (Result)//
    printf(" --- Multiplied Matrix ---\n\n");

    for(i=0; i<x; i++)
    {

        for(j=0; j<q; j++)
        {

            printf("%5d",C[i][j]);

        }

        printf("\n\n");

    }

     printf(" --- Multiplied Matrix2 ---\n\n");

    for(i=0; i<x; i++)
    {

        for(j=0; j<q; j++)
        {

            printf("%5d",D[i][j]);

        }

        printf("\n\n");

    }
    /// Printing into file
    fptr=fopen(op_file_name,"w");
    for(i=0;i<x;i++){
        for(j=0;j<q;j++)
            fprintf(fptr,"%5d",C[i][j]);
        fprintf(fptr,"\n\n");
    }



    ///Calculate Total Time - - - -//

    printf ("\n-> Total time for task 1 = %f seconds\n",
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));

printf ("\n-> Total time for task 2 = %f seconds\n",
         (double) (tv3.tv_usec - tv2.tv_usec) / 1000000 +
         (double) (tv3.tv_sec - tv2.tv_sec));

///*--------------------------------------------------------------------------File Business-------------------------------*/
         fprintf (fptr,"\n-> Total time for task 1 = %f seconds\n",
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));

        fprintf (fptr,"\n-> Total time for task 2 = %f seconds\n",
         (double) (tv3.tv_usec - tv2.tv_usec) / 1000000 +
         (double) (tv3.tv_sec - tv2.tv_sec));

         fprintf(fptr,"\n->Apparently, Total time for Multi-thread row multiplication is way smaller than element by element technique\n");


    printf("\n ->Task 1 Used Threads : %d \n\n",thread_number);

    for(z=0; z<thread_number; z++)

        printf(" - Thread %d ID : %d\n",z+1,(int)thread[z]);

    printf("\n ->Task 2 Used Threads : %d \n\n",rthread_number);

    for(z=0; z<rthread_number; z++)

        printf(" - Thread %d ID : %d\n",z+1,(int)rthread[z]);


        fprintf(fptr,"\n ->Task 1 Used Threads : %d \n\n",thread_number);

    for(z=0; z<thread_number; z++)

        fprintf(fptr," - Thread %d ID : %d\n",z+1,(int)thread[z]);


    fprintf(fptr,"\n ->Task 2 Used Threads : %d \n\n",rthread_number);

    for(z=0; z<rthread_number; z++)

        fprintf(fptr," - Thread %d ID : %d\n",z+1,(int)rthread[z]);

///*-------------------------------------------------End of File Business-------------------------------------------------------*/
///Freeing Allocated Resources
    for(i=0; i<x; i++)
    {
        free(A[i]);
    }
for(i=0; i<r; i++)
    {
        free(B[i]);
    }
    for(i=0; i<x; i++)
    {
        free(C[i]);
    }
     for(i=0; i<x; i++)
    {
        free(D[i]);
    }

    free(A);
    free(B);
    free(C);
    free(D);


    return 0;
}


