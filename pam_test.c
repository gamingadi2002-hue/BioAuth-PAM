#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> // For dynamic loading test

// Function pointer type for pam_sm_authenticate
typedef int (*pam_sm_authenticate_t)(pam_handle_t *, int, int, const char **);

int main() {
    printf("Loading PAM module: ./pam_bio_auth.so\n");

    void *handle = dlopen("./pam_bio_auth.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Error loading module: %s\n", dlerror());
        return 1;
    }

    pam_sm_authenticate_t pam_auth = (pam_sm_authenticate_t)dlsym(handle, "pam_sm_authenticate");
    if (!pam_auth) {
        fprintf(stderr, "Symbol pam_sm_authenticate not found: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    printf("Symbol found. Calling pam_sm_authenticate...\n");
    // We pass NULL for pamh as our module doesn't use it yet (standard modules use it for conversation function)
    int result = pam_auth(NULL, 0, 0, NULL);
    
    printf("Result code: %d\n", result);

    if (result == PAM_SUCCESS) {
        printf("PAM_SUCCESS: Face matched!\n");
    } else if (result == PAM_AUTH_ERR) {
        printf("PAM_AUTH_ERR: Face not matched (Expected if not enrolled).\n");
    } else {
        printf("Other code: %d\n", result);
    }

    dlclose(handle);
    return 0;
}
