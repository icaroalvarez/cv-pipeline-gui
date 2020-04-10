#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <PipelineController.h>
#include "qlabelmat.h"
#include "qwidgethandler.h"

class ProcessorWidget: public QWidget {
    Q_OBJECT
public:
    ProcessorWidget(unsigned int processorIndex, const Parameters& processorParameters);

    void setDebugImage(const cv::Mat& image);

private:
    QLabelMat imageLabel;
    QWidgetHandler widgetHandler;
    unsigned int index;

public slots:
    void configureProcessor(const QString& parameterName, const QVariant& parameterValue);

signals:
    void sendProcessorConfiguration(unsigned int index, const Configuration& configuration);
};
