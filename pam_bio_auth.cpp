#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION
#define PAM_SM_PASSWORD

#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

/* This is the function the system calls for authentication */
/* This is the function the system calls for authentication */
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    
    // Use pam_syslog instead of global syslog to avoid interfering with sudo
    pam_syslog(pamh, LOG_INFO, "Starting biometric authentication via external tool...");

    // Call the external verification tool
    // Check standard installation path first
    int result = system("/usr/local/bin/faceunlock_verify");

    if (result != 0) {
        // Fallback to relative path (testing)
        result = system(".face_auth/verify_face"); 
    }

    if (result != 0) {
         // Fallback to absolute build path (dev)
         result = system("/home/aaditya/Documents/faceunlock/verify_face");
    }

    if (result == 0) {
        pam_syslog(pamh, LOG_INFO, "Face verification SUCCESS");
        return PAM_SUCCESS;
    } else {
        pam_syslog(pamh, LOG_WARNING, "Face verification FAILED (Exit code: %d)", result);
        return PAM_AUTH_ERR;
    }
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}