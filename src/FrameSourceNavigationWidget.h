#pragma once
#include <QtWidgets/QWidget>
#include <PipelineController.h>
#include <QtWidgets/QSlider>
#include <QObject>
#include "qlabelmat.h"

class FrameSourceNavigationWidget: public QWidget {
    Q_OBJECT
public:
    explicit FrameSourceNavigationWidget(std::shared_ptr<PipelineController> controller);
    void setOriginalImage(const cv::Mat& image);

private:
    QSlider slider;
    QLabel indexLabel;
    std::shared_ptr<PipelineController> controller;
    QLabelMat originalImageLabel;

public slots:
    void sliderValueChanged(int value);
    void sliderReleased();
    void fireNewImage(int index);
};