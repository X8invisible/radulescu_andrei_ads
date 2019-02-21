#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZEOFBD 9

struct History
{
    int slotChanged;
    char sign;
    struct History *next, *prev;
};
struct Player
{
    char sign;
    char name[256];
    int score;
};


void printScore(struct Player one, struct Player two)
{
    printf("\n\nScore x (%s): %d | Score y (%s): %d\n",one.name,one.score,two.name,two.score);
}
void printBoard(char *board)
{
    for(int i = 0; i < SIZEOFBD; i++)
    {
        printf(" %c ", board[i]);
        if (( i + 1 ) % 3 == 0) {
           printf("\n-------------\n");
        }else
            printf("|");
        
    }
    
}
char CheckBoard(char *board)
{
    int index =0;
    for(int x = 0; x < 3; x++)
    {
        //horizontal check
        if ((board[index] == board[index + 1]) && (board[index + 1] == board[index +2])) {
            printf("%c wins\n", board[index]);
            return board[index];
        }
        //vertical check
        if((board[x] == board[x+3]) && (board[x+6]== board[x+3]))
        {
             printf("%c wins\n", board[x]);
             return board[x];
        }
        index += 3;
    }
    if(((board[0] == board[4]) && (board[8] == board[4])) || ((board[2] == board[4]) && (board[6] == board[4])))
    {
         printf("%c wins\n", board[4]);
             return board[4];
    }
    return 'n';
}

void Change(char *board, struct History *h)
{
    int index = h->slotChanged;
    char sign = h->sign;

    board[index] = (index+1)+'0';
}
void Move(char *board, struct Player p)
{
    char player = p.sign;
    int index =10;
    int correct = 0;
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

void resetGame(char *board)
{
    for(int i = 0; i < SIZEOFBD; i++)
    {
        board[i] = (i+1) + '0';
    }
    
}
int main()
{
    struct Player one, two;
    one.sign = 'x';
    one.score = 0;
    two.sign = '0';
    two.score = 0;
    printf("Player one name: ");
    scanf("%s", one.name);
    printf("Player two name: ");
    scanf("%s", two.name);
    int turn =0, gameOver = 0;
    char winner;
    char board[SIZEOFBD] = "123456789";
    while(gameOver == 0)
    {
        struct Player p;
        if(turn % 2 == 0)
        {
            p = one;
        }else
        {
            p = two;
        }
        printScore(one,two);
        printBoard(board);
        Move(&*board, p);
        winner = CheckBoard(&*board);
        if(winner == 'x')
        {
            one.score++;
        }
        if (winner =='0') {
            two.score++;
        }
        if(winner != 'n')
        {
            printf("Play another?(y/n): ");
            char choice;
            scanf(" %c", &choice);
            if(choice == 'y')
            {
                resetGame(&*board);
            }
            else
            {
                gameOver =1;
            }
            
            
            
        }
        
        turn += 1;
    }
    
    

    return 0;
}