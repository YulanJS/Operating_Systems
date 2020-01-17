#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define NUM_THREADS 27
#define GRID_SIZE 9
#define NUM_SUBGRIDS 9
#define SUBGRID_SIZE 3
typedef struct
{
	int startRow;
	int startCol;
	int numOfRows;
	int numOfCols;
	int threadNum;
}parameters;
void * checkPart(void *param);

int results[NUM_THREADS] = {0};
int grid[GRID_SIZE][GRID_SIZE] = {{6, 5, 3, 1, 2, 8, 7, 9, 4},
				{1, 7, 4, 3, 5, 9, 6, 8, 2},
				{9, 2, 8, 4, 6, 7, 5, 3, 1},
				{2, 8, 6, 5, 1, 4, 3, 7, 9},
				{3, 9, 1, 7, 8, 2, 4, 5, 6},
				{5, 4, 7, 6, 9, 3, 2, 1, 8},
				{8, 6, 5, 2, 3, 1, 9, 4, 7},
				{4, 1, 2, 9, 7, 5, 8, 6, 3},
				{7, 3, 9, 8, 4, 6, 1, 2, 5}};
int main(int agrc, char*argv[])
{
	pthread_t tid;
	pthread_attr_t attr;

	pthread_t workers[NUM_THREADS];
	parameters* data[NUM_THREADS];

	printf("\nCS149 Sudoku from Yulan Jin\n");
	//print the current sudoku first
	for(int i = 0; i < GRID_SIZE; i++)
	{
		for(int j = 0; j < GRID_SIZE; j++)
		{
			printf("%d ",grid[i][j]);
		}
		printf("\n");
	}
	for(int i = 0; i < NUM_THREADS; i++)
	{

		data[i] = (parameters *)malloc(sizeof(parameters));
		pthread_attr_init(&attr);
		if(i < NUM_SUBGRIDS)
		{
			//each subgrid: index 0 to 8
			data[i]-> startRow = i / (SUBGRID_SIZE) * SUBGRID_SIZE;
			data[i]-> startCol = i % (SUBGRID_SIZE) * SUBGRID_SIZE;
			data[i]-> numOfRows = SUBGRID_SIZE;
			data[i]-> numOfCols = SUBGRID_SIZE;
			data[i]-> threadNum = i;

		}
		else if(i >= NUM_SUBGRIDS && i < NUM_SUBGRIDS + GRID_SIZE)
		{
			//check columns: index 9 to 17
			data[i]-> startRow = 0;
			data[i]-> startCol = i - NUM_SUBGRIDS;
			data[i]-> numOfRows = GRID_SIZE;
			data[i]-> numOfCols = 1;
			data[i]-> threadNum = i;
		}
		else
		{
			//check rows: index 18 to 26
			data[i]-> startRow = i - NUM_SUBGRIDS - GRID_SIZE;
			data[i]-> startCol = 0;
			data[i]-> numOfRows = 1;
			data[i]-> numOfCols = GRID_SIZE;
			data[i]-> threadNum = i;
		}
		pthread_create(&workers[i], &attr, checkPart, data[i]);
	}

	for(int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(workers[i], NULL);
	}
	for(int i = 0; i < NUM_THREADS; i++)
	{
		//after each thread finishes (after join), free parameters* pointers
		free((void*) data[i]);
	}
	/*
	printf("\nthreads all completed");
	printf("\n");
	for(int i = 0; i < NUM_THREADS; i++)
	{
		if(i % GRID_SIZE == 0) printf("\n");
		printf("%d ", results[i]);
	}*/
	for(int i = 0; i < NUM_THREADS; i++)
	{
		if(results[i] != 1)
		{
			//printf("\n%dth thread gets an invalid result\n", i);
			printf("The sudoku is invalid\n");
			return 0;
		}
	}
	printf("\nThe sudoku is valid\n.");
	return 0;
}

void * checkPart(void *param)
{
	//check repetition in a subgrid, a column or a row
	int count[SUBGRID_SIZE * SUBGRID_SIZE]= {0};
	int k = 0;
	parameters* data = (parameters *) param;//type cast
	for(int i = data->startRow; i < data->startRow + data->numOfRows; i++)
	{
		for(int j = data->startCol; j < data->startCol + data->numOfCols; j++)
		{
			if(grid[i][j] >= 1 && grid[i][j] <= SUBGRID_SIZE * SUBGRID_SIZE)
			{
				count[grid[i][j] - 1] += 1;
			}
		}
	}
	for(k = 0; k < SUBGRID_SIZE * SUBGRID_SIZE; k++)
	{
		//if any number appear not exactly once, this subgrid is invalid
		if(count[k] != 1)
		{
			results[data->threadNum]= 0;
			//printf("\nInvalid result from %dth thread", data->threadNum);

			pthread_exit(0);
		}
	}
	results[data->threadNum]= 1;
	//printf("\nValid result from %dth thread", data->threadNum);
	pthread_exit(0);
}
