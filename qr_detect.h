#ifndef QR_CODE_SCANNER_H
#define QR_CODE_SCANNER_H

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include <zbar.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;
using namespace zbar;


#ifdef __cplusplus
extern "C" {
#endif

const char* qr_code();

#ifdef __cplusplus
}

#endif
#endif