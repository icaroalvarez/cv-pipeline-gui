#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <PipelineController.h>
#include "qlabelmat.h"
#include "qwidgethandler.h"

class ProcessorWidget: public QWidget {
    Q_OBJECT
public:
    ProcessorWidget(std::shared_ptr<PipelineController> controller, int indexTab);

    void setDebugImage(const cv::Mat& image);

private:
    std::shared_ptr<PipelineController> controller;
    QLabelMat imageLabel;
    QWidgetHandler widgetHandler;
    int indexTab;

public slots:
    void configureProcessor(const QString& parameterName, const QVariant& parameterValue);
};
