#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define threadNum 6
#define x 8
#define y 8
#define generations 5

int array1[x][y] = {
         {0,  0,  0,   0,  0,  0,  0, 0},
         {0,  3,  6,  17, 15, 13, 15, 0},
         {0,  6, 12,   9,  1,  2,  7, 0},
         {0, 10, 19,   3,  6,  0,  6, 0},
         {0, 12, 16,  11,  8,  7,  9, 0},
         {0,  2, 10,   2,  3,  7, 15, 0},
         {0,  9,  2,   2, 18,  9,  7, 0},
         {0,  0,  0,   0,  0,  0,  0, 0}
     };
int array2[x][y] = {
         {0,  0,  0,   0,  0,  0,  0, 0},
         {0,  3,  6,  17, 15, 13, 15, 0},
         {0,  6, 12,   9,  1,  2,  7, 0},
         {0, 10, 19,   3,  6,  0,  6, 0},
         {0, 12, 16,  11,  8,  7,  9, 0},
         {0,  2, 10,   2,  3,  7, 15, 0},
         {0,  9,  2,   2, 18,  9,  7, 0},
         {0,  0,  0,   0,  0,  0,  0, 0}
     };

//initializes arrays using rand and copies array1 to array2
void initializeArray(int array[x][y])
{
     int i,j = 0;
     
	 //loops through and initializes border of 0's and 
	 //intializes the rest with rand
     for(j = 0; j < y; j++)
     {
		 for(i = 0; i < x; i++)
		 {
			 if(i == 0 || i == x-1 || j == 0 || j == y-1)
			 {
				 array[i][j] = 0;
			 }
			 else
			 {
				 array[i][j] = rand() % 100;
			 }
		 }
	 }
	
	 //copies array1 to array2
	 for(j = 0; j < y; j++)
	 {
		 for(i = 0; i < x; i++)
		 {
			 array2[i][j] = array[i][j];
		 }
	 }
}

//prints array passed
void printArray(int array[x][y])
{
     int i,j = 0;
	 
	 //loops and prints array
     for(j = 0; j < y; j++)
     {
           for(i = 0; i < x; i++)
           {
                 printf("%2d ", array[i][j]);      
           }
           printf("\n");      
     }    
}

//copies array2 to array1
void copyAfromB(int array[x][y])
{
		int i,j = 0;
		
		for(j = 0; j < y; j++)
		{
			for(i = 0; i < x; i++)
			{
				array1[i][j] = array[i][j];
			}
		}
}

//called by each thread and loops from start row to end row
//computes sum and according to rules updates array 2
void computeAndUpdate(int start, int end)
{
	int i,j, sum = 0;
	
	//loops from start row to end row
	for(i = start+1; i <= end; i++)
	{
		//loops through each row, computes sum, and updates array 2
		//according to rules
		for(j = 1; j < y-1; j++)
		{
				sum = array1[i][j];
				sum = sum + array1[i - 1][j];
				sum = sum + array1[i + 1][j];
				sum = sum + array1[i][j + 1];
				sum = sum + array1[i - 1][j + 1];
				sum = sum + array1[i + 1][j + 1];
				sum = sum + array1[i][j - 1];
				sum = sum + array1[i - 1][j -1];
				sum = sum + array1[i + 1][j + 1];
				
				if(sum % 10 == 0)
				{
					array2[i][j] = 0;
				}
				else if(sum < 50)
				{
					array2[i][j] = array1[i][j] + 3;
				}
				else if(sum > 50 && sum < 150)
				{
					if(array2[i][j] > 2)
					{
						array2[i][j] = array1[i][j] - 3;
					}
					else
					{
						array2[i][j] = array1[i][j];
					}
				}
				else
				{
					array2[i][j] = 1;
				}
				
				sum = 0;
		}
	}
	
	
}

//splits work between threads based on thread id and number of rows
//calls computeAndUpdate for each thread which computes the sum 
//and updates array 2
void *divideWork(void *param)
{
	//determines rows to be done by each thread
	int tid = (int) param;
	int row = y - 2;
	int divRow = row / threadNum;
	int leftoverRow = row % threadNum;
	int start = divRow * tid;
	int end;
	
	if(tid == threadNum - 1)
	{
		end  = divRow * tid + divRow + leftoverRow;
	}
	else
	{
		end = divRow * tid + divRow;
	}
	
	computeAndUpdate(start, end);
	
	pthread_exit(NULL);
}

//creates threads, divides work between threads, and then joins when
//computation is complete
void createThread(void)
{
	pthread_t tids[threadNum];
	int i = 0;
	void *status;
	
	for(i = 0; i < threadNum; i++)
	{
		pthread_create(&tids[i], NULL, divideWork, (void *)i);
	}
	
	for(i = 0; i < threadNum; i++)
	{
		pthread_join(tids[i], &status);
	}
}

//array1 and array2 start with same values, array 2 is updated 
//according to rules, and then array2 is copied to array1 to start again
int main(void)
{	
	//Used for random generation, currently using Tom's array
	//srand(1);
    //initializeArray(array1);
    int g = 1;
	
	//loops for number of generations and prints to screen
	printf("Generation 0: \n");
	printArray(array1);
	while(g < generations)
	{
	createThread();
	copyAfromB(array2);
	
	printf("Generation %d: \n", g);
    printArray(array1);
	
	g++;
	}
    
    return 0;
}
