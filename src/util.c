#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void repeat_char(unsigned int cnt, char ch) {
    char buffer[cnt + 1];
    /*assuming you want to repeat the c character 30 times*/
    memset(buffer,ch,cnt);
    buffer[cnt]='\0';
    printf("%s\n",buffer);
}

void printUsers(struct User * u, int count){
    int len = 119;
    repeat_char(len, '-');
    printf("| %15s | %-20s | %-20s | %-15s | %-15s | %15s |\n",
    "User Id",
    "Name",
    "Email",
    "Account Type",
    "Password",
    "Account Id"
    );
    repeat_char(len, '-');
    for(int i = 0; i<count; i++)
        printf("| %15d | %-20s | %-20s | %-15s | %-15s | %15d |\n",
            u[i].id,
            u[i].name,
            u[i].email,
            u[i].accountType == Normal ?  "Normal" : "Admin",
            u[i].password,
            u[i].account_id );
    repeat_char(len, '-');
}

void printUser(struct User * u){
    printUsers(u, 1);
}

void printAccounts(struct Account *a, int count){
    int len = 42;
    repeat_char(len, '-');
    printf("| %15s | %20s |\n",
    "Account Id",
    "Balance"
    );
    repeat_char(len, '-');
    for(int i = 0; i<count; i++)
        printf("| %15d | %20.2lf |\n",
            a[i].id,
            a[i].balance );
    repeat_char(len, '-');
}

void printAccount(struct Account *a){
    printAccounts(a, 1);
}


void printTransactions(struct Transaction *t, int count){

    int len = 96;
    repeat_char(len, '-');
    printf("| %15s | %15s | %-20s | %15s | %15s |\n",
    "Transaction Id",
    "User Id",
    "Name",
    "Opening Balance",
    "Closing Balance"
    );
    repeat_char(len, '-');
    for(int i=0; i<count; i++){
        printf("| %15d | %15d | %-20s | %15.2lf | %15.2lf |\n",
            t[i].id,
            t[i].user_id,
            t[i].name,
            t[i].opening_balance,
            t[i].closing_balance );
    }
    repeat_char(len, '-');
}


void printTransaction(struct Transaction * t) {
    printTransactions(t, 1);
}
