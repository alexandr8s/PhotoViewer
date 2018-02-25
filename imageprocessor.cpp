#include "imageprocessor.h"

ImageProcessor::ImageProcessor(QObject *parent) : QObject(parent)
{
}

void ImageProcessor::load(QString path, QSize size, QListWidgetItem * current)
{
    mx.lock();
    cv::Mat cvimage = cv::imread(path.toStdString(), CV_LOAD_IMAGE_COLOR);
    QImage img = QImage((const unsigned char*)(cvimage.data), cvimage.cols, cvimage.rows, QImage::Format_RGB888).rgbSwapped();

    emit ready(img.scaled(size.width(), size.height(), Qt::KeepAspectRatio), current);
    mx.unlock();
}

