#ifndef UTIL_H
#define UTIL_H

#define PORTNO 9001

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
};

enum AccountType{
    Admin,
    Normal,
    Joint,
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

struct Account {
    enum AccountType accountType;
    double balance;
    char name[100];
    char email[100];
    char password[100];
};

struct AddAccountRequest {
    struct Account account;
};

struct LoginRequest{
    char email[100];
    char password[20];
    enum AccountType loginType;
};

struct LoginResponse{
    enum Status status;
    int session_id;
};

struct BalanceEnquiryResponse{
    enum Status status;
    double balance;
};



#endif /* end of include guard:  */
