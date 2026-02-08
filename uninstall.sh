#!/bin/bash

# Face Unlock Uninstaller
# Removes files and reverts PAM.

if [ "$EUID" -ne 0 ]; then
  echo "Please run as root."
  exit 1
fi

echo "========================================"
echo "   Face Unlock Uninstaller"
echo "========================================"

# 1. Revert PAM
echo "[*] Reverting PAM configuration..."
authselect select local --force

# 2. Delete Files
echo "[*] Removing files..."
rm -f /usr/local/bin/faceunlock_verify
rm -rf /usr/local/share/faceunlock
rm -rf /etc/faceunlock
rm -f /lib64/security/pam_bio_auth.so
rm -rf /etc/authselect/custom/face-unlock

echo "========================================"
echo "   Uninstallation Complete!"
echo "========================================"
