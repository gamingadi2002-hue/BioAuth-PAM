#include <iostream>
#include <fstream>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>

#include "face_model.h"

using namespace std;
using namespace dlib;

// Configuration (Must match what was intended for PAM)
const string MODEL_DIR = "/home/aaditya/Documents/faceunlock/"; // In production, use absolute paths or config
const string LANDMARK_MODEL = MODEL_DIR + "shape_predictor_68_face_landmarks.dat";
const string RESNET_MODEL = MODEL_DIR + "dlib_face_recognition_resnet_model_v1.dat";
const string TRUSTED_FACE = MODEL_DIR + "trusted_face.bin";
const double MATCH_THRESHOLD = 0.6;

int main() {
    try {
        // 1. Load Models
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor sp;
        anet_type net;

        // Standard installation paths
    const std::string model_path = "/usr/local/share/faceunlock/dlib_face_recognition_resnet_model_v1.dat";
    const std::string shape_path = "/usr/local/share/faceunlock/shape_predictor_68_face_landmarks.dat";
    const std::string trusted_face_path = "/etc/faceunlock/trusted_face.bin";

    try {
        deserialize(model_path) >> net;
        deserialize(shape_path) >> sp;
    } catch(...) {
        // Fallback to local directory for development/testing
        try {
            deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;
            deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
        } catch(std::exception& e) {
             cerr << "[CRITICAL ERROR]: Could not load models from " << model_path << " or local directory." << endl;
             cerr << e.what() << endl;
             return 1;
        }
    }

        // 2. Load Trusted Face
        matrix<float,0,1> trusted_face;
        try {
            deserialize(trusted_face_path) >> trusted_face;
        } catch(...) {
             try {
                deserialize("trusted_face.bin") >> trusted_face;
             } catch(...) {
                cerr << "Error: trusted_face.bin not found in " << trusted_face_path << " or local dir." << endl;
                return 1;
             }
        }

        // 3. Capture Frame
        // Try Index 2 (IR) -> Index 0 (RGB)
        cv::VideoCapture cap(2);
        if (!cap.isOpened()) {
            cap.open(0);
        }
        if (!cap.isOpened()) {
            cerr << "Error: No camera found." << endl;
            return 1;
        }

        cv::Mat frame;
        matrix<float,0,1> current_face;
        bool face_found = false;

        // Try for a limited number of frames
        for (int i = 0; i < 20; ++i) {
            cap >> frame;
            if (frame.empty()) continue;

            if (frame.channels() == 1) {
                cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
            }

            cv_image<bgr_pixel> cimg(frame);
            std::vector<dlib::rectangle> faces = detector(cimg);

            if (faces.size() > 0) {
                auto shape = sp(cimg, faces[0]);
                matrix<rgb_pixel> face_chip;
                extract_image_chip(cimg, get_face_chip_details(shape, 150, 0.25), face_chip);
                current_face = net(face_chip);
                face_found = true;
                break;
            }
        }

        if (!face_found) {
            cerr << "No face detected." << endl;
            return 1;
        }

        // 4. Compare
        double distance = length(trusted_face - current_face);
        cout << "Distance: " << distance << endl;

        if (distance < MATCH_THRESHOLD) {
            cout << "MATCH" << endl;
            return 0; // Success
        } else {
            cout << "NO MATCH" << endl;
            return 1; // Failure
        }

    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }
}
