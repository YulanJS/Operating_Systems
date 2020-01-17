#include <stdio.h>
#include <stdlib.h>
#include <string.h>

short sudoku[9][9];
unsigned long long cubeSolutions=0;
void* cubeValues[10];
const unsigned char oneLookup[64] = {0x8b, 0x80, 0, 0x80, 0, 0, 0, 0x80, 0, 0,0,0,0,0,0, 0x80, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int ifOne(int val) {
  if ( oneLookup[(val-1) >> 3] & (1 << ((val-1) & 0x7))  )
    return val;
  return 0;
}


void init_sudoku() 
{
  int i,j;
  for (i=0; i<9; i++)
    for (j=0; j<9; j++)
      sudoku[i][j]=0x1ff;
}
void set_sudoku( char* initialValues)
 {
  int i;
  if ( strlen (initialValues) !=  81 ) {
    printf("Error: inputString should have length=81, length is %2.2d\n", strlen(initialValues) );
    exit (-12);
  }
  for (i=0; i < 81; i++)
    if ((initialValues[i] > 0x30) && (initialValues[i] <= 0x3a))
      sudoku[i/9][i%9] = 1 << (initialValues[i] - 0x31) ;
}
void print_sudoku ( int style ) {
  int i, j, k;
  for (i=0; i < 9; i++) {
    for (j=0; j < 9; j++) {
      if ( ifOne(sudoku[i][j]) || !style) {
        for (k=0; k < 9; k++)
          if (sudoku[i][j] & 1<<k)
            printf("%d", k+1);
      } else
        printf("*");
      if ( !((j+1)%3) )
        printf("\t");
      else
        printf(",");
    }
    printf("\n");
    if (!((i+1) % 3) )
      printf("\n");
  }
}
void print_HTML_sudoku () {
  int i, j, k, l, m;
  printf("<TABLE>\n");
  for (i=0; i<3; i++) {
    printf("  <TR>\n");
    for (j=0; j<3; j++) {
      printf("    <TD><TABLE>\n");
      for (l=0; l<3; l++) { printf("      <TR>"); for (m=0; m<3; m++) { printf("<TD>"); for (k=0; k < 9; k++)  { if (sudoku[i*3+l][j*3+m] & 1<<k)
            printf("%d", k+1);
          }
          printf("</TD>");
        }
        printf("</TR>\n");
      }
    printf("    </TABLE></TD>\n");
    }
    printf("  </TR>\n");
  }
  printf("</TABLE>");
}
int doRow () {
  int count=0, new_value, row_value, i, j;
  for (i=0; i<9; i++) {
    row_value=0x1ff;
    for (j=0; j<9; j++)
      row_value&=~ifOne(sudoku[i][j]);
    for (j=0; j<9; j++) {
      new_value=sudoku[i][j] & row_value;
      if (new_value && (new_value != sudoku[i][j]) ) {
        count++;
        sudoku[i][j] = new_value;
      }
    }
  }
  return count;
}
int doCol () {
  int count=0, new_value, col_value, i, j;
  for (i=0; i<9; i++) {
    col_value=0x1ff;
    for (j=0; j<9; j++)
      col_value&=~ifOne(sudoku[j][i]);
    for (j=0; j<9; j++) {
      new_value=sudoku[j][i] & col_value;
      if (new_value && (new_value != sudoku[j][i]) ) {
        count++;
        sudoku[j][i] = new_value;
      }
    }
  }
  return count;
}
int doCube () {
  int count=0, new_value, cube_value, i, j, l, m;
  for (i=0; i<3; i++)
    for (j=0; j<3; j++) {
      cube_value=0x1ff;
      for (l=0; l<3; l++)
        for (m=0; m<3; m++)
          cube_value&=~ifOne(sudoku[i*3+l][j*3+m]);
      for (l=0; l<3; l++)
        for (m=0; m<3; m++) {
          new_value=sudoku[i*3+l][j*3+m] & cube_value;
          if (new_value && (new_value != sudoku[i*3+l][j*3+m]) ) {
            count++;
            sudoku[i*3+l][j*3+m] = new_value;
          }
        }
    }
  return count;
}
#define FALSE -1
#define TRUE 1
#define INCOMPLETE 0

int validCube () {
  int i, j, l, m, r, c;
  int pigeon;
  int solved=TRUE;

  //check horizontal
  for (i=0; i<9; i++) {
    pigeon=0;
    for (j=0; j<9; j++)
      if (ifOne(sudoku[i][j])) {
        if (pigeon & sudoku[i][j]) return FALSE;
        pigeon |= sudoku[i][j];
      } else {
        solved=INCOMPLETE;
      }
  }
  //check vertical
  for (i=0; i<9; i++) {
    pigeon=0;
    for (j=0; j<9; j++)
      if (ifOne(sudoku[j][i])) {
        if (pigeon & sudoku[j][i]) return FALSE;
        pigeon |= sudoku[j][i];
      }
      else {
        solved=INCOMPLETE;
      }
  }

  //check cube
  for (i=0; i<3; i++)
    for (j=0; j<3; j++) {
      pigeon=0;
      r=j*3; c=i*3;
      for (l=0; l<3; l++)
        for (m=0; m<3; m++)
        if (ifOne(sudoku[r+l][c+m])) {
          if (pigeon & sudoku[r+l][c+m]) return FALSE;
          pigeon |= sudoku[r+l][c+m];
        }
        else {
          solved=INCOMPLETE;
        }
    }

  return solved;
}
int solveSudoku(int position ) {
  int status, i, k;
  short oldCube[9][9];

  for (i=position; i < 81; i++) {

    while ( doCube() + doRow() + doCol() );

    status = validCube() ;
    if ((status == TRUE) || (status == FALSE))
      return status;


    if ((status == INCOMPLETE) && !ifOne(sudoku[i/9][i%9]) ) {
      memcpy( &oldCube, &sudoku, sizeof(short) * 81) ;
      for (k=0; k < 9; k++) {
        if ( sudoku[i/9][i%9] & (1<<k) ) {
          sudoku[i/9][i%9] = 1 << k ;
          if (solveSudoku(i+1) == TRUE ) {

            /* return TRUE; */
            /* Or look for entire set of solutions */

            if (cubeSolutions < 10) {
              cubeValues[cubeSolutions] = malloc ( sizeof(short) * 81 ) ;
              memcpy( cubeValues[cubeSolutions], &sudoku, sizeof(short) * 81) ;
            }

            cubeSolutions++;
            if ((cubeSolutions & 0x3ffff) == 0x3ffff ) {
              printf ("cubeSolutions = %llx\n", cubeSolutions+1 );
            }

            //if ( cubeSolutions > 10 ) 
            //    return TRUE;

          }

          memcpy( &sudoku, &oldCube, sizeof(short) * 81) ;
        }
        if (k==8)
          return FALSE;
      }

    }
  }

  return FALSE;
}
int main ( int argc, char** argv)  {
  int i;
  if (argc != 2) {
    printf("Error: number of arguments on command line is incorrect\n");
    exit (-12);
  }

  init_sudoku();
  set_sudoku(argv[1]);

  printf("[----------------------- Input  Data ------------------------]\n\n");
  print_sudoku(1);

  solveSudoku(0);
  if ((validCube()==1) && !cubeSolutions)  {
    // If sudoku is effectively already solved, cubeSolutions will not be set
    printf ("\n  This is a trivial sudoku. \n\n");
    print_sudoku(1);
  }


  if (!cubeSolutions && validCube()!=1)
    printf("Not Solvable\n");
  if (cubeSolutions > 1) {
    if (cubeSolutions >= 10)
      printf("10+ Solutions, returning first 10 (%lld) [%llx] \n", cubeSolutions, cubeSolutions);
    else
      printf("%llx Solutions. \n", cubeSolutions);
  }

  for (i=0; (i < cubeSolutions) && (i < 10); i++) {
    memcpy ( &sudoku, cubeValues[i], sizeof(short) * 81 );
    printf("[----------------------- Solution %2.2d ------------------------]\n\n", i+1);
    print_sudoku(0);
    //print_HTML_sudoku();
  }
  return 0;
}