
#include "util.h"
#include "database.h"

int sd;

struct Session;;

struct Session sessions[SESSION_COUNT];
pthread_mutex_t sessions_lock;

void killServer();


void handle_my(int sig);

int createSocket(int portno);

bool validatePassword(struct LoginRequest request);

void loginController(int nsd);
void exitSessionController(int nsd, int session_id);

void balanceEnquiryController(int nsd, int session_id);

void addAccountController(int nsd, int session_id);

void modifyAccountController(int nsd, int session_id);

void transactionController(int nsd, int session_id);

void changePasswordController(int nsd, int session_id);

void viewDetailsController(int nsd, int session_id);

void getAllUsersController(int nsd, int session_id);

void deleteAccountController(int nsd, int session_id);

void* handle_request(void *socketDescriptor);

void setup();


int main();
