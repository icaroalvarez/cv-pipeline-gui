#include "qlabelmat.h"
#include <opencv2/imgproc.hpp>
#include <QDebug>

void QLabelMat::resizeEvent(QResizeEvent * e)
{
    if(!qImage.isNull())
    {
        showFrame();
    }
}

QImage createQImageFrom(const cv::Mat& image)
{
    QImage qImage;
    if(image.channels() > 1)
    {
        cv::Mat imageRGB;
        cv::cvtColor(image, imageRGB, cv::COLOR_BGR2RGB);
        qImage = QImage((const uchar*) imageRGB.data, imageRGB.cols,
                        imageRGB.rows, static_cast<int>(imageRGB.step), QImage::Format_RGB888);
        qImage.bits();
    }else
    {
        qImage = QImage((const uchar*) image.data, image.cols,
                        image.rows, static_cast<int>(image.step), QImage::Format_Indexed8);
        qImage.bits();
    }
    return qImage;
}

void QLabelMat::setImage(const cv::Mat& image)
{
    qImage = createQImageFrom(image);
    if(!qImage.isNull())
    {
        showFrame();
    }
}

void QLabelMat::showFrame()
{
    setPixmap(QPixmap::fromImage(qImage).scaled(QSize(this->width(), this->height()),
                                              Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
