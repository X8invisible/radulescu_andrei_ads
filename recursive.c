#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZEOFBD 9

int turn =0;
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
    if(*bigBoard == NULL)
        *bigBoard = (struct Board*) malloc(sizeof(struct Board)*SIZEOFBD);
    for(int i = 0; i < SIZEOFBD; i++)
    {
        bigBoard[i] = (struct Board*) malloc(sizeof(struct Board));
        for(int j = 0; j < SIZEOFBD; j++)
        {
            bigBoard[i]->board[j] = j+1 +'0';
        }
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
                printf("||  %c  |  %c  |  %c  |", bigBoard[j]->board[row*3], bigBoard[j]->board[row*3+1], bigBoard[j]->board[row*3+2]);
            else
                printf("||  %c  |  %c  |  %c  |", bigBoard[j]->won, bigBoard[j]->won, bigBoard[j]->won);
            
        }
        printf("|\n\n");
        row++;
        if(row == 3)
        {
            row =0;
            i +=3;
            printf("___________________________________________________________________\n\n");
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
    printf("\n\n\n\n\n\nScore X (%s): %d | Score O (%s): %d\n\n",one.name,one.score,two.name,two.score);
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
    if((((bigBoard[0]->won == bigBoard[4]->won) && (bigBoard[8]->won == bigBoard[4]->won)) || ((bigBoard[2]->won == bigBoard[4]->won) && (bigBoard[6]->won == bigBoard[4]->won))) && (bigBoard[4]->won != 'f'))
    {
         printf("%c wins\n", bigBoard[4]->won);
             return bigBoard[4]->won;
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
/*int Move(char *board, struct Player p, struct History **h)
{
    char player = p.sign;
	char ch[10];
	char *ptr;
    int index =10;
    int correct = 0;
    do
    {
        printf("%c's turn \n", p.sign);
        printf("Enter a square (99 for undo, 11 for redo): ");
		fgets(ch,10,stdin);
		index = strtol(ch,&ptr,10);
        if(index<10 && index>0)
        {
            char c = index + '0';
            if(board[index -1] == c)
            {
                board[index -1] = player;
                correct = 1;
                appendMove(h, index-1,player);
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
*/
void resetGame(char *board, struct History **h)
{
    for(int i = 0; i < SIZEOFBD; i++)
    {
        board[i] = (i+1) + '0';
    }
    freeMoves(h);
}
int main()
{
    struct Player one, two;
    //struct History *h, *undoList;
    //h = NULL;
    //undoList = NULL;
    int gameOver = 0; //undo =-1;
    //char winner;
    struct Board *bigBoard;
    bigBoard = NULL;
    one.sign = 'X';
    one.score = 0;
    two.sign = 'O';
    two.score = 0;
    printf("Player one name: ");
	fgets(one.name,256,stdin);
	one.name[strcspn(one.name,"\n")] =0;
    printf("Player two name: ");
    fgets(two.name,256,stdin);
	two.name[strcspn(two.name,"\n")] =0;
    
    setBigBoard(&bigBoard);
    (&bigBoard)[6]->won ='X';
    (&bigBoard)[7]->won ='O';
    (&bigBoard)[8]->won ='X';
    displayBigBoard(&bigBoard);
    CheckBoard(&bigBoard);
    return 0;
}