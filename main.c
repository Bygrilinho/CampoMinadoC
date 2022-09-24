#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define ROW 8
#define COL 8

void render(int *a[ROW][COL], int  *b[ROW][COL], int *c[ROW][COL]);

int random(int min, int max);

int main()
{
    int visivel[ROW][COL]={{0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,1,0,0,0,0},
                          {0,0,0,0,0,1,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0}};

    int numeros[ROW][COL]={{0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,3,2,0,0,0},
                          {0,0,0,0,0,5,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0}};

    int minas[ROW][COL]=  {{0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0}};

    render(&visivel, &numeros, &minas);
}

int random(int min, int max)
{
    int resultado = 0, menor = 0, maior = 0;

    if (min < max)
    {
        menor = min;
        maior = max + 1;
    } else {
        menor = max + 1;
        maior = min;
    }

    srand(time(NULL));
    resultado = (rand() % (maior - menor)) + menor;
    return resultado;
}

void render(int *a[ROW][COL], int  *b[ROW][COL], int *c[ROW][COL]){
    printf("  A B C D E F G H");
    for(int i=0;i<ROW;i++){
        printf("\n%d ",i+1);
        for(int j=0;j<COL;j++){
            if(a[i][j]==0){
                printf("? ");
            } else {
                printf("%d ", b[i][j]);
            }
        }
    }
}