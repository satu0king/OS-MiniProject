#include <sys/types.h>          /* See NOTES */
#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "database.h"
 #include <sys/stat.h>

int main(){

    mkdir(BASE_PATH, 0744);

    int account_fd = creat(ACCOUNT_DB_PATH, 0744);
    int user_fd = creat(USER_DB_PATH, 0744);
    int transaction_fd = creat(TRANSACTION_DB_PATH, 0744);
    int metadata_fd = creat(METADATA_DB_PATH, 0744);

    if(metadata_fd == -1 || user_fd == -1 || account_fd == -1 || transaction_fd == -1){
        close(account_fd);
        close(transaction_fd);
        close(metadata_fd);
        close(metadata_fd);
        perror("creat(): ");
        return 0;
    }

    struct Metadata metadata = {
        .user_id = 0,
        .account_id = 0,
        .transaction_id = 0,
    };

    write(metadata_fd, &metadata, sizeof(metadata));

    struct User u1 = {
        .accountType = Admin,
        .name = "Administrator",
        .email = "admin@gmail.com",
        .password = "admin@123",
        .account_id = -1,
        .id = -1
    };

    struct User u2 = {
        .accountType = Normal,
        .name = "Satvik Ramaprasad",
        .email = "satviksr@gmail.com",
        .password = "1234qwe",
        .account_id = -1,
        .id = -1
    };

    struct User u3 = {
        .accountType = Normal,
        .name = "Biswesh Mohopatra",
        .email = "biswesh@gmail.com",
        .password = "1234qwe",
        .account_id = -1,
        .id = -1
    };

    int user_id;
    createUser(&u1);
    createUser(&u2);
    u3.account_id = u2.account_id;
    createUser(&u3);

    printUser(&u1);
    printUser(&u2);
    printUser(&u3);

    close(account_fd);
    close(transaction_fd);
    close(metadata_fd);
    close(metadata_fd);

}
