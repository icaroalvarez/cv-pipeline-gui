#include <iostream>
#include <QHBoxLayout>
#include "ProcessorWidget.h"
#include "easylogging++.h"

ProcessorWidget::ProcessorWidget(PipelineController *controller, int indexTab)
        :controller(controller), indexTab(indexTab)
{
    this->setLayout(new QVBoxLayout());

    this->layout()->addWidget(&imageLabel);

    const auto parameters{controller->getProcessorParameters(static_cast<unsigned int>(indexTab))};

    this->layout()->addWidget(widgetHandler.createQWidgetFromParameters(parameters));

    // connect parameters widget
    QObject::connect(&widgetHandler, SIGNAL(configureProcessor(QString, QVariant)),
                     this, SLOT(configureProcessor(QString, QVariant)));

    controller->firePipelineProcessing();
}

void ProcessorWidget::configureProcessor(const QString& paramName, const QVariant& value)
{
    Configuration configuration;

    if(std::string(value.typeName()) == "int")
    {
        configuration[paramName.toStdString()] = value.toInt();
    }else if(std::string(value.typeName()) == "double")
    {
        configuration[paramName.toStdString()] = value.toDouble();
    }else if(std::string(value.typeName()) == "bool")
    {
        configuration[paramName.toStdString()] = value.toBool();
    }else if(std::string(value.typeName()) == "QString")
    {
        configuration[paramName.toStdString()] = value.toInt();
    }else{
        throw std::invalid_argument("Processor widget receive unknown parameter: "+std::string(value.typeName()));
    }

    controller->configureProcessor(static_cast<unsigned int>(indexTab), configuration);
    controller->firePipelineProcessing();
}

void ProcessorWidget::setDebugImage(const cv::Mat& image) {
    imageLabel.setImage(image);
}