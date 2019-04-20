#pragma once
#include "datastructures.h"

#define NEW_LINE printf("\n")

int connectSocket(int portno);

int login(int *session_id, enum AccountType *loginType) ;

int getNextOperation();

void balanceEnquiry(int session_id, int sd);

void exitSession(int session_id, int sd);

void getUserInformationFromUI(struct User *user);

void addUser(int session_id, int sd, bool joint);

void modifyUser(int session_id, int sd);

void deleteUser(int session_id, int sd);

void deposit(int session_id, int sd);

void withdraw(int session_id, int sd);

void changePassword(int session_id, int sd);

void viewDetails(int session_id, int sd);

void viewAllUsers(int session_id, int sd);

int main();
