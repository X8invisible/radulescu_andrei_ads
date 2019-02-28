#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZEOFBD 9

int turn =0;
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

void appendMove(struct History **list, int slot, char sign)
{
    struct History *temp, *current = *list;
    if(*list == NULL)
    {
        *list = (struct History*)malloc(sizeof(struct History));
        (*list)->prev = NULL;
        (*list)->slotChanged = slot;
        (*list)->sign = sign;
        (*list)->next = NULL;
    }else
    {
        while(current->next !=NULL)
            current = current->next;
        temp = (struct History*) malloc(sizeof(struct History));
        temp->sign = sign;
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
        printf("%c was played at location %d \n", list->sign, list->slotChanged+1);
        list = list->next;
    }
}
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
    }
    if(*list == NULL && way == 0)
        for(int i = 0; i < SIZEOFBD; i++)
            board[i] = (i+1) + '0';
    turn -= 2;
}
int Move(char *board, struct Player p, struct History **h)
{
    char player = p.sign;
    int index =10;
    int correct = 0;
    do
    {
        printf("Enter a square (99 for undo, 0 for redo): ");
        scanf("%d", &index);
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
            if(index == 99 || index == 0)
            {
                return index;
            }else
                printf("Wrong move\n");
        }
        
        
    } while (correct == 0);
    return 1;
}

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
    struct History *h, *undoList;
    h = NULL;
    undoList = NULL;
    one.sign = 'x';
    one.score = 0;
    two.sign = '0';
    two.score = 0;
    printf("Player one name: ");
    scanf("%s", one.name);
    printf("Player two name: ");
    scanf("%s", two.name);
    int gameOver = 0, undo =-1;
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
        undo = Move(&*board, p, &h);
        if(undo == 99)
        {   
            UndoMove(&*board,&h, &undoList,0);
            //Change(&*board, lastOf(&undoList),0);
        }
        if(undo == 0)
        {
            UndoMove(&*board,&undoList, &h,1);
            //Change(&*board, lastOf(&h),1);
        }
        if(undo == 1)
            freeMoves(&undoList);
        displayHistory(h);
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
                resetGame(&*board,&h);
            }
            else
            {
                printf("Bye\n");
                gameOver =1;
            }
            
            
            
        }
        
        turn += 1;
    }
    
    

    return 0;
}