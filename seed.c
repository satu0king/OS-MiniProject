#include <sys/types.h>          /* See NOTES */
#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>
#include "util.h"

#define ACCOUNT_DB_PATH "account_db.dat"
#define TRANSACTION_DB_PATH "transaction_db.dat"

int main(){
    int account_fd = creat(ACCOUNT_DB_PATH, 0744);
    int transaction_fd = creat(TRANSACTION_DB_PATH, 0744);

    if(account_fd == -1 || transaction_fd == -1){
        close(account_fd);
        close(transaction_fd);
        perror("creat(): ");
        return 0;
    }

    struct Account a1 = {
        .accountType = Admin,
        .balance = 0,
        .name = "Administrator",
        .email = "admin@gmail.com",
        .password = "admin@123",
    };

    struct Account a2 = {
        .accountType = Normal,
        .balance = 1000,
        .name = "Satvik Ramaprasad",
        .email = "satviksr@gmail.com",
        .password = "1234qwe",
    };

    write(account_fd, &a1, sizeof(a1));
    write(account_fd, &a2, sizeof(a2));

    close(account_fd);
    close(transaction_fd);
}
