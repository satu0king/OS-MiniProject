#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include "util.h"
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ACCOUNT_DB_PATH "account_db.dat"
#define TRANSACTION_DB_PATH "transaction_db.dat"
#define SESSION_COUNT 100
#define NEW_LINE printf("\n")

int sd;

struct Session{
    bool isActive;
    char email[100];
    enum AccountType loginType;
};

struct Session sessions[SESSION_COUNT];


void handle_my(int sig){

  switch (sig) {
    case SIGINT:
        printf("Closing Server\n");
        close(sd);
        exit(0);
        break;
  }

}

int createSocket(int portno){
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1){
        perror("socket() ");
        return sd;
    }
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons (portno);
    int status = bind (sd, (struct sockaddr*)&serv, sizeof (serv));
    if (status == -1){
        perror("bind() ");
        return status;
    }
    status = listen (sd, 5);
    if (status == -1){
        perror("listen() ");
        return status;
    }
    return sd;
}

int getAccount(char email[], struct Account *account){

    int account_fd = open(ACCOUNT_DB_PATH, O_RDONLY);
    if(account_fd == -1){
        perror("open() ");
    }

    int len;
    while(len = read(account_fd, account, sizeof(*account))){
        if (strcmp(account->email, email) == 0){
            close(account_fd);
            return 0;
        }

    }

    close(account_fd);
    return -1;
}

bool validatePassword(struct LoginRequest request){

    struct Account account;
    if (getAccount(request.email, &account) == -1) return false;

    if (strcmp(account.password, request.password)  == 0){
        if(account.accountType == Admin)
            return true;
        if(account.accountType == request.loginType)
            return true;
    }
    return false;
}

void login(int nsd){

    char requestType[] = "Login";

    struct LoginRequest request;
    NEW_LINE;
    printf("<-- %s Request Start-->\n", requestType);
    read(nsd, &request, sizeof(request));
    printf("Email: %s\n", request.email);
    printf("Password: %s\n", request.password);
    printf("LoginType: %d\n", request.loginType);
    printf("<-- %s Request End-->\n", requestType);
    NEW_LINE;

    bool isValidPassword = validatePassword(request);

    struct LoginResponse response = {
        .session_id = -1
    };

    if(!isValidPassword){
        response.status = Unauthorized;
    }
    else {
        int session_id;

        for(session_id = 0; session_id < SESSION_COUNT; session_id++){
            if(!sessions[session_id].isActive)break;
        }

        if (session_id == SESSION_COUNT){
            response.status = Failure;
        }
        else {
            sessions[session_id].isActive = true;
            strcpy(sessions[session_id].email, request.email);
            sessions[session_id].loginType = request.loginType;
            response.status = Success;
            response.session_id = session_id;
        }
    }

    write(nsd, &response, sizeof(response));

    NEW_LINE;
    printf("<-- %s Response Start-->\n", requestType);
    printf("Status: %d\n", response.status);
    printf("SessionId: %d\n", response.session_id);
    printf("<-- %s Response End-->\n", requestType);
    NEW_LINE;


}

void balanceEnquiry(int nsd, int session_id){

    char requestType[] = "Balance Enquiry";
    NEW_LINE;
    printf("<-- %s Request Start-->\n", requestType);
    printf("Email: %s\n", sessions[session_id].email);
    printf("SessionId: %d\n", session_id);
    printf("<-- %s Request End-->\n", requestType);
    NEW_LINE;

    struct Account account;
    struct BalanceEnquiryResponse response;
    if (getAccount(sessions[session_id].email, &account) == -1){
        response.status = Failure;
    }
    else {
        response.status = Success;
        response.balance = account.balance;
    }

    write(nsd, &response, sizeof(response));

    NEW_LINE;
    printf("<-- %s Response Start-->\n", requestType);
    printf("Status: %d\n", response.status);
    printf("Balance: %f\n", response.balance);
    printf("<-- %s Response End-->\n", requestType);
    NEW_LINE;


}

int createAccount(struct Account account){
    int account_fd = open(ACCOUNT_DB_PATH, O_APPEND | O_WRONLY);
    if(account_fd == -1){
        perror("open() ");
        return account_fd;
    }
    int ret = write(account_fd, &account, sizeof(account));
    printf("%d\n", account_fd);
    perror("write: ");

    close(account_fd);
    return 0;
}
void addAccount(int nsd, int session_id){

    char requestType[] = "Admin: Add Account Enquiry";

    struct AddAccountRequest request;
    NEW_LINE;
    printf("<-- %s Request Start-->\n", requestType);
    read(nsd, &request, sizeof(request));
    printf("Admin Email: %s\n", sessions[session_id].email);
    printf("Email: %s\n", request.account.email);
    printf("Name: %s\n", request.account.name);
    printf("SessionId: %d\n", session_id);
    printf("<-- %s Request End-->\n", requestType);
    NEW_LINE;

    struct Response response;

    if(sessions[session_id].loginType == Admin){
        int status = createAccount(request.account);
        if(status == 0)
            response.status = Success;
        else
            response.status = Failure;
    }
    else {
        response.status = Unauthorized;
    }

    write(nsd, &response, sizeof(response));

    NEW_LINE;
    printf("<-- %s Response Start-->\n", requestType);
    printf("Status: %d\n", response.status);
    printf("<-- %s Response End-->\n", requestType);
    NEW_LINE;


}


int main(){

    for(int i=0; i< SESSION_COUNT;i++)
        sessions[i].isActive = false;

    signal(SIGINT, handle_my);
    sd = createSocket(PORTNO);
    if(sd == -1) return 0;

    while(1){
        int nsd = accept (sd, NULL, NULL);
        struct Header header;
        header.action = Login;
        header.session_id = -1;

        read(nsd, &header, sizeof(header));

        printf("Action: %d, SessionId: %d\n", header.action, header.session_id);

        switch(header.action) {
            case Login:
                login(nsd);
                break;
            case BalanceEnquiry:
                balanceEnquiry(nsd, header.session_id);
                break;
            case AddAccount:
                addAccount(nsd, header.session_id);
                break;
        }

        close(nsd);
    }

    close(sd);

}
