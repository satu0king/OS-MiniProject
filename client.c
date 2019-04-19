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
    char password[20] = "admin@123";
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
    printf("1) Balance Enquiry\n");
    printf("2) Add New User\n");
    printf("3) Add New Joint User\n");
    printf("4) Deposit amount\n");
    printf("5) Withdraw amount\n");
    printf("6) Change Password\n");
    printf("7) Exit\n");
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

void addUser(int session_id, int sd, bool joint){

    printf("Add User in process...\n" );

    struct User user = {
        .id = -1,
        .account_id = -1
    };

    printf("Enter Username: ");
    scanf("%s", user.name);
    printf("Enter Email Id: ");
    scanf("%s", user.email);
    printf("Enter Password: ");
    scanf("%s", user.password);
    printf("Enter AccountType (Normal: %d, Admin: %d): ", Normal, Admin);
    scanf("%d", (int *)&user.accountType);

    if(joint){
        printf("Enter Joint account id ");
        scanf("%d", &user.account_id);
    }

    //
    struct Header header = {
        .action = AddAccount,
        .session_id = session_id,
    };

    struct AddUserRequest request = {.user = user};

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

void deposit(int session_id, int sd) {

    printf("Deposit in process...\n" );

    struct Header header = {
        .action = Deposit,
        .session_id = session_id,
    };

    struct DepositRequest request;

    printf("Enter amount: ");
    scanf("%lf", &request.amount);
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

void withdraw(int session_id, int sd){

    printf("Withdraw in process...\n" );

    struct Header header = {
        .action = Withdraw,
        .session_id = session_id,
    };

    struct WithdrawRequest request;

    printf("Enter amount: ");
    scanf("%lf", &request.amount);
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
            case 1:
                balanceEnquiry(session_id, sd);break;
            case 2:
                addUser(session_id, sd, false);break;
            case 3:
                addUser(session_id, sd, true);break;
            case 4:
                deposit(session_id, sd);break;
            case 5:
                withdraw(session_id, sd);break;
            case 6:
                changePassword(session_id, sd);break;
            case 7:
                exitSession(session_id, sd);break;
        }
        close(sd);
    }


}
