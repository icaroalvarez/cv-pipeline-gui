#pragma once
#include <QLabel>
#include <opencv2/core.hpp>

class QLabelMat: public QLabel
{
public:
    void setImage(const cv::Mat& image);

public slots:
    void resizeEvent(QResizeEvent *) override;

private:
    QImage qImage;
    void showFrame();
};