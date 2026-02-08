CXX = g++
CXXFLAGS = -std=c++17 -O3 $(shell pkg-config --cflags opencv4) -I/usr/local/include
LDFLAGS = $(shell pkg-config --libs opencv4) /usr/local/lib64/libdlib.a -lpthread -lX11 -lopenblas

# PAM specific flags (No dlib needed)
PAM_CXXFLAGS = -std=c++17 -O3 -fPIC
PAM_LDFLAGS = -shared -lpam

TARGET = extract_feature
VERIFY_TARGET = verify_face
PAM_TARGET = pam_bio_auth.so

all: $(TARGET) $(VERIFY_TARGET) $(PAM_TARGET)

$(TARGET): extract_feature.cpp face_model.h
	$(CXX) $(CXXFLAGS) extract_feature.cpp -o $@ $(LDFLAGS)

$(VERIFY_TARGET): verify_face.cpp face_model.h
	$(CXX) $(CXXFLAGS) verify_face.cpp -o $@ $(LDFLAGS)

$(PAM_TARGET): pam_bio_auth.cpp
	$(CXX) $(PAM_CXXFLAGS) pam_bio_auth.cpp -o $@ $(PAM_LDFLAGS)

clean:
	rm -f $(TARGET) $(VERIFY_TARGET) $(PAM_TARGET)
