#ifndef _GLIST_H_
#define _GLIST_H_

#define PRT_SIZE
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct glist_s{
    void* vptr;
    struct glist_s *next;
}glist_t;

typedef struct glist_head_s
{
	glist_t* child;
	uint32_t len;
}glist_head_t;


glist_head_t* glist_newGListHead();
void glist_freeGListHead(glist_head_t **head);
glist_t* glist_newVoidList();
void glist_freeVoidList(glist_t** head);
// void glist_addNode(glist_t **head, void* vptr);
void glist_addNode(glist_head_t *head, void* vptr);
void glist_removeNode(glist_head_t *head, void* vptr);
void glist_dumpStr(glist_head_t *head);

void* glist_getNodeByIndex(glist_head_t *head, size_t index);
bool glist_getUInt32ByIndex(glist_head_t *head, size_t index, uint32_t *v);
bool glist_getInt32ByIndex(glist_head_t *head, size_t index, int32_t *v);
bool glist_getMacByIndex(glist_head_t *head, size_t index, unsigned char* mac);

#endif