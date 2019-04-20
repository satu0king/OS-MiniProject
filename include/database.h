
#ifndef _DATABASE_H
#define  _DATABASE_H

#include "util.h"

void toBeginning(int fd);
int lock(int fd, short lock_type, int size, int record);

int createAccount(struct Account *account);
int createTransaction(struct Transaction *transaction);
int getTransactions(int account_id, struct Transaction transactions[], int maxcount);
int getUsers(struct User users[], int maxcount);
int getAccounts(struct Account accounts[], int maxcount);
int createUser(struct User *user);

int getUser(char email[], struct User *user);
int getUserById(int user_id, struct User *user);
int getAccount(int account_id, struct Account *account);
int changeAccountBalance(int account_id, float amount);
int saveUser(struct User * user);
int deleteUser(int user_id);

#endif
