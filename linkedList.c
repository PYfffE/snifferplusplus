#include "linkedList.h"
#include <stdio.h>
#include <windows.h>
#include <strsafe.h>
#include <Shlobj.h>

HANDLE heap = NULL;

void initHeap() {
    if (heap == NULL) {
        heap = GetProcessHeap();
    }
}

Node* createnode(WCHAR* data, DWORD size) {
    initHeap();
    Node* newNode = (Node*)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(Node));
    if (!newNode) {
        return NULL;
    }
    //newNode->wcFilePath = data;
    newNode->dwfileSize = size;
    StringCchCopy(newNode->wcFilePath, MAX_PATH, data);
    newNode->next = NULL;
    return newNode;
}

List* makelist() {
    initHeap();
    List* list = (List*)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(List));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    return list;
}

void add(WCHAR* data, DWORD size, List* list) {
    Node* current = NULL;
    if (list->head == NULL) {
        list->head = createnode(data, size);
    }
    else {
        current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = createnode(data, size);
    }
}

void display(List* list) {
    Node* current = list->head;
    if (list->head == NULL)
        return;

    for (; current != NULL; current = current->next) {
        wprintf(L"%s\n", current->wcFilePath);
    }
}

void destroy(List* list) {
    Node* current = list->head;
    Node* next = current;
    while (current != NULL) {
        next = current->next;
        HeapFree(heap, 0, current);
        current = next;
    }
    HeapFree(heap, 0, list);
}