#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "string.h"
#include <map>

#ifndef DILATATION_EROSION_HW2_H
#define DILATATION_EROSION_HW2_H

struct Img {
    int* srcImg;
    int width;
    int height;

    int x_c;
    int y_c;

    Img();
    Img(int w, int h);
    Img(const int* src, int w, int h);
    ~Img();
};

class ImageProcess {
private :
    Img* srcImg;
    Img* processedImg;
    Img* mask;

    int** createMat(int h, int w);

    int** deleteMat(int** A, int h, int w);

    void clearImg(Img* &mask);
    //здесь можно объявлять другие переменные и функции
public :
    //здесь нельзя объявлять функции и переменные
    ImageProcess();
    /* выделяется память для картинки размера w*h */
    explicit ImageProcess(int w, int h);
    /* выделяется память для картинки размера w*h и копируется картинка*/
    explicit ImageProcess(const Img* img);
    /* выделяется память для картинки копируется картинка из файла, первая строчка в файле содержит ширину и высоту картинки*/
    explicit ImageProcess(const char* fileName);
    ~ImageProcess();

    void showSrc();

    /*задание маски*/
    int updateMask(const Img& NewMask);
    /*перезаписать исходную картинку картинкой, которая была получена в результате дилатации и/или эрозии*/
    int updateSrcImg();
    /*дилатация картинки, результат записать в processedImg*/
    /*
        input		:	command = 1, обрабатывается srcImg  - результат записывается в processedImg
                        command = 0, обрабатывается processedImg - результат записывается в processedImg
    */
    int dilatation(int c);	//исправить на dilatation
    /*эрозия картинки, результат записать в processedImg*/
    int erosion(int c);
    /*
        загрузить/сохранить из файла с имененм fileName картинку
        input		:	fileName - имя файла
                        format - формат файла 	0 - последовательность 0 и 1, без перехода на новую строку
                                                1 - последовательность 0 и 1, с переходом на новую строку
    */
    int loadImgFromFile(const char* fileName, int format = 1);
    int saveImgToFile(const char* fileName, int format = 1);
};

#endif //DILATATION_EROSION_HW2_H
