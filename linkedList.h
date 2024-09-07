#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <windows.h>

// Структуры
typedef struct node {
    WCHAR wcFilePath[260];
    DWORD dwfileSize;
    struct node* next;
} Node;

typedef struct list {
    Node* head;
} List;

// Прототипы функций
Node* createnode(WCHAR* data, DWORD size);
List* makelist();
void add(WCHAR* data, DWORD size, List* list);
void display(List* list);
void reverse(List* list);
void reverse_using_two_pointers(List* list);
void destroy(List* list);

#endif