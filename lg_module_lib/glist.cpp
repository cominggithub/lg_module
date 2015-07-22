#include "glist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mem_alloc.h"
#include "checker.h"

glist_head_t* glist_newGListHead()
{
    glist_head_t* head;
    head = (glist_head_t*)ALLOC_BY_SIZE(sizeof(glist_head_t));
    
    return head;
}

void glist_freeGListHead(glist_head_t **head)
{
    glist_freeVoidList(&(*head)->child);
    FREE((void**)head);
}


glist_t* glist_newVoidList()
{
    return (glist_t*)ALLOC_BY_SIZE(sizeof(glist_t));
}

void glist_freeVoidList(glist_t** head)
{
    if (head == NULL)
    {
        return;
    }

    if (*head == NULL)
    {
        return;
    }

    glist_freeVoidList(&(*head)->next);
    FREE((void**)&((*head)->vptr));
    FREE((void**)head);
}

void glist_addNode(glist_head_t *head, void* vptr)
{
    glist_t *cur;
    glist_t *prev;
    glist_t *new_node;

    RETURN_ON_NULL(head);

    prev = NULL;
    cur = head->child;
    new_node = glist_newVoidList();
    new_node->vptr = vptr;

    /* check duplicately registration */
    while(cur != NULL)
    {
        if (cur->vptr == new_node->vptr)
        {
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    if (prev == NULL)
    {
        head->child = new_node;
    }
    else
    {
        prev->next = new_node;
    }

    head->len++;
}

void glist_removeNode(glist_head_t *head, void* vptr)
{
    glist_t *cur;
    glist_t *prev;

    prev = NULL;
    cur = head->child;

    if (cur == NULL)
        return;

    while(cur != NULL)
    {
        if (cur->vptr == vptr)
        {
            if (prev == NULL)
            {
                head->child = cur->next;
            }
            else
            {
                prev->next = cur->next;
            }
            FREE((void**)cur);
        }
    }
}

void glist_dumpStr(glist_head_t *head)
{
    glist_t *cur;
    cur = head->child;
    
    while(cur)
    {
        pStr((char*)cur->vptr);
        cur = cur->next;
    }
}

void* glist_getNodeByIndex(glist_head_t *head, size_t index)
{
    RETURNV_ON_NULL(head, NULL);
    RETURNV_ON_ZERO(head->len, NULL);

    size_t i;
    glist_t *cur;

    if (head->len < index)
    {
        return NULL;
    }

    cur = head->child;
    for(i=0; i<index; i++)
    {
        cur = cur->next;
    }

    return cur->vptr;
}

bool glist_getUInt32ByIndex(glist_head_t *head, size_t index, uint32_t *v)
{
    char *node;

    node = (char*)glist_getNodeByIndex(head, index);
    if (node == NULL)
        return false;

    //*v = (uint32_t)atoll(node);

    return true;
}

bool glist_getInt32ByIndex(glist_head_t *head, size_t index, int32_t *v)
{
    char *node;

    node = (char*)glist_getNodeByIndex(head, index);
    if (node == NULL)
        return false;

    *v = (uint32_t)atoi(node);

    return true;
}




// bool glist_getMacByIndex(glist_head_t *head, size_t index, unsigned char* mac)
// {
//     char *node;

//     node = (char*)glist_getNodeByIndex(head, index);
//     RETURNV_ON_NULL(node, false);

//     return str_to_mac(node, mac);
// }
