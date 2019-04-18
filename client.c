#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
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

int login(enum AccountType loginType){

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

    struct LoginRequest request = {
        .loginType = loginType
    };

    strcpy(request.email, email);
    strcpy(request.password, password);
    write(sd, &request, sizeof(request));

    struct LoginResponse response;
    read(sd, &response, sizeof(response));

    if(response.status == Unauthorized){
        printf("Unauthorized\n");
    }
    else {
        printf("Logged in with session id %d\n", response.session_id);
    }
    return response.session_id;
}

int getNextOperation(){
    NEW_LINE;
    printf("1) Balance Enquiry\n");
    printf("2) Add Account\n");
    printf("Enter operation Id: ");
    int operation;
    scanf("%d", &operation);
    NEW_LINE;
    return operation;

}

void balanceEnquiry(int session_id){
    int sd = connectSocket(PORTNO);
    if(sd == -1) exit(0);

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
}

void addAccount(int session_id){

    printf("Add Account in process...\n" );

    struct Account account;

    printf("Enter Username: ");
    scanf("%s", account.name);
    printf("Enter Email Id: ");
    scanf("%s", account.email);
    printf("Enter Password: ");
    scanf("%s", account.password);
    printf("Enter Balance: ");
    scanf("%lf", &account.balance);
    printf("Enter AccountType (Normal: %d, Admin: %d): ", Normal, Admin);
    scanf("%d", (int *)&account.accountType);

    int sd = connectSocket(PORTNO);
    if(sd == -1) exit(0);

    struct Header header = {
        .action = AddAccount,
        .session_id = session_id,
    };

    struct AddAccountRequest request = {.account = account};

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

    int session_id = login(Admin);
    if (session_id == -1)return 0;

    while(1){
        int operation = getNextOperation();
        switch(operation){
            case 1:
                balanceEnquiry(session_id);
            case 2:
                addAccount(session_id);
        }
    }


}
