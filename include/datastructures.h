#pragma once

#include <stdbool.h>

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

struct Session{
    bool isActive;
    struct User user;
};
