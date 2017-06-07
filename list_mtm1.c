#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "list_mtm1.h"
#include "test_utilities.h"
#include <string.h>


void printList(List listOfStrings);

typedef struct node{
    void* data;
    struct node* next;
}*Node;

 struct list_t{
    Node head;
    CopyListElement copyFunction;
    FreeListElement freeFunction;
    Node iterator;
};

/*destroys a node using the given free function*/
static void destroyNode (FreeListElement freeFunction ,Node node_to_destroy){
    freeFunction(node_to_destroy->data);
    node_to_destroy->data = NULL;
  //  node_to_destroy->next = NULL;
    free(node_to_destroy);
    node_to_destroy = NULL;
}

/*gets a ListElement and creates a node using the given copy function*/
static Node createNode (ListElement element, CopyListElement copyFunction){
    Node new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = copyFunction(element);
    if (new_node->data == NULL)
        return NULL;
    new_node->next = NULL;
    return new_node;
}


List listCreate(CopyListElement copyElement, FreeListElement freeElement){
    if (copyElement == NULL || freeElement == NULL){
        return NULL;
    }
    List list = malloc (sizeof(List));
    if (list == NULL)
        return NULL;
    list->head = createNode("head",copyElement);
    if (list->head == NULL)
        return NULL;
    printf("head is %s", (char*)list->head->data);
   // list->iterator = malloc(sizeof(Node));
    list->iterator = NULL;
    list->copyFunction = copyElement;
    list->freeFunction = freeElement;
    return list;
}


void listDestroy(List list){
    if (list != NULL){
        listClear(list);
        destroyNode(list->freeFunction,list->head);
        list->head = NULL;
        list->copyFunction = NULL;
        list->freeFunction = NULL;
        free(list);
        //list = NULL;
    }

}
/*sets the new list's iterator to the same node pointed by the original list*/
static void placeNewIterator (List original_list, List new_list){
    assert ((original_list == NULL) || (new_list == NULL));
    Node inner_iterator = original_list->head->next;
    new_list->iterator = new_list->head->next;
    while (inner_iterator != original_list->iterator){
        inner_iterator = inner_iterator->next;
        new_list->iterator = new_list->iterator->next;
    }
}

List listCopy(List list) {
    if (list == NULL)
        return NULL;
    List new_list = listCreate(list->copyFunction, list->freeFunction);
    if (new_list == NULL)
        return NULL;
    Node inner_iterator = list->head;
    if (inner_iterator->next != NULL) {
        inner_iterator = inner_iterator->next;
    }
    while (inner_iterator != NULL) {
        listInsertLast(new_list, inner_iterator->data);
        inner_iterator = inner_iterator->next;
    }
    placeNewIterator (list, new_list);
    return new_list;
}



List listFilter(List list, FilterListElement filterElement, ListFilterKey key){
    if ((list == NULL) || (filterElement == NULL))
        return NULL;
    List new_list = listCreate(list->copyFunction, list->freeFunction);
    if (new_list == NULL)
        return NULL;
    Node inner_iterator = list->head;
    if (inner_iterator->next != NULL) {
        inner_iterator = inner_iterator->next;
    }
    while (inner_iterator != NULL) {
        if (filterElement(inner_iterator->data, key)) {
            listInsertLast(new_list, inner_iterator->data);
        }
        inner_iterator = inner_iterator->next;
    }
    listGetFirst (new_list);
    return new_list;
}





int listGetSize(List list) {
    if (list == NULL)
        return -1;
    int counter = 0;
    Node inner_iterator = list->head->next;
    if (inner_iterator != NULL) {
        counter++;
        while (inner_iterator->next != NULL) {
            inner_iterator = inner_iterator->next;
            counter++;
        }
    }
    return counter;
}


ListElement listGetFirst(List list) {
    if (list == NULL)
        return NULL;
    if (list->head->next == NULL)
        return NULL;
    list->iterator = list->head->next;
    return list->iterator->data;
}


 ListElement listGetNext(List list){
     if (list == NULL)
         return NULL;
     if(list->iterator == NULL){
         return NULL;
     }
     list->iterator = list->iterator->next;
     if (list->iterator == NULL)
         return NULL;
     return list->iterator->data;
}

ListElement listGetCurrent(List list){
    if (list == NULL)
        return NULL;
    if(list->iterator == NULL){
        return NULL;
    }
    return list->iterator->data;
}

ListResult listInsertFirst(List list, ListElement element){
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    Node new_node = createNode(element,list->copyFunction);
    if (new_node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    new_node->next = list->head->next;
    list->head->next = new_node;
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element){
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    Node new_node = createNode(element,list->copyFunction);
    if (new_node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    Node inner_iterator;
    if(list->head->next == NULL){
        inner_iterator = list->head;
    }
    else
         inner_iterator = list->head->next;
    while (inner_iterator->next != NULL) {
        inner_iterator = inner_iterator->next;
    }

    inner_iterator->next = new_node;
    return LIST_SUCCESS;
    }


ListResult listRemoveCurrent(List list){
    if (list == NULL)
        return LIST_NULL_ARGUMENT;
    if (list->iterator == NULL)
        return LIST_INVALID_CURRENT;
    Node inner_iterator = list->head;
    while ((inner_iterator->next != NULL) &&(inner_iterator->next != list->iterator)){
        inner_iterator = inner_iterator->next;
    }
    inner_iterator->next = list->iterator->next;
    destroyNode(list->freeFunction, list->iterator);
    list->iterator = NULL;
    return LIST_SUCCESS;
}


ListResult listClear(List list){
    if (list == NULL)
        return LIST_NULL_ARGUMENT;
    ListResult result;
    while (listGetFirst(list) != NULL) {
        result = listRemoveCurrent(list);
        if (result !=  LIST_SUCCESS) {
            return result;
        }
    }
    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element){
    if (list == NULL)
        return LIST_NULL_ARGUMENT;
    if (list->iterator == NULL)
        return LIST_INVALID_CURRENT;
    Node node_to_insert = createNode(element,list->copyFunction);
    if (node_to_insert == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    Node inner_iterator = list->head;
    while ((inner_iterator->next != NULL) &&(inner_iterator->next != list->iterator)){
        inner_iterator = inner_iterator->next;
    }
    node_to_insert->next = list->iterator;
    inner_iterator->next = node_to_insert;
    return LIST_SUCCESS;
}


ListResult listInsertAfterCurrent(List list, ListElement element){
    if (list == NULL)
        return LIST_NULL_ARGUMENT;
    if (list->iterator == NULL)
        return LIST_INVALID_CURRENT;
    Node node_to_insert = createNode(element,list->copyFunction);
    if (node_to_insert == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    node_to_insert->next = list->iterator->next;
    list->iterator->next = node_to_insert;
    return LIST_SUCCESS;
}

static Node swap (Node low, Node high){

    low->next = high->next;
    high->next = high->next->next;
    low->next->next = high;
    low = low->next;
    return low;
}
/*the flag will be true if 2 nodes were swapped.*/
ListResult listSort(List list, CompareListElements compareElement) {
    if ((list == NULL) || (compareElement == NULL)) {
        return LIST_NULL_ARGUMENT;
    }
    bool swapped_flag = true;
    if (list->head->next != NULL) {
        Node low_iterator = list->head;
        Node high_iterator = list->head->next;
        while (swapped_flag == true) {
            swapped_flag = false;
            low_iterator = list->head;
            high_iterator = list->head->next;
            while (high_iterator->next != NULL ) {
                if (compareElement(low_iterator->next->data, high_iterator->next->data) > 0) {
                    low_iterator = swap(low_iterator, high_iterator);
                    swapped_flag = true;
                }
                else{
                    low_iterator = low_iterator->next;
                    high_iterator = high_iterator->next;
                }

            }
        }
    }
    return LIST_SUCCESS;
}





/*TESTS!!!!!!!!*/

/*
static ListElement copyString(ListElement str){
    assert(str);
    char* copy = malloc(strlen(str)+1);
    return copy != NULL ? strcpy(copy, str) : NULL;
}

static void freeString(ListElement str){
    free(str);
}


void printList(List listOfStrings) {
    printf("\n\nlist is: \n");
    LIST_FOREACH(char*, str, listOfStrings) {
        printf("%s\n", str);
    }
    printf("\n");
}

static bool isLongerThan(ListElement element1,ListElement element2) {
    char* string1 = element1;
    char* string2 = element2;
    return strlen(string1) > strlen(string2);
}



static bool testListSort() {
    char* a[7] = {"aaaaaaa","bb","ccc", "dddddddd", "I", "h", "tbmhekupshv"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <7; ++i){
        listInsertLast(list,a[i]);
    }
    printList(list);

    listSort(list, (CompareListElements)isLongerThan);
    printList(list);

    return true;
}


int main() {
    RUN_TEST(testListSort);
    return 0;
}*/