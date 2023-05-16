
#include "hw_2.h"
int main() {
    ImageProcess test;
    test.loadImgFromFile("test.txt", 1);
    test.dilatation(1);
    test.erosion(0);
    test.saveImgToFile("res.txt",1);
    return 0;
}
