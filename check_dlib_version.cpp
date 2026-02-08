#include <dlib/revision.h>
#include <iostream>

int main() {
    std::cout << "Dlib Major: " << DLIB_MAJOR_VERSION << std::endl;
    std::cout << "Dlib Minor: " << DLIB_MINOR_VERSION << std::endl;
    std::cout << "Dlib Patch: " << DLIB_PATCH_VERSION << std::endl;
    return 0;
}
