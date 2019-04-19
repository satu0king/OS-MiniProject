#ifndef UTIL_H
#define UTIL_H

#define PORTNO 9000

#define USER_DB_PATH "user_db.dat"
#define ACCOUNT_DB_PATH "account_db.dat"
#define TRANSACTION_DB_PATH "transaction_db.dat"
#define METADATA_DB_PATH "metadata.dat"

enum Action{
    Login,
    Deposit,
    Withdraw,
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

struct Transaction {
    int account_id;
    char name[100];
    char date[100];
    double opening_balance;
    double closing_balance;
    char message[100];
    int id;
};

struct AddUserRequest {
    struct User user;
};

struct DepositRequest {
    double amount;
};

struct WithdrawRequest {
    double amount;
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



#endif /* end of include guard:  */
