#include "qr_detect.h"

const char* qr_code() {
    string data="";
    VideoCapture camera(0);
    if (!camera.isOpened()) {
        return data.c_str();
    }

    camera.set(CAP_PROP_FRAME_WIDTH, 320);
    camera.set(CAP_PROP_FRAME_HEIGHT, 240);
    camera.set(CAP_PROP_FPS, 120);
    camera.set(CAP_PROP_BRIGHTNESS, 40);
    camera.set(CAP_PROP_CONTRAST, 50);
    camera.set(CAP_PROP_EXPOSURE, 156);

    Mat frame;
    camera >> frame;
   
   
    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    Mat gray_image;
    cvtColor(frame, gray_image, COLOR_BGR2GRAY);

    int width = gray_image.cols;
    int height = gray_image.rows;
    uchar *raw = (uchar *)(gray_image.data);

    Image zbarImage(width, height, "Y800", raw, width * height);
    scanner.scan(zbarImage);

    auto symbol=zbarImage.symbol_begin();
    if (symbol!=zbarImage.symbol_end()){data = symbol->get_data();}
    else {return data.c_str();}
   
    camera.release();
    destroyAllWindows();
    return data.c_str();
}