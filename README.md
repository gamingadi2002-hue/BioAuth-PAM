# BioAuth-PAM
A secure, open-source Linux PAM module for face authentication. Works with  sudo , su, and login screens using dlib's ResNet and IR/RGB webcams.
=======
# 🛡️ BioAuth-PAM: Linux Face Authentication System

## 🌟 Project Overview

**BioAuth-PAM** is a custom biometric security integration for **Fedora 43**. It replaces or augments standard password authentication (`sudo`, `login`, `gdm`) with a high-security **Face Recognition** challenge.

Unlike standard webcam wrappers, this project uses **Dlib's ResNet-34 Deep Learning model** for state-of-the-art accuracy (**99.38%** on LFW benchmark) and is compiled with AVX instructions for real-time performance on local hardware.

---

## 🖥️ System Architecture

| Component | Specification |
| :--- | :--- |
| **OS** | Fedora 43 (Linux) |
| **Hardware** | Custom IR Camera Setup (Index 2) / RGB Fallback (Index 0) |
| **Language** | C++17 (compiled with GCC) |
| **Core Libraries** | dlib (19.24+), OpenCV (4.x), OpenBLAS |
| **AI Model** | ResNet-34 (Metric Loss) + 68-Point Landmark Detector |
| **Security Mechanism** | Pluggable Authentication Module (PAM) |

---

## 📂 Project Structure

```plaintext
/face-auth-project
│
├── 📜 extract_feature.cpp       # ENROLLMENT TOOL
│   ├── Captures video from IR Camera (device 2)
│   ├── Detects face & aligns landmarks
│   ├── Generates 128D biometric hash
│   └── Saves to 'trusted_face.bin'
│
├── 📜 pam_bio_auth.cpp          # AUTHENTICATION MODULE (Upcoming)
│   ├── Intercepts 'sudo' requests
│   ├── Captures live face & compares to .bin
│   └── Grants/Denies access based on Euclidean distance
│
├── 📦 dlib_face_recognition_resnet_model_v1.dat  # (21 MB)
│   └── The "Brain": ResNet-34 Neural Network Weights
│
├── 📦 shape_predictor_68_face_landmarks.dat      # (95 MB)
│   └── The "Eyes": Geometric Face aligner
│
└── 🔑 trusted_face.bin          # (Generated)
    └── Your encrypted biometric signature
```

---
# BioAuth-PAM: Face Unlock for Linux

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Platform: Linux](https://img.shields.io/badge/Platform-Linux-blue)
![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-green)

BioAuth-PAM is a Pluggable Authentication Module (PAM) that brings secure, fast face recognition to your Linux environment. It integrates seamlessly with `sudo`, login screens (GDM/SDDM), and `su`, providing a modern biometric experience.

## ✨ Features

-   **Fast & Accurate**: Powered by `dlib`'s ResNet face recognition model.
-   **IR Camera Support**: Works with standard webcams and Windows Hello™ style IR cameras (grayscale).
-   **System-Wide Integration**: Uses `authselect` to safely enable face unlock for all authentication prompts.
-   **Secure**: Runs locally with no cloud dependencies.

## 🚀 Installation

### Prerequisites
-   Linux (Fedora, RHEL, CentOS, or derivatives recommended due to `authselect`).
-   Webcam (IR or RGB).
-   `make`, `g++`, `pam-devel`.
-   `dlib` (installed via package manager or compiled).

### Quick Start

1.  **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/faceunlock.git
    cd faceunlock
    ```

2.  **Build**:
    ```bash
    make
    ```

3.  **Enroll your face**:
    ```bash
    ./extract_feature
    ```
    *Look at the camera until `trusted_face.bin` is created.*

4.  **Install**:
    ```bash
    sudo ./install.sh
    ```
    *This script copies binaries to `/usr/local/bin`, models to `/usr/local/share/faceunlock`, and configures PAM.*

5.  **Test**:
    Open a new terminal and run:
    ```bash
    sudo ls
    ```

## 🛠️ Uninstallation

To revert all changes and remove the module:
```bash
sudo ./uninstall.sh
```

## ⚠️ Troubleshooting

-   **Crash on Success**: If `sudo` crashes, ensure you are using the latest version of `pam_bio_auth.so` which uses `pam_syslog`. The `install.sh` script handles this automatically.
-   **Camera Not Found**: The system falls back from Index 2 (IR) to Index 0 (RGB). Check your camera index with `v4l2-ctl --list-devices`.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
> [!NOTE]
> These constraints ensure compatibility with specific system libraries and C++ standards.

1.  **Architecture Mismatch**: The code explicitly uses a **Decomposed Network Definition** (Level 4 -> Level 1) to prevent C++ compiler syntax errors with nested templates. | Ensure you are using the `pyramid_down<6>` definition in `extract_feature.cpp`. |
| **expected ‘;’ before ‘>’ token** | C++ compiler interpreting nested templates `>>>>` as bitwise operators. | Use the "Decomposed" code structure or add spaces: `> > > >`. |
| **Undefined reference to dlib::...** | Linker cannot find the library symbols. | You must link the `.a` file directly, not using `-ldlib`. |
>>>>>>> 514f064 (Initial commit of BioAuth-PAM: Face Unlock for Linux)
