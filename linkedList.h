#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <windows.h>

typedef struct node {
    WCHAR wcFilePath[260];
    DWORD dwfileSize;
    struct node* next;
} Node;

typedef struct list {
    Node* head;
} List;

Node* createnode(WCHAR* data, DWORD size);
List* makelist();
void add(WCHAR* data, DWORD size, List* list);
void destroy(List* list);

#endif