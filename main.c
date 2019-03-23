#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZEOFBD 9
//bigboard stores the index the player sees, aka. bigboardIndex = 1 corresponds to the first board
int turn =0,bigBoardIndex =-1;
//linked list for storing the history of the moves, every node stores a move and all the data associated(the slots and the sign)
struct History
{
    int bigBoard;
    int slotChanged;
    char sign;
    struct History *next, *prev;
};
//player struct for storing the name and the score
struct Player
{
    char sign;
    char name[256];
    int score;
};
//the 3*3(stored in 1D as 9) big board that stores the smaller 'normal' tic-tac-toe boards
struct Board
{
    char board[SIZEOFBD];
    char won;
};
//initialises the big board
void setBigBoard(struct Board **bigBoard)
{
    for(int i = 0; i < SIZEOFBD; i++)
    {
        bigBoard[i] = malloc(sizeof(struct Board));
        //every 'cell' in the small boards needs to be initialised
        for(int j = 0; j < SIZEOFBD; j++)
        {
            bigBoard[i]->board[j] = j+1 +'0';
        }
        printf("\n");
        bigBoard[i]->won = 'f';
    }
}

//displays in the console the game board
void displayBigBoard(struct Board **bigBoard)
{
    printf("\n\n");
    int i =0, row =0;
    while(i < SIZEOFBD)
    {
        for(int j = i; j < i+3; j++)
        {
            if(bigBoard[j]->won == 'f')
                printf(" [%d] |  %c  |  %c  |  %c  | ",j+1, bigBoard[j]->board[row*3], bigBoard[j]->board[row*3+1], bigBoard[j]->board[row*3+2]);
            else
                printf(" [%d] |  %c  |  %c  |  %c  | ",j+1, bigBoard[j]->won, bigBoard[j]->won, bigBoard[j]->won);
            
        }
        printf("\n\n");
        row++;
        if(row == 3)
        {
            row =0;
            i +=3;
            printf("-----------------------------------------------------------------------------\n\n");
        }
        
    }
    
}
//appends a NEW node to the list
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
//appends an EXISTING node to the list
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
//removes last node in a list
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
//cleans a list
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
//displays the contents of the list(used mostly for debugging purposes)
void displayHistory(struct History *list)
{
    while(list != NULL)
    {
        printf("%c was played at location %d in the board at %d \n", list->sign, list->slotChanged+1, list->bigBoard);
        list = list->next;
    }
}
void printScore(struct Player one, struct Player two)
{
    printf("\n\n\n\n\n\nScore X (%s): %d | Score O (%s): %d\n\n", one.name, one.score, two.name, two.score);
}
//checks if a player has won the game, returns the player's sign or 'n' if no one has won
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
//counts how many moves are left(in case the game is a draw)
int MovesLeft(struct Board **bigBoard)
{
    int moves = 0;
    for(int i  =0; i < SIZEOFBD; i++)
    {
        for(int j = 0; j < SIZEOFBD; j++)
        {
            if(bigBoard[i]->board[j] == (j+1 +'0'))
                moves++;
        }
        
    }
    return moves;
}
//checks if a player has won a small board
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
//used by the undo/redo function, the 'way' variable specifies if it's an undo(0) or a redo(1)
void Change(struct Board **bigBoard, struct History *h, int way)
{
    int index = h->bigBoard;
    int slot = h->slotChanged;
    char sign = h->sign;
    if(way == 0)
        {
            bigBoard[index-1]->board[slot] = (slot+1)+'0';
            bigBoard[index-1]->won = 'f';
            bigBoardIndex = index;
        }
    else
        {
            bigBoard[index-1]->board[slot] = sign;
            bigBoardIndex = slot+1;
        }
    
}
//the undo/redo function, the 'way' variable specifies if it's an undo(0) or a redo(1)
//listFrom is where the move will be removed from and added to listTo
void UndoMove(struct Board **bigBoard, struct History **listFrom , struct History **listTo, int way)
{
    struct History *removedMove = popMove(listFrom);
    if(removedMove != NULL)
    {
        Change(bigBoard, removedMove, way);
        appendNode(listTo, &removedMove);
    }else
        //if undo/redo is unsuccesful, turn doesn't change
        turn -= 1;
            
}
//used for adding moves to the board
int Move(struct Board **bigBoard, struct Player p, struct History **h)
{
    char player = p.sign;
    //used to read player choice from the keyboard
	char ch[10];
	char *ptr;
    int index =10;
    int correct = 0;
    printf("%c's turn \n", p.sign);
    //if a board is selected, check if it's already won
    if(bigBoardIndex != -1)
    {
        if(bigBoard[bigBoardIndex-1]->won != 'f')
            bigBoardIndex = -1;
    }
    //if no board selected, one needs to be selected before making a move
    while (bigBoardIndex == -1)
    {
        printf("Enter a board location you want to play: ");
        fgets(ch,10,stdin);
        bigBoardIndex = strtol(ch,&ptr,10);
        //checks if board is a valid choice
        if(bigBoardIndex<10 && bigBoardIndex>0)
        {  
            //checks if board hasn't already been won
            if(bigBoard[bigBoardIndex-1]->won !='f')
            {
                bigBoardIndex = -1;
                printf("Board is already full\n");
            }
        }
        else
        {
            printf("Board doesn't exist\n");
            bigBoardIndex = -1;
        }
        
    }
    //for selecting a valid move inside a small board
    do
    {
        printf("Enter a square for location %d(99 for undo, 11 for redo, 55 to end game in a tie): ",bigBoardIndex);
        fgets(ch,10,stdin);
		index = strtol(ch,&ptr,10);

        if(index<10 && index>0)
        {
            //transforms the player choice to a character
            char c = index + '0';
            //checks if the board already has a sign in it
            if(bigBoard[bigBoardIndex-1]->board[index-1] == c)
            {
                //change the tile to the player's sign
                bigBoard[bigBoardIndex-1]->board[index-1] = player;
                correct = 1;
                //append the choice to the move history
                appendMove(h, bigBoardIndex,index-1,player);
                //checks if the small board has been won
                char check = CheckTile(bigBoard[bigBoardIndex-1]->board);
                //if small board has been won, next player chooses big board
                if(check !='n')
                {
                    bigBoard[bigBoardIndex-1]->won = check;
                    bigBoardIndex = -1;
                }
                //if small board not won, next move will happen in the board which corresponds to the tile where last move was played in
                //e.g. if a move was placed in board 5 in tile 3, next move will happen in board 3
                else
                    bigBoardIndex = index;
            }
            else
            {
                printf("Wrong move\n");
            }
            
        }else
        {
            //if player has chosen something other than making a move, return it to main function
            if(index == 99 || index == 11 || index == 55)
            {
                return index;
            }else
                printf("Wrong move\n");
        }
        
        
    } while (correct == 0);
    return 1;
}
//starts a new game
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
    //the two players
    struct Player one,two;
    //list that contains past moves and a list that contains the undone moves
    struct History *h, *undoList;
    h = NULL;
    undoList = NULL;
    int gameOver = 0, undo = -1, moves = 0;
    char winner, choice;
    //allocates memory for the big board
    struct Board **bigBoard = malloc(sizeof(struct Board*) * SIZEOFBD);
    //sets up the players
    one.sign = 'X';
    one.score = 0;
    two.sign = 'O';
    two.score = 0;
    //reads the player names from the terminal
    printf("Player one name: ");
	fgets(one.name, 256, stdin);
	one.name[strcspn(one.name,"\n")] =0;
    printf("Player two name: ");
    fgets(two.name,256,stdin);
	two.name[strcspn(two.name,"\n")] =0;
    setBigBoard(bigBoard);
    while(gameOver == 0)
    {
        //decides whose turn it is(x starts, and then the loser will always start on consecutive games)
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
        moves = MovesLeft(bigBoard);
        if(undo == 99)
        {   
            UndoMove(bigBoard, &h, &undoList, 0);
        }
        if(undo == 11)
        {
            UndoMove(bigBoard, &undoList, &h, 1);
        }
        if(undo == 1)
            freeMoves(&undoList);
        displayHistory(h);
        printf("undo list: \n");
        displayHistory(undoList);
        if(winner == 'X')
            one.score++;

        if (winner =='O')
            two.score++;

        if(moves ==0 || undo == 55)
            winner = 't';

        if(winner != 'n')
        {
            printf("Play another?(y/n): ");
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