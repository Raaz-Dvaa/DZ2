#include "hw_2.h"

Img::Img() {
    srcImg = nullptr;
    width = 0;
    height = 0;
    x_c = 0;
    y_c = 0;
}
Img::Img(int w, int h) {
    srcImg = new int[w*h];
    width = w;
    height = h;
    x_c = w/2;
    y_c = h/2;
}
Img::Img(const int *src, int w, int h) {
    srcImg = new int[w*h];
    for (int i = 0; i < w*h ; i++){
        srcImg[i] = src[i];
    }
    width = w;
    height = h;
    x_c = w/2;
    y_c = h/2;
}
Img::~Img(){
    delete [] srcImg;
    srcImg = nullptr;
}
ImageProcess::ImageProcess() {
    srcImg = new Img;
    processedImg = new Img;
     int* a = new int [25] {0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,};
    //int* a = new int[9] {0, 1 , 0, 1,1,1,0,1,0};
     mask = new Img(a, 5, 5);
    //mask = new Img(a,3,3);
    delete[] a;
}
ImageProcess::ImageProcess(int w, int h) {
    srcImg = new Img(w,h);
    processedImg = new Img(w,h);
     int* a = new int [25] {0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,};
    //int* a = new int[9] {0, 1 , 0, 1,1,1,0,1,0};
     mask = new Img(a, 5, 5);
    //mask = new Img(a,3,3);
    delete[] a;
}
ImageProcess::ImageProcess(const Img *img) {
    srcImg = new Img(img->srcImg,img->width,img->height);
    processedImg = new Img(img->srcImg,img->width,img->height);
     int* a = new int [25] {0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,};
    //int* a = new int[9] {0, 1 , 0, 1,1,1,0,1,0};
     mask = new Img(a, 5, 5);
    //mask = new Img(a,3,3);
    delete[] a;
}
ImageProcess::ImageProcess(const char *fileName) {
    FILE* res = fopen(fileName, "r");
    int w,h;
    if(fscanf(res, "%d\t%d", &w, &h) == false){
        fclose(res);
        srcImg = new Img;
        processedImg = new Img;
         int* a = new int [25] {0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,};
        //int* a = new int[9] {0, 1 , 0, 1,1,1,0,1,0};
         mask = new Img(a, 5, 5);
        //mask = new Img(a,3,3);
        delete[] a;
        return;
    }
    int* a = new int[w*h];
    std::string datafile;
    char* buf = new char [256];
    while (fscanf(res, "%s", buf) != EOF){
        datafile += buf;
    }
    for (int i = 0; i < h*w; i++){
        a[i] = (int)datafile[i] - '0';
    }
    fclose(res);
    srcImg = new Img(a,w,h);
    processedImg = new Img(a,w,h);
    int* ar = new int [25] {0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,};
    //int* ar = new int[9] {0, 1 , 0, 1,1,1,0,1,0};
    mask = new Img(a, 5, 5);
    //mask = new Img(a,3,3);
    delete[] a;
    delete[] ar;
    delete[] buf;
}
ImageProcess::~ImageProcess() {
    delete srcImg;
    delete processedImg;
    delete mask;
    srcImg = nullptr;
    processedImg = nullptr;
    mask = nullptr;
}

void ImageProcess::clearImg(Img *&mask) {
    delete mask;
}

int ImageProcess::updateMask(const Img &NewMask) {
    clearImg(mask);
    mask = new Img(NewMask.srcImg, NewMask.width, NewMask.height);
    return 0;
}

int ImageProcess::updateSrcImg() {
    clearImg(srcImg);
    srcImg = new Img(processedImg->srcImg, processedImg->width, processedImg->height);
    return 0;
}

int **ImageProcess::createMat(int h, int w) {
    int **A = new int *[h];
    for (int i = 0; i < h; i++){
        A[i] = new int[w];
    }
    return A;
}

int **ImageProcess::deleteMat(int **A, int h, int w) {
    for(int i = 0; i < h ;i++){
        delete A[i];
    }
    delete A;
}

int ImageProcess::dilatation(int c) {
    int w = srcImg->width, h = srcImg->height;
    int** A = createMat(h, w);
    if (c == 1) {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                A[i][j] = srcImg->srcImg[i * w + j];
            }
        }
    }
    else if (c == 0) {
        w = processedImg->width;
        h = processedImg->height;
        deleteMat(A, h, w);
        int** A = createMat(h, w);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                A[i][j] = processedImg->srcImg[i * w + j]; // копирование матриц для работы
            }
        }
    }
    else {
        deleteMat(A, h, w);
        return 1;
    }
    int** buff = createMat(h, w);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            buff[i][j] = 0;
        }
    }// создание поля для работы

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (A[i][j] != mask->srcImg[(mask->width * mask->height) / 2])
                continue;
            for (int i_m = 0; i_m < mask->height; i_m++){
                for(int j_m = 0; j_m < mask->width; j_m++){
                    int h1 = i - mask->y_c + i_m;
                    int w1 = j - mask->x_c + j_m;
                    if (h1 < 0 || h1 >= h || w1 < 0 || w1 >= w || (mask->srcImg[i_m * mask->width + j_m] == 0))
                        continue;
                    if (mask->srcImg[i_m * mask->width + j_m] == 1){
                        buff[h1][w1] = 1;
                    }
                }
            }
        }
    }
    if(c == 1){
        clearImg(processedImg);
        processedImg = new Img(srcImg->srcImg, srcImg->width, srcImg->height);
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            processedImg->srcImg[i * w + j] = buff[i][j];
        }
    }
    deleteMat(buff,h,w);
    deleteMat(A,h,w);
    return 0;
}

int ImageProcess::loadImgFromFile(const char *fileName, int format) {
    FILE* load = fopen(fileName, "r");
    int w,h;
    if (fscanf(load, "%d\t%d", &w, &h) == false){
        fclose(load);
        return 1;
    }
    char* buf = new char[255];
    std::string data;
    int* NewSrc = new int[w * h];
    while (fscanf(load, "%s", buf) != EOF){
        data += buf;
    }
    for(int i = 0; i < w*h ; i++){
        int k = (int)data[i] - '0';
        NewSrc[i] = (int)data[i] - '0';
    }
    Img* loaded = new Img(NewSrc, w, h);
    clearImg(srcImg);
    srcImg = new Img(loaded->srcImg, loaded->width, loaded->height);
    delete[] buf;
    delete[] NewSrc;
    delete loaded;
    fclose(load);
    return 0;
}

int ImageProcess::saveImgToFile(const char *fileName, int format) {
    FILE* save = fopen( fileName, "w");
    fprintf(save, "%d\t%d\n", processedImg->width, processedImg->height);
    for (int i = 0; i < processedImg->height; i++){
        for (int j = 0; j < processedImg->width; j++){
            fprintf(save, "%d", processedImg->srcImg[i * processedImg->width + j]);
        }
        if(format == 1){
            fprintf(save, "\n");
        }
    }
    fclose(save);
    return 0;
}

void ImageProcess::showSrc() {
    for(int i = 0; i < (srcImg->width * srcImg->height); i++){
        std::cout << srcImg->srcImg[i];
    }
}

int ImageProcess::erosion(int c) {
    int w = srcImg->width, h = srcImg->height;
    int** A = createMat(h, w);
    if (c == 1) {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                A[i][j] = srcImg->srcImg[i * w + j];
            }
        }
    }
    else if (c == 0) {
        w = processedImg->width;
        h = processedImg->height;
        deleteMat(A, h, w);
        int** A = createMat(h, w);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                A[i][j] = processedImg->srcImg[i * w + j]; // копирование матриц для работы
            }
        }
    }
    else {
        deleteMat(A, h, w);
        return 1;
    }
    int** buff = createMat(h, w);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            buff[i][j] = 1;
        }
    }// создание поля для работы

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (A[i][j] == mask->srcImg[(mask->width * mask->height) / 2])
                continue;
            for (int i_m = 0; i_m < mask->height; i_m++){
                for(int j_m = 0; j_m < mask->width; j_m++){
                    int h1 = i - mask->y_c + i_m;
                    int w1 = j - mask->x_c + j_m;
                    if (h1 < 0 || h1 >= h || w1 < 0 || w1 >= w || (mask->srcImg[i_m * mask->width + j_m] == 0))
                        continue;
                    if (mask->srcImg[i_m * mask->width + j_m] == 1){
                        buff[h1][w1] = 0;
                    }
                }
            }
        }
    }
    if(c == 1){
        clearImg(processedImg);
        processedImg = new Img(srcImg->srcImg, srcImg->width, srcImg->height);
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            processedImg->srcImg[i * w + j] = buff[i][j];
        }
    }
    deleteMat(buff,h,w);
    deleteMat(A,h,w);
    return 0;
}
