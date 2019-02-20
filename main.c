#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZEOFBD 9

void printBoard(char *board)
{
    for(int i = 0; i < SIZEOFBD; i++)
    {
        printf("%c |", board[i]);
        if (( i + 1 ) % 3 == 0) {
           printf("\n");
        }
        
    }
    
}

int CheckBoard(char *board)
{
    int index =0;
    for(int x = 0; x < 3; x++)
    {
        //horizontal check
        if ((board[index] == board[index + 1]) && (board[index + 1] == board[index +2])) {
            printf("%c wins\n", board[index]);
            return 1;
        }
        //vertical check
        if((board[x] == board[x+3]) && (board[x+6]== board[x+3]))
        {
             printf("%c wins\n", board[x]);
             return 1;
        }
        index += 3;
    }
    if(((board[0] == board[4]) && (board[8] == board[4])) || ((board[2] == board[4]) && (board[6] == board[4])))
    {
         printf("%c wins\n", board[4]);
             return 1;
    }
    return 0;
}

void Move(char *board, int turn)
{
    char player = 'x';
    int index =10;
    int correct = 0;
    if (turn % 2 == 1)
    {
        player = 'o';
    }
    do
    {
        printf("Enter a square: ");
        scanf("%d", &index);
        if(index<10 && index>0)
        {
            char c = index + '0';
            if(board[index -1] == c)
            {
                board[index -1] = player;
                correct = 1;
            }
            else
            {
                printf("Wrong move\n");
            }
            
        }else
        {
            printf("Wrong move\n");
        }
        
        
    } while (correct == 0);
}
int main()
{
    int turn =0;
    int gameOver = 0;
    char board[SIZEOFBD] = {'1','2','3','4','5','6','7','8','9'};
    while(gameOver == 0)
    {
        printBoard(board);
        Move(&*board, turn);
        gameOver = CheckBoard(&*board);
        turn += 1;
    }
    
    

    return 0;
}