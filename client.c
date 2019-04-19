#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include "util.h"


#define NEW_LINE printf("\n")

int connectSocket(int portno){
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1){
        perror("socket() ");
        return sd;
    }
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons (portno);
    int ret  = connect (sd, (struct sockaddr*)&serv, sizeof (serv));
    if(ret == -1){
        perror("connect() ");
        return ret;
    }
    return sd;

}

int login(int *session_id, enum AccountType *loginType) {

    int sd = connectSocket(PORTNO);
    if(sd == -1) exit(0);

    // char email[100];
    // char password[20];
    // printf("Enter Email Id: ");
    // scanf("%s", email);
    // printf("Enter Password: ");
    // scanf("%s", password);

    char email[100] = "admin@gmail.com";
    char password[20] = "1234qwe";
    // char email[100] = "satviksr@gmail.com";
    // char password[20] = "1234qwe";

    printf("Logging in ...\n" );
    struct Header header = {
        .action = Login,
        .session_id = -1,
    };

    write(sd, &header, sizeof(header));

    struct LoginRequest request;

    strcpy(request.email, email);
    strcpy(request.password, password);
    write(sd, &request, sizeof(request));

    struct LoginResponse response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else {
        *session_id = response.session_id;
        *loginType = response.loginType;
        printf("Logged in with session id %d\n", response.session_id);
    }
    return 0;
}

int getNextOperation(){
    NEW_LINE;
    printf("[1]  (Admin) Add New User\n");
    printf("[2]  (Admin) Add New Joint User\n");
    printf("[3]  (Admin) View All Users\n");
    printf("[4]  (Admin) Delete User\n");
    printf("[5]  (Admin) Modify User\n");
    printf("[6]  (User) Balance Enquiry\n");
    printf("[7]  (User) Deposit amount\n");
    printf("[8]  (User) Withdraw amount\n");
    printf("[9]  (User) Change Password\n");
    printf("[10] (User) View Details\n");
    printf("[11] (User) Exit\n");
    printf("Enter operation Id: ");
    int operation;
    scanf("%d", &operation);
    NEW_LINE;
    return operation;

}

void balanceEnquiry(int session_id, int sd){

    printf("Balance Enquiry in process...\n" );

    struct Header header = {
        .action = BalanceEnquiry,
        .session_id = session_id,
    };

    write(sd, &header, sizeof(header));

    struct BalanceEnquiryResponse response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else if (response.status == Failure){
        printf("Server Failure\n");
    }
    else {
        printf("Balance:  %f\n", response.balance);
    }

    close(sd);
}

void exitSession(int session_id, int sd){

    printf("Exit in process...\n" );

    struct Header header = {
        .action = Exit,
        .session_id = session_id,
    };

    write(sd, &header, sizeof(header));

    struct Response response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else if (response.status == Failure){
        printf("Server Failure\n");
    }
    else {
        printf("Success\n");
        exit(0);
    }
}

void getUserInformationFromUI(struct User *user){
    printf("Enter Username: ");
    scanf("%s", user->name);
    printf("Enter Email Id: ");
    scanf("%s", user->email);
    printf("Enter Password: ");
    scanf("%s", user->password);
    printf("Enter AccountType (Normal: %d, Admin: %d): ", Normal, Admin);
    scanf("%d", (int *)&user->accountType);
}

void addUser(int session_id, int sd, bool joint){

    printf("Add User in process...\n" );

    struct User user = {
        .id = -1,
        .account_id = -1
    };

    getUserInformationFromUI(&user);

    if(joint){
        printf("Enter Joint account id ");
        scanf("%d", &user.account_id);
    }

    struct Header header = {
        .action = AddAccount,
        .session_id = session_id,
    };

    struct UserRequest request = {.user = user};

    write(sd, &header, sizeof(header));
    write(sd, &request, sizeof(request));

    struct UserResponse response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else if (response.status == Failure){
        printf("Server Failure\n");
    }
    else {
        printf("Success\n");
        printUser(&response.user);
    }
}

void modifyUser(int session_id, int sd){

    printf("Modify User in process...\n" );

    struct User user = {
        .id = -1,
        .account_id = -1
    };

    printf("Enter User id ");
    scanf("%d", &user.id);

    getUserInformationFromUI(&user);

    struct Header header = {
        .action = ModifyAccount,
        .session_id = session_id,
    };

    struct UserRequest request = {.user = user};

    write(sd, &header, sizeof(header));
    write(sd, &request, sizeof(request));

    struct UserResponse response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else if (response.status == Failure){
        printf("Server Failure\n");
    }
    else {
        printf("Success\n");
        printUser(&response.user);
    }
}

void deleteUser(int session_id, int sd){

    printf("Delete User in process...\n" );

    struct Header header = {
        .action = DeleteAccount,
        .session_id = session_id,
    };

    struct DeleteUserRequest request;
    printf("Enter Account Id: ");
    scanf("%d", &request.user_id);

    write(sd, &header, sizeof(header));
    write(sd, &request, sizeof(request));

    struct Response response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else if (response.status == Failure){
        printf("Server Failure\n");
    }
    else {
        printf("Success\n");
    }
}

void deposit(int session_id, int sd){

    printf("Deposit in process...\n" );

    struct Header header = {
        .action = Transaction,
        .session_id = session_id,
    };

    struct TransactionRequest request;
    request.transactionType = Deposit;

    printf("Enter amount: ");
    scanf("%lf", &request.amount);
    write(sd, &header, sizeof(header));
    write(sd, &request, sizeof(request));

    struct TransactionResponse response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else if (response.status == Failure){
        printf("Server Failure\n");
    }
    else {
        printf("Success\n");
        printTransaction(&response.transaction);
    }
}

void withdraw(int session_id, int sd){

    printf("Withdraw in process...\n" );

    struct Header header = {
        .action = Transaction,
        .session_id = session_id,
    };

    struct TransactionRequest request;
    request.transactionType = Withdraw;

    printf("Enter amount: ");
    scanf("%lf", &request.amount);
    write(sd, &header, sizeof(header));
    write(sd, &request, sizeof(request));

    struct TransactionResponse response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else if (response.status == Failure){
        printf("Server Failure\n");
    }
    else {
        printf("Success\n");
        printTransaction(&response.transaction);
    }
}

void changePassword(int session_id, int sd){

    printf("Change Password in process...\n" );

    struct Header header = {
        .action = PasswordChange,
        .session_id = session_id,
    };

    struct ChangePasswordRequest request;

    printf("Enter new password: ");
    scanf("%s", request.new_password);
    write(sd, &header, sizeof(header));
    write(sd, &request, sizeof(request));

    struct Response response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else if (response.status == Failure){
        printf("Server Failure\n");
    }
    else {
        printf("Success\n");
    }
}

void viewDetails(int session_id, int sd){

    printf("View details in process...\n");

    struct Header header = {
        .action = Viewdetails,
        .session_id = session_id,
    };

    write(sd, &header, sizeof(header));

    struct ViewDetailsResponse response;
    read(sd, &response, sizeof(response));

    struct User user;
    read(sd, &user, sizeof(user));

    struct Account account;
    read(sd, &account, sizeof(account));

    struct Transaction transactions[response.transaction_details_count];
    for(int i =0; i<response.transaction_details_count; i++)
        read(sd, transactions + response.transaction_details_count - i -1, sizeof(struct Transaction));

    NEW_LINE;
    printUser(&user);

    NEW_LINE;
    printAccount(&account);

    NEW_LINE;
    // for(int i=0; i<response.transaction_details_count; i++){
    //     printTransaction(transactions + i);
    //     NEW_LINE;
    // }
    //

    printTransactions(transactions, response.transaction_details_count);


    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else if (response.status == Failure){
        printf("Server Failure\n");
    }
    else {
        printf("Success\n");
    }
}

void viewAllUsers(int session_id, int sd){

    printf("View details in process...\n");

    struct Header header = {
        .action = AdminAllUsers,
        .session_id = session_id,
    };

    write(sd, &header, sizeof(header));

    struct ViewAllUsersResponse response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else if (response.status == Failure){
        printf("Server Failure\n");
    }
    else {
        printf("Success\n");
        struct User users[response.user_count];
        for(int i = 0; i<response.user_count; i++)
            read(sd, users + i, sizeof(struct User));

        NEW_LINE;
        printUsers(users, response.user_count);
    }
}

int main(){

    printf("Client started with pid %d\n", getpid());
    int session_id = -1;
    enum AccountType loginType;
    login(&session_id, &loginType);

    if (session_id == -1)return 0;

    while(1){
        int operation = getNextOperation();
        int sd = connectSocket(PORTNO);
        if(sd == -1) exit(0);
        switch(operation){
            case 6:
                balanceEnquiry(session_id, sd);break;
            case 1:
                addUser(session_id, sd, false);break;
            case 2:
                addUser(session_id, sd, true);break;
            case 7:
                deposit(session_id, sd);break;
            case 8:
                withdraw(session_id, sd);break;
            case 9:
                changePassword(session_id, sd);break;
            case 11:
                exitSession(session_id, sd);break;
            case 10:
                viewDetails(session_id, sd);break;
            case 3:
                viewAllUsers(session_id, sd);break;
            case 4:
                deleteUser(session_id, sd);break;
            case 5:
                modifyUser(session_id, sd);break;
        }
        close(sd);
    }


}
