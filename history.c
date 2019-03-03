#include <stdio.h>
#include <stdlib.h>

struct History
{
    int slotChanged;
    char sign;
    struct History *next, *prev;
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
void freeMoves(struct History **list)
{
    struct History *temp, *current = *list;
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
void UndoMove(struct History **list, struct History **undoMoveList)
{

    struct History *removedMove = popMove(list);
    if(removedMove != NULL)
        appendNode(&(*undoMoveList), &removedMove);
    
    
}

void displayHistory(struct History *list)
{
    if(list == NULL)
    {
        printf("empty\n");
    }
    while(list != NULL)
    {
        printf("%c was played at location %d \n", list->sign, list->slotChanged+1);
        list = list->next;
    }
    
    
}

int main()
{
    struct History *list;
    list = NULL;
    struct History *undo;
    undo = NULL;
    appendMove(&list,1,'a');
    appendMove(&list,3,'f');
    appendMove(&list,5,'j');
    displayHistory(list);
    UndoMove(&list, &undo);
    UndoMove(&list, &undo);
    UndoMove(&list, &undo);
    printf("\n");
    displayHistory(list);
    displayHistory(undo);
    UndoMove(&undo, &list);
    printf("l\n");
    displayHistory(list);
     printf("und\n");
    displayHistory(undo);

    //appendMove(&list,1,'a');
    //displayHistory(list);
}