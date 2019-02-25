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


int main()
{
    struct History *list;
    list = NULL;
    displayHistory(list);
    appendMove(&list,1,'a');
    appendMove(&list,3,'f');
    appendMove(&list,5,'j');
    displayHistory(list);
    //printf("%c", list->next->sign);
    freeMoves(&list);
    appendMove(&list,1,'a');
    displayHistory(list);
}