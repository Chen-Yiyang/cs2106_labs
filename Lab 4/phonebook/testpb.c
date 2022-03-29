#include <stdio.h>
#include "phonebook.h"

#define ITEMS   5

typedef struct d {
    char *name, *tel;
} TData;

void printResult(char *name) {
    char *result;

    printf("Looking for %s ", name);
    result = findPerson(name);
    if(result == NULL)
	    printf("NOT FOUND\n");
    else
	    printf(" Number is %s\n", result);
}

int main() {
    TData data[ITEMS] = {{"Fred Astaire", "95551234"}, {"Jean Valjean",
    "95558764"}, {"Gal Gadot", "95551123"}, {"Aiken Dueet", "95558876"},
    {"Victor Hugo", "95524601"}};

    int i;

    for(i = 0; i<ITEMS; i++){
	    printf("Adding %s, phone number %s\n", data[i].name, data[i].tel);
	    addPerson(data[i].name, data[i].tel);
    }

    printf("\nNow retrieiving stored data.\n");
    char *result;

    for(i=0; i<ITEMS; i++) {
	    printResult(data[i].name);
    }

    printf("\nRetrieving unknown person.\n");
    printResult("Wee Tu Loh");

    printf("\nPrinting entire phonebook.\n");
    print_phonebook();

    printf("\nDeleting Aiken Dueet.\n");
    delPerson("Aiken Dueet");
    print_phonebook();

    printf("\nDeleting Victor Hugo.\n");
    delPerson("Victor Hugo");
    print_phonebook();

    printf("\nDeleting entire phone book.\n");
    delPhonebook();
    print_phonebook();
}
