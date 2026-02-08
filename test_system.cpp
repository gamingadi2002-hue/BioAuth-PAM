#include <stdlib.h>
#include <stdio.h>

int main() {
    int ret_success = system("exit 0");
    printf("Exit 0 returns: %d\n", ret_success);
    
    int ret_fail = system("exit 1");
    printf("Exit 1 returns: %d\n", ret_fail);

    if (ret_success == 0 && ret_fail != 0) {
        printf("Logic is SAFE.\n");
        return 0;
    } else {
        printf("Logic is UNSAFE.\n");
        return 1;
    }
}
