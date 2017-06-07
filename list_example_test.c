#include "test_utilities.h"
#include "../list_mtm/list_mtm1.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static ListElement copyString(ListElement str){
	assert(str);
	char* copy = malloc(strlen(str)+1);
	return copy != NULL ? strcpy(copy, str) : NULL;
}

static void freeString(ListElement str){
	free(str);
}

static bool isLongerThan(ListElement element,ListFilterKey number) {
	char* string = element;
    return strlen(string) > *(int*)number;
}

static bool testListCreate() {
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(copyString,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,freeString) == NULL);

	return true;
}
static bool testListFilter() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	int key = 5;
	List filtered = listFilter(list,isLongerThan, &key);
	ASSERT_TEST(listGetSize(filtered) == 1);
	ASSERT_TEST(strcmp(listGetFirst(filtered),a[3])==0);
//	listDestroy(list);
	//listDestroy(filtered);
	return true;
}
static bool testListCopy() {
	char* a[5] = {"yogo","pizza","pasta","cake","cheese"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		listInsertLast(list,a[i]);
	}
	listGetFirst(list);
	listGetNext(list);
	List new_list = listCopy(list);
	// printList(new_list);
	ASSERT_TEST(strcmp(listGetNext(new_list),"pasta")==0);
	//printList(new_list);
	return true;
}

static bool testListGetSize() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ASSERT_TEST(listGetSize(list) == 5);
//	listDestroy(list);
	return true;
}

static bool testListGetFirst() {
	char *a[5] = {"barr", "noa", "NURIT", "Inbar Gil", "hadass and ohad"};
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listGetFirst(list) == NULL);
	for (int i = 0; i < 5; ++i) {
		listInsertFirst(list, a[i]);
	}
	//printList(list);

	ASSERT_TEST(strcmp(listGetFirst(list), "hadass and ohad") == 0);
//	listDestroy(list);
	return true;
}
static bool testListGetNext() {
	char *a[5] = {"barr", "noa", "NURIT", "Inbar Gil", "hadass and ohad"};
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listGetNext(list) == NULL);

	for (int i = 0; i < 5; ++i) {
		listInsertLast(list, a[i]);
	}
	//printList(list);

	ASSERT_TEST(strcmp(listGetFirst(list), "barr") == 0);
	ASSERT_TEST(strcmp(listGetNext(list), "noa") == 0);
	//listDestroy(list);
	return true;
}

static bool testListInsertFirst() {
	char *a[5] = {":)", "beginning", "the", "from", "start"};
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listGetNext(list) == NULL);

	for (int i = 0; i < 5; ++i) {
		listInsertFirst(list, a[i]);
	}
//	printList(list);

	ASSERT_TEST(strcmp(listGetFirst(list), "start") == 0);
	ASSERT_TEST(strcmp(listGetNext(list), "from") == 0);
	//listDestroy(list);
	return true;
}

static bool testListInsertLast() {
	char *a[5] = {"I", "Love", "MATAM", "234122", "<3"};
	List list = listCreate(copyString, freeString);
	for (int i = 0; i < 5; ++i) {
		listInsertLast(list, a[i]);
	}
	//printList(list);

	ASSERT_TEST(strcmp(listGetFirst(list), "I") == 0);
	ASSERT_TEST(strcmp(listGetNext(list), "Love") == 0);
	//listDestroy(list);
	return true;
}


static bool testListInsertBeforeCurrent() {
	char *a[5] = {"I", "Love", "MATAM", "234122", "<3"};
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listInsertBeforeCurrent(list, "HI") == LIST_INVALID_CURRENT);
	for (int i = 0; i < 5; ++i) {
		listInsertLast(list, a[i]);
	}
	//printList(list);
	listGetFirst(list);
	listGetNext(list);
	ASSERT_TEST(listInsertBeforeCurrent(list, "dont") == LIST_SUCCESS);
	//listDestroy(list);
	//printList(list);
	return true;
}

static bool testListInsertAfterCurrent() {
	char *a[5] = {"I", "Love", "MATAM", "234122", "<3"};
	List list = listCreate(copyString, freeString);
	ASSERT_TEST(listInsertAfterCurrent(list, "HI") == LIST_INVALID_CURRENT);
	for (int i = 0; i < 5; ++i) {
		listInsertLast(list, a[i]);
	}
	printList(list);
	listGetFirst(list);
	ASSERT_TEST(listInsertAfterCurrent(list, "dont") == LIST_SUCCESS);
	//listDestroy(list);
	//printList(list);
	return true;
}


static bool testListRemoveCurrent() {
	char *a[5] = {"I", "Love", "MATAM", "234122", "<3"};
	List list;
	//ASSERT_TEST(listRemoveCurrent(list) == LIST_NULL_ARGUMENT);
	list = listCreate(copyString, freeString);
	ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
	for (int i = 0; i < 5; ++i) {
		listInsertLast(list, a[i]);
	}
	printList(list);
	listGetFirst(list);
	listGetNext(list);
	ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
	listGetFirst(list);
	ASSERT_TEST(strcmp(listGetNext(list), "MATAM") == 0);
	// listDestroy(list);
	//printList(list);

	return true;

}


static bool testListSort() {
	char* a[7] = {"aaaaaaa","bb","ccc", "dddddddd", "I", "h", "tbmhekupshv"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <7; ++i){
		listInsertLast(list,a[i]);
	}
	//printList(list);

	listSort(list, (CompareListElements)isLongerThan);
	//printList(list);
	//listDestroy(list);

	return true;
}

static bool testListClear() {
	char *a[4] = {"Omri", "Keren", "YONKI", "Itay"};
	List list = listCreate(copyString, freeString);
	for (int i = 0; i < 4; ++i) {
		listInsertLast(list, a[i]);
	}
	//printList(list);
	ASSERT_TEST(listClear(list) == LIST_SUCCESS);
	ASSERT_TEST(listGetFirst(list) == NULL);
	//listDestroy(list);
	return true;
}

static bool testListDestroy() {
	char *a[4] = {"Omri", "Keren", "YONKI", "Itay"};
	List list = listCreate(copyString, freeString);
	for (int i = 0; i < 4; ++i) {
		listInsertLast(list, a[i]);
	}
	printList(list);
	//listDestroy(list);
	ASSERT_TEST(listGetFirst(list) == NULL);
	return true;
}

/*static bool testswap() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	printList(list);
	swap (list->head, list->head->next);
	printList(list);
	return true;
}
*/


int main (int argv, char** arc){
	RUN_TEST(testListCreate);
	RUN_TEST(testListFilter);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
	RUN_TEST(testListClear);
	RUN_TEST(testListDestroy);
	return 0;
}

