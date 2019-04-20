#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include "datastructures.h"
#include "constants.h"

void repeat_char(unsigned int cnt, char ch);

void printUsers(struct User * u, int count);

void printUser(struct User * u);

void printAccounts(struct Account *a, int count);
void printAccount(struct Account *a);

void printTransactions(struct Transaction *t, int count);

void printTransaction(struct Transaction * t);

#endif /* end of include guard:  */
