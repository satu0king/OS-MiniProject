#ifndef UTIL_H
#define UTIL_H

#define PORTNO 9000

#define BASE_PATH "DB"
#define USER_DB_PATH "DB/user_db.dat"
#define ACCOUNT_DB_PATH "DB/account_db.dat"
#define TRANSACTION_DB_PATH "DB/transaction_db.dat"
#define METADATA_DB_PATH "DB/metadata.dat"
#define MAXTRANSACTIONS 100
#define MAXRECORDS 100
#define DELAY 0

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
    AdminAllUsers,
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

struct TransactionResponse{
    enum Status status;
    struct Transaction transaction;
};

struct UserRequest {
    struct User user;
};

struct UserResponse {
    enum Status status;
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

struct DeleteUserRequest{
    int user_id;
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
    int transaction_details_count;
};

struct ViewAllUsersResponse{
    enum Status status;
    int user_count;
};

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





#endif /* end of include guard:  */
