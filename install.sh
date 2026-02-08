#!/bin/bash

# Face Unlock Installation Script
# Installs binaries, models, and configures PAM.

set -e

if [ "$EUID" -ne 0 ]; then
  echo "Please run as root."
  exit 1
fi

echo "========================================"
echo "   Face Unlock Installer"
echo "========================================"

# 1. Prepare Directories
echo "[*] Creating directories..."
mkdir -p /usr/local/bin
mkdir -p /usr/local/share/faceunlock
mkdir -p /etc/faceunlock
mkdir -p /lib64/security

# 2. Copy Files
echo "[*] Installing binaries and models..."

# Check if compiled
if [ ! -f "verify_face" ] || [ ! -f "pam_bio_auth.so" ]; then
    echo "Error: Binaries not found. Please run 'make' first."
    exit 1
fi

cp verify_face /usr/local/bin/faceunlock_verify
chmod +x /usr/local/bin/faceunlock_verify

cp pam_bio_auth.so /lib64/security/
chmod 644 /lib64/security/pam_bio_auth.so

# Copy models if they exist in current dir
if [ -f "dlib_face_recognition_resnet_model_v1.dat" ]; then
    cp dlib_face_recognition_resnet_model_v1.dat /usr/local/share/faceunlock/
    cp shape_predictor_68_face_landmarks.dat /usr/local/share/faceunlock/
else
    echo "Warning: Model files not found in current directory."
    echo "Please ensure they are copied to /usr/local/share/faceunlock/"
fi

# Copy trusted face if exists (Migration)
if [ -f "trusted_face.bin" ]; then
    echo "[*] Migrating existing enrollment..."
    cp trusted_face.bin /etc/faceunlock/
fi

# 3. Configure PAM
echo "[*] Configuring PAM..."

# Create profile if not exists
if ! authselect list | grep -q "face-unlock"; then
    echo "    Creating authselect profile..."
    authselect create-profile face-unlock -b local
fi

# Inject module into system-auth and password-auth
PROFILE_DIR="/etc/authselect/custom/face-unlock"
if [ -d "$PROFILE_DIR" ]; then
    # Idempotent check
    if ! grep -q "pam_bio_auth.so" "$PROFILE_DIR/system-auth"; then
        sed -i '/pam_faildelay.so/a auth        sufficient                                   pam_bio_auth.so' "$PROFILE_DIR/system-auth"
        sed -i '/pam_faildelay.so/a auth        sufficient                                   pam_bio_auth.so' "$PROFILE_DIR/password-auth"
        echo "    Module added to custom profile."
    else
        echo "    Module already in profile."
    fi
    
    echo "    Selecting profile..."
    authselect select custom/face-unlock with-fingerprint with-mdns4 with-silent-lastlog --force
else
    echo "Error: Custom profile directory not found."
    exit 1
fi

echo "========================================"
echo "   Installation Complete!"
echo "========================================"
echo "Run 'sudo ls' to test."
