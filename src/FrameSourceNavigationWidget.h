#pragma once
#include <QtWidgets/QWidget>
#include <PipelineController.h>
#include <QtWidgets/QSlider>
#include <QObject>
#include "qlabelmat.h"

class FrameSourceNavigationWidget: public QWidget {
Q_OBJECT
public:
    FrameSourceNavigationWidget();
    void setOriginalImage(const cv::Mat& image);
    void setTotalFrames(unsigned int totalFrames);

private:
    QSlider slider;
    QLabel indexLabel;
    QLabelMat originalImageLabel;
    QString totalFramesString;

public slots:
    void sliderValueChanged(int value);
    void sliderReleased();
    void fireNewImage(int index);

signals:
    void firePipelineProcessing(unsigned int frameIndex);
};