#include <dlib/dnn.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>

using namespace dlib;
using namespace std;

#include "face_model.h"

// ----------------------------------------------------------------------------------------

int main() {
    try {
        // Try Index 2 (IR Camera) for Latitude 7400
        cv::VideoCapture cap(2); 
        if (!cap.isOpened()) {
            cout << "IR Camera (2) not found. Trying RGB (0)..." << endl;
            cap.open(0);
        }

        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor sp;
        anet_type net;

        cout << "Loading AI models..." << endl;
        
        // This MUST work if the static library is linked correctly
        deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
        deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;

        cout << "SUCCESS: Models loaded!" << endl;
        cout << "Look at the camera..." << endl;

        cv::Mat frame;
        while (true) {
            cap >> frame;
            if (frame.empty()) continue;

            if (frame.channels() == 1) {
                cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
            }

            cv_image<bgr_pixel> cimg(frame);
            auto faces = detector(cimg);

            if (faces.size() > 0) {
                cout << "Face detected..." << endl;
                auto shape = sp(cimg, faces[0]);
                matrix<rgb_pixel> face_chip;
                extract_image_chip(cimg, get_face_chip_details(shape, 150, 0.25), face_chip);
                
                matrix<float,0,1> face_descriptor = net(face_chip);
                
                ofstream out("trusted_face.bin", ios::binary);
                serialize(face_descriptor, out);
                out.close();

                cout << "SUCCESS: Biometric key 'trusted_face.bin' created." << endl;
                break; 
            }
        }
    } catch (std::exception& e) {
        cerr << "\n[CRITICAL ERROR]: " << e.what() << endl;
        return 1;
    }
    return 0;
}