#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZEOFBD 9

int turn =0,bigBoardIndex =-1;
struct History
{
    int bigBoard;
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

struct Board
{
    char board[SIZEOFBD];
    char won;
};
void setBigBoard(struct Board **bigBoard)
{
    for(int i = 0; i < SIZEOFBD; i++)
    {
        bigBoard[i] = malloc(sizeof(struct Board));
        for(int j = 0; j < SIZEOFBD; j++)
        {
            bigBoard[i]->board[j] = j+1 +'0';
        }
        printf("\n");
        bigBoard[i]->won = 'f';
    }
}

void displayBigBoard(struct Board **bigBoard)
{
    printf("\n\n");
    int i =0, row =0;
    while(i < SIZEOFBD)
    {
        for(int j = i; j < i+3; j++)
        {
            if(bigBoard[j]->won == 'f')
                printf(" [%d]▬  %c  ▬  %c  ▬  %c  ▬ ",j+1, bigBoard[j]->board[row*3], bigBoard[j]->board[row*3+1], bigBoard[j]->board[row*3+2]);
            else
                printf(" [%d]▬  %c  ▬  %c  ▬  %c  ▬ ",j+1, bigBoard[j]->won, bigBoard[j]->won, bigBoard[j]->won);
            
        }
        printf("\n\n");
        row++;
        if(row == 3)
        {
            row =0;
            i +=3;
            printf("▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n");
        }
        
    }
    
}
void appendMove(struct History **list,int bigBoardSlot, int slot, char sign)
{
    struct History *temp, *current = *list;
    if(*list == NULL)
    {
        *list = (struct History*)malloc(sizeof(struct History));
        (*list)->prev = NULL;
        (*list)->bigBoard = bigBoardSlot;
        (*list)->slotChanged = slot;
        (*list)->sign = sign;
        (*list)->next = NULL;
    }else
    {
        while(current->next !=NULL)
            current = current->next;
        temp = (struct History*) malloc(sizeof(struct History));
        temp->sign = sign;
        temp->bigBoard = bigBoardSlot;
        temp->slotChanged = slot;
        temp->next = NULL;
        temp->prev = current;
        current->next = temp;
    }
    
}
void appendNode(struct History **list, struct History **node)
{
    struct History *current = *list;
    if(*list == NULL)
    {
        *list = *node;
    }else
    {
        while(current->next != NULL)
            current=current->next;
        current->next = *node;
        (*node)->prev = current;
    }

}
struct History* popMove(struct History **list)
{
    struct History *current = *list;
    if(*list != NULL)
    {
        while(current->next != NULL)
            current = current->next;
        if(current->prev == NULL)
        {
            *list = NULL;
            return current;
        }else
        {
            current->prev->next = NULL;
            current->prev = NULL;

            return current;
        }
        
    }else
    {
        printf("List is empty\n");
        return NULL;
    }
    
}
struct History* lastOf(struct History **list)
{
    struct History *current = *list;
    if(*list != NULL)
    {
        while(current->next != NULL)
            current = current->next;
        return current;
    }else
    {
        printf("List is empty\n");
        return NULL;
    }
}
void freeMoves(struct History **list)
{
    struct History *temp, *current = *list;
    if(* list !=NULL)
    {
        while(current->next !=NULL)
        {
            temp = current;
            current = current->next;
            temp->prev = NULL;
            temp->next = NULL;
            current->prev=NULL;
            free(temp);
        }
        free(current);
        *list = NULL;
    }
}
void deleteAfter(struct History **list)
{
    struct History *temp, *current = *list;
    if(current->next != NULL)
    {
        temp = current->next;
        current->next = NULL;
        freeMoves(&temp);
    }
    
}

void displayHistory(struct History *list)
{
    while(list != NULL)
    {
        printf("%c was played at location %d in the table at %d \n", list->sign, list->slotChanged+1, list->bigBoard+1);
        list = list->next;
    }
}
void printScore(struct Player one, struct Player two)
{
    printf("\n\n\n\n\n\nScore X (%s): %d | Score O (%s): %d\n\n", one.name, one.score, two.name, two.score);
}

char CheckBoard(struct Board **bigBoard)
{
    int index =0;
    for(int x = 0; x < 3; x++)
    {
        //horizontal check
        if ((bigBoard[index]->won == bigBoard[index + 1]->won) && (bigBoard[index + 1]->won == bigBoard[index +2]->won) && (bigBoard[index]->won != 'f')) {
            printf("%c wins\n", bigBoard[index]->won);
            return bigBoard[index]->won;
        }
        //vertical check
        if((bigBoard[x]->won == bigBoard[x+3]->won) && (bigBoard[x+6]->won== bigBoard[x+3]->won) && (bigBoard[x]->won != 'f'))
        {
             printf("%c wins\n", bigBoard[x]->won);
             return bigBoard[x]->won;
        }
        index += 3;
    }
    //diagonal check
    if((((bigBoard[0]->won == bigBoard[4]->won) && (bigBoard[8]->won == bigBoard[4]->won)) || ((bigBoard[2]->won == bigBoard[4]->won) && (bigBoard[6]->won == bigBoard[4]->won))) && (bigBoard[4]->won != 'f'))
    {
         printf("%c wins\n", bigBoard[4]->won);
             return bigBoard[4]->won;
    }
    return 'n';
}
char CheckTile(char *board)
{
    int index =0;
    for(int x = 0; x < 3; x++)
    {
        //horizontal check
        if ((board[index] == board[index + 1]) && (board[index + 1] == board[index +2])) {
            return board[index];
        }
        //vertical check
        if((board[x] == board[x+3]) && (board[x+6]== board[x+3]))
        {
             return board[x];
        }
        index += 3;
    }
    //diagonal check
    if(((board[0] == board[4]) && (board[8] == board[4])) || ((board[2] == board[4]) && (board[6] == board[4])))
    {
             return board[4];
    }
    return 'n';
}
void Change(char *board, struct History *h, int way)
{
    int index = h->slotChanged;
    char sign = h->sign;
    if(way == 0)
        board[index] = (index+1)+'0';
    else
        board[index] = sign;
    
}
void UndoMove(char *board, struct History **list, struct History **undoMoveList, int way)
{
    struct History *removedMove = popMove(list);
    if(removedMove != NULL)
    {
        Change(board, removedMove, way);
        appendNode(&(*undoMoveList), &removedMove);
        turn -= 2;
    }else
        turn -= 1;
    
    if(*list == NULL && way == 0)
        for(int i = 0; i < SIZEOFBD; i++)
            board[i] = (i+1) + '0';
}
int Move(struct Board **bigBoard, struct Player p, struct History **h)
{
    char player = p.sign;
	char ch[10];
	char *ptr;
    int index =10;
    int correct = 0;
    printf("%c's turn \n", p.sign);
    if(bigBoardIndex != -1)
    {
        if(bigBoard[bigBoardIndex-1]->won != 'f')
            bigBoardIndex = -1;
    }
    while (bigBoardIndex == -1)
    {
        printf("Enter a board location you want to play: ");
        fgets(ch,10,stdin);
        bigBoardIndex = strtol(ch,&ptr,10);
        if(bigBoardIndex<10 && bigBoardIndex>0)
            if(bigBoard[bigBoardIndex-1]->won !='f')
            {
                bigBoardIndex = -1;
                printf("Board is already full\n");
            }
    }

    do
    {
        printf("Enter a square for location %d(99 for undo, 11 for redo): ",bigBoardIndex);
        fgets(ch,10,stdin);
		index = strtol(ch,&ptr,10);

        if(index<10 && index>0)
        {
            char c = index + '0';
            if(bigBoard[bigBoardIndex-1]->board[index-1] == c)
            {
                bigBoard[bigBoardIndex-1]->board[index-1] = player;
                correct = 1;
                char check = CheckTile(bigBoard[bigBoardIndex-1]->board);
                if(check !='n')
                {
                    bigBoard[bigBoardIndex-1]->won = check;
                    bigBoardIndex = -1;
                }else
                    bigBoardIndex = index;
                appendMove(h, bigBoardIndex-1,index-1,player);
            }
            else
            {
                printf("Wrong move\n");
            }
            
        }else
        {
            if(index == 99 || index == 11)
            {
                return index;
            }else
                printf("Wrong move\n");
        }
        
        
    } while (correct == 0);
    return 1;
}
void resetGame(struct Board **bigBoard, struct History **h)
{
    for(int i = 0; i < SIZEOFBD; i++)
    {
        bigBoard[i]->won = 'f';
        for(int j = 0; j < SIZEOFBD; j++)
        {
            bigBoard[i]->board[j] = j+1 +'0';
        }
        
    }
    freeMoves(h);
}
int main()
{
    struct Player one,two;
    struct History *h, *undoList;
    h = NULL;
    undoList = NULL;
    int gameOver = 0, undo =-1;
    char winner;
    struct Board **bigBoard = malloc(sizeof(struct Board*) * SIZEOFBD);
    one.sign = 'X';
    one.score = 3;
    two.sign = 'O';
    two.score = 3;
    printf("Player one name: ");
	fgets(one.name, 256, stdin);
	one.name[strcspn(one.name,"\n")] =0;
    printf("Player two name: ");
    fgets(two.name,256,stdin);
	two.name[strcspn(two.name,"\n")] =0;
    setBigBoard(bigBoard);
    bigBoard[0]->won ='x';
    bigBoard[1]->won ='x';
    bigBoard[2]->won ='x';
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
        printScore(one, two);
        displayBigBoard(bigBoard);
        undo = Move(bigBoard,p,&h);
        winner = CheckBoard(bigBoard);
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
                resetGame(bigBoard,&h);
            }
            else
            {
                printf("Bye\n");
                gameOver =1;
            }
        }
        turn++;
    }
    return 0;
}