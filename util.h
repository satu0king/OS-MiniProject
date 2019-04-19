#ifndef UTIL_H
#define UTIL_H

#define PORTNO 9000

#define BASE_PATH "DB"
#define USER_DB_PATH "DB/user_db.dat"
#define ACCOUNT_DB_PATH "DB/account_db.dat"
#define TRANSACTION_DB_PATH "DB/transaction_db.dat"
#define METADATA_DB_PATH "DB/metadata.dat"
#define MAXTRANSACTIONS 100

#include <time.h>

enum Action{
    Login,
    Transaction,
    BalanceEnquiry,
    PasswordChange,
    Viewdetails,
    AddAccount,
    DeleteAccount,
    ModifyAccount,
    SearchAccount,
    Exit
};

enum AccountType{
    Admin,
    Normal
};

enum Status{
    Success,
    Unauthorized,
    Failure
};

struct Header{
    enum Action action;
    int session_id;
};

struct Response {
    enum Status status;
};

struct User {
    enum AccountType accountType;
    char name[100];
    char email[100];
    char password[100];
    int id;
    int account_id;
};

struct Account {
    int id;
    double balance;
};

enum TransactionType {
    Deposit,
    Withdraw
};

struct Transaction {
    int account_id;
    int user_id;
    char name[100];
    time_t date;
    double opening_balance;
    double closing_balance;
    enum TransactionType transactionType;
    int id;
};

struct TransactionRequest {
    double amount;
    enum TransactionType transactionType;
};

struct AddUserRequest {
    struct User user;
};


struct LoginRequest{
    char email[100];
    char password[20];
};

struct ChangePasswordRequest{
    char new_password[20];
};

struct LoginResponse{
    enum Status status;
    int session_id;
    enum AccountType loginType;
};

struct BalanceEnquiryResponse{
    enum Status status;
    double balance;
};

struct Metadata{
    int user_id;
    int account_id;
    int transaction_id;
};

struct ViewDetailsResponse{
    enum Status status;
    int transactionDetailsCount;
};

void printUser(struct User * user){
    printf("Name: %s\n", user->name);
    printf("Email: %s\n", user->email);
    printf("AccountType: %d\n", (int)user->accountType);
    printf("User Id: %d\n", (int)user->id);
    printf("Account Id: %d\n", (int)user->account_id);
}

void printAccount(struct Account * account){
    printf("Balance: %lf\n", account->balance);
    printf("Account Id: %d\n", (int)account->id);
}

void printTransaction(struct Transaction * transaction) {
    printf("Transaction Id: %d\n", transaction->id);
    printf("Account Id: %d\n", transaction->account_id);
    printf("User Id: %d\n", transaction->user_id);
    printf("User Name: %s\n", transaction->name);

    char time[100];
    struct tm *t = localtime(&(transaction->date));
    strftime(time, sizeof(time)-1, "%d %m %Y %H:%M", t);
    printf("Date of Transaction: %s\n", time);
    printf("TransactionType: %d\n", (int)transaction->transactionType);
    printf("Opening Balance: %lf\n", transaction->opening_balance);
    printf("Closing Balance: %lf\n", transaction->closing_balance);
}


#endif /* end of include guard:  */
