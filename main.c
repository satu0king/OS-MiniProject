#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "util.h"
#include "database.h"

#define SESSION_COUNT 100
#define NEW_LINE printf("\n")

int sd;

struct Session{
    bool isActive;
    struct User user;
};

struct Session sessions[SESSION_COUNT];



void killServer(){
    NEW_LINE;
    printf("Closing Server\n");
    close(sd);
    NEW_LINE;
    exit(0);
}


void handle_my(int sig){

  switch (sig) {
    case SIGINT:
        killServer();
        break;
  }

}

int createSocket(int portno){
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1){
        perror("socket() ");
        return sd;
    }
    struct linger lin;

    lin.l_onoff = 0;
    lin.l_linger = 0;
    setsockopt(sd, SOL_SOCKET, SO_LINGER, (const char *)&lin, sizeof(int));

    int reuse = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

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

bool validatePassword(struct LoginRequest request){

    struct User user;
    if (getUser(request.email, &user) == -1) return false;

    if (strcmp(user.password, request.password)  == 0)
        return true;

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
        struct User user;
        getUser(request.email, &user);
        int session_id;

        for(session_id = 0; session_id < SESSION_COUNT; session_id++){
            if(!sessions[session_id].isActive)break;
        }

        if (session_id == SESSION_COUNT){
            response.status = Failure;
        }
        else {
            sessions[session_id].isActive = true;
            sessions[session_id].user = user;
            response.session_id = session_id;
            response.loginType = user.accountType;
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
void exitSession(int nsd, int session_id){

    char requestType[] = "User: Exit";

    NEW_LINE;
    printf("<-- %s Request Start-->\n", requestType);
    printf("<-- %s Request End-->\n", requestType);
    NEW_LINE;


    struct Response response = {.status = Success};

    sessions[session_id].isActive = false;

    write(nsd, &response, sizeof(response));

    NEW_LINE;
    printf("<-- %s Response Start-->\n", requestType);
    printf("Status: %d\n", response.status);
    printf("<-- %s Response End-->\n", requestType);
    NEW_LINE;

}

void balanceEnquiry(int nsd, int session_id){

    char requestType[] = "Balance Enquiry";
    NEW_LINE;
    printf("<-- %s Request Start-->\n", requestType);
    printf("Email: %s\n", sessions[session_id].user.email);
    printf("SessionId: %d\n", session_id);
    printf("<-- %s Request End-->\n", requestType);
    NEW_LINE;

    struct Account account;
    struct BalanceEnquiryResponse response;
    if (getAccount(sessions[session_id].user.account_id, &account) == -1){
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

void addAccount(int nsd, int session_id){

    char requestType[] = "Admin: Add Account";

    struct AddUserRequest request;
    NEW_LINE;
    printf("<-- %s Request Start-->\n", requestType);
    read(nsd, &request, sizeof(request));
    printf("Admin Email: %s\n", sessions[session_id].user.email);
    printf("Email: %s\n", request.user.email);
    printf("Name: %s\n", request.user.name);
    printf("SessionId: %d\n", session_id);
    printf("<-- %s Request End-->\n", requestType);
    NEW_LINE;

    struct Response response;

    if(sessions[session_id].user.accountType == Admin){
        int status = createUser(&request.user);
        if(status == 0){
            response.status = Success;
            printf("User created: \n");
            printf("User Id: %d \n", request.user.id);
            printf("Account Id: %d \n", request.user.account_id);
        }
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

void transaction(int nsd, int session_id){

    char requestType[] = "User: Transaction";

    struct TransactionRequest request;
    NEW_LINE;
    printf("<-- %s Request Start-->\n", requestType);
    read(nsd, &request, sizeof(request));
    printf("Email: %s\n", sessions[session_id].user.email);
    printf("SessionId: %d\n", session_id);
    printf("Amount: %lf\n", request.amount);
    printf("Transaction Type: %d\n", (int) request.transactionType);
    printf("<-- %s Request End-->\n", requestType);
    NEW_LINE;

    struct Response response;

    int status;
    int account_id = sessions[session_id].user.account_id;
    double amount = request.amount;
    double opening_balance, closing_balance;
    struct Account account;

    getAccount(account_id, &account);
    opening_balance = account.balance;

    if(request.transactionType == Deposit)
      status = changeAccountBalance(account_id, amount);
    else if(request.transactionType = Withdraw)
      status = changeAccountBalance(account_id, -amount);
    else status = -1;

    getAccount(account_id, &account);
    closing_balance = account.balance;

    if(status == 0){
        response.status = Success;
        struct Transaction transaction = {
            .account_id = account_id,
            .user_id = sessions[session_id].user.id,
            .opening_balance = opening_balance,
            .closing_balance = closing_balance,
            .transactionType = request.transactionType,
            .id = -1,
            .date = time(NULL)
        };
        strcpy(transaction.name, sessions[session_id].user.name);
        createTransaction(&transaction);
    }
    else {
        response.status = Failure;
    }

    write(nsd, &response, sizeof(response));

    NEW_LINE;
    printf("<-- %s Response Start-->\n", requestType);
    printf("Status: %d\n", response.status);
    printf("<-- %s Response End-->\n", requestType);
    NEW_LINE;

}


void changePassword(int nsd, int session_id){

    char requestType[] = "User: Change Password";

    struct ChangePasswordRequest request;
    NEW_LINE;
    printf("<-- %s Request Start-->\n", requestType);
    read(nsd, &request, sizeof(request));
    printf("Email: %s\n", sessions[session_id].user.email);
    printf("SessionId: %d\n", session_id);
    printf("New Password: %s\n", request.new_password);
    printf("<-- %s Request End-->\n", requestType);
    NEW_LINE;

    struct Response response;

    strcpy(sessions[session_id].user.password, request.new_password);

    int status = saveUser(&sessions[session_id].user);

    if(status == 0){
        response.status = Success;
    }
    else {
        response.status = Failure;
    }

    write(nsd, &response, sizeof(response));

    NEW_LINE;
    printf("<-- %s Response Start-->\n", requestType);
    printf("Status: %d\n", response.status);
    printf("<-- %s Response End-->\n", requestType);
    NEW_LINE;

}

void viewDetails(int nsd, int session_id){

    char requestType[] = "User: View Details";

    struct ChangePasswordRequest request;
    NEW_LINE;
    printf("<-- %s Request Start-->\n", requestType);
    printf("Email: %s\n", sessions[session_id].user.email);
    printf("SessionId: %d\n", session_id);
    printf("<-- %s Request End-->\n", requestType);
    NEW_LINE;


    struct User user;
    getUser(sessions[session_id].user.email, &user);
    struct Account account;
    getAccount(user.account_id, &account);

    struct Transaction transactions[MAXTRANSACTIONS];
    int count = getTransactions(user.account_id, transactions, MAXTRANSACTIONS);

    struct ViewDetailsResponse response ={
        .status = Success,
        .transactionDetailsCount = count
    };

    write(nsd, &response, sizeof(response));
    if (response.status == Success){

        write(nsd, &user, sizeof(user));
        write(nsd, &account, sizeof(account));

        for(int i = 0; i < count; i++)
                write(nsd, &transactions[i], sizeof(transactions[i]));
    }

    NEW_LINE;
    printf("<-- %s Response Start-->\n", requestType);
    printf("Status: %d\n", response.status);
    printf("<-- %s Response End-->\n", requestType);
    NEW_LINE;

}



void* handle_request(void *socketDescriptor){

    int nsd = *(int *)socketDescriptor;
    struct Header header;

    read(nsd, &header, sizeof(header));

    printf("Action: %d, SessionId: %d\n", header.action, header.session_id);

    switch(header.action) {
        case Login:
            login(nsd);break;
        case BalanceEnquiry:
            balanceEnquiry(nsd, header.session_id);break;
        case AddAccount:
            addAccount(nsd, header.session_id);break;
        case Transaction:
            transaction(nsd, header.session_id);break;
        case PasswordChange:
            changePassword(nsd, header.session_id);break;
        case Exit:
            exitSession(nsd, header.session_id);break;
        case Viewdetails:
            viewDetails(nsd, header.session_id);break;
        default:
            printf("Error unknown action: %d\n", (int)header.action);
    }

    printf("Closing socked %d\n", nsd);
    close(nsd);
}

void setup(){
    // int fd = creat("pid", 0744);
    // char pid[10];
    // sprintf(pid, "%d", getpid());
    // write(fd, pid, strlen(pid));
    // close(fd);

    printf("Starting Server\n");
    sd = createSocket(PORTNO);
    if (sd !=-1){
        printf("Server started on port number %d and process id %d\n", PORTNO, getpid());
    }
    NEW_LINE;

    for(int i=0; i< SESSION_COUNT;i++)
        sessions[i].isActive = false;

    signal(SIGINT, handle_my);


}


int main(){

    setup();

    if(sd == -1) return 0;

    while(1){
        int nsd = accept (sd, NULL, NULL);
        pthread_t thread_id;
        // handle_request(&nsd);
        pthread_create(&thread_id, NULL, &handle_request, &nsd);
    }

    close(sd);

}
