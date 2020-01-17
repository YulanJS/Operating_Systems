//
//  main.c
//  homework2
//
//  Created by Craig on 9/30/17.
//  Copyright © 2017 Craig. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define GRID_SIZE 9

/* structure for passing data to threads */
typedef struct
{
    int row;
    int column;
} parameters;

void *checkValidRow(void *param);
void *checkValidColumn(void *param);
void *checkValidSquare(void *param);

int grid[GRID_SIZE][GRID_SIZE] = {{6,5,3,1,2,8,7,9,4}, {1,7,4,3,5,9,6,8,2}, {9,2,8,4,6,7,5,3,1} , {2,8,6,5,1,4,3,7,9} , {3,9,1,7,8,2,4,5,6} , {5,4,7,6,9,3,2,1,8} , {8,6,5,2,3,1,9,4,7} , {4,1,2,9,7,5,8,6,3} , {7,3,9,8,4,6,1,2,5} };
int validCheck[GRID_SIZE*GRID_SIZE];
int counter = 0;

/* Now we need to create the thread passing data as the parameter*/
int main(int argc, const char * argv[]) {
    
    pthread_t tid = NULL;
    pthread_attr_t attr;
    printf("CS149 Sudoku from Craig Huff\n");
    int finish = 1;
    
    //print out the grid
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; j++) {
            printf("%d", grid[i][j]);
            printf("%s", ",");
        }
        printf("\n");
    }
    
   /*Loop creates threads that check each row*/
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < 1; j++) {
            parameters *data = (parameters * ) malloc(sizeof(parameters));
            data->row = i;
            data->column = j;
            pthread_attr_init(&attr);
            pthread_create(&tid, &attr, checkValidRow, data);
        }
    }
    
    /*Loop creates threads that check each column*/
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < GRID_SIZE; j++) {
            parameters *data = (parameters * ) malloc(sizeof(parameters));
            data->row = i;
            data->column = j;
            pthread_attr_init(&attr);
            pthread_create(&tid, &attr, checkValidColumn, data);
        }
    }
    
    /*Loop creates threads that check each 3x3 square of the grid*/
    for (int i = 0; i < GRID_SIZE; i+=3) {
        for (int j = 0; j < GRID_SIZE; j+=3) {
            parameters *data = (parameters * ) malloc(sizeof(parameters));
            data->row = i;
            data->column = j;
            pthread_attr_init(&attr);
            pthread_create(&tid, &attr, checkValidSquare, data);
        }
    }
    
    pthread_join(tid, NULL);
    
    /*Checks to make sure the table is valid or not*/
    for (int i = 0; i < counter; i++) {
        if (validCheck[i] == 0){
            printf("%s", "\nInvalid Sudoku Table\n");
            finish = 0;
            break;
        }
    }
    if (finish == 1)
    printf("%s","\nThis is a valid Sudoku Table\n");
    return 0;
}

/*Check the validity of the row of the specific thread and location in the grid.
 *Since each row will have a total of 45 if it's valid, that's what we need to check for
 */
void *checkValidRow(void *param)
{
    int sum = 0;
    parameters * data = (parameters *) param;
    int startRow = data->row;
    int startCol = data->column;
    
    for (int i = startRow; i < startRow+1; ++i){
        for (int j = startCol; j < GRID_SIZE; ++j){
            sum += grid[i][j];
        }
    }
    if(sum != 45) {
        validCheck[counter] = 0;
        counter++;
    }
    else{
        validCheck[counter] = 1;
        counter++;
//        printf("%s", "YAY! ");
    }
    return (void*) 1;
}

/*Check the validity of a column of the specific thread and location in the grid.
 *Since each column will have a total of 45 if it's valid, that's what we need to check for.
 */
void *checkValidColumn(void *param)
{
    int sum = 0;
    parameters * data = (parameters *) param;
    int startRow = data->row;
    int startCol = data->column;
    
    for (int i = startCol; i < startCol+1; ++i){
        for (int j = startRow; j < GRID_SIZE; ++j){
            sum += grid[j][i];
        }
    }
    if(sum != 45) {
        validCheck[counter] = 0;
        counter++;
    }
    else {
        validCheck[counter] = 1;
        counter++;
//        printf("%s", "Cool Beans? ");
    }
      return (void*) 1;
}

/*Check the validity of the sqaure of the specific thread and location in the grid.
 *Since each 3x3 sqaure will have a total of 45 if it's valid, that's what we need to check for.
 */
void *checkValidSquare(void *param)
{
    int sum = 0;
    parameters * data = (parameters *) param;
    int startRow = data->row;
    int startCol = data->column;
    
    for (int i = startRow; i < startRow+3; ++i){
        for (int j = startCol; j < startCol+3; ++j){
            sum += grid[i][j];
        }
    }
    if(sum != 45){
        validCheck[counter] = 0;
        counter++;
    }
    else {
        validCheck[counter] = 1;
        counter++;
    }
     return (void*) 1;
}
