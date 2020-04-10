#include <iostream>
#include <QHBoxLayout>
#include "ProcessorWidget.h"

ProcessorWidget::ProcessorWidget(unsigned int processorIndex, const Parameters& processorParameters)
        :index(processorIndex)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(&imageLabel);
    this->layout()->addWidget(widgetHandler.createQWidgetFromParameters(processorParameters));

    QObject::connect(&widgetHandler, &QWidgetHandler::configureProcessor,
                     this, &ProcessorWidget::configureProcessor);
}

void ProcessorWidget::configureProcessor(const QString& parameterName, const QVariant& parameterValue)
{
    Configuration configuration;

    if(std::string(parameterValue.typeName()) == "int")
    {
        configuration[parameterName.toStdString()] = parameterValue.toInt();
    }else if(std::string(parameterValue.typeName()) == "double")
    {
        configuration[parameterName.toStdString()] = parameterValue.toDouble();
    }else if(std::string(parameterValue.typeName()) == "bool")
    {
        configuration[parameterName.toStdString()] = parameterValue.toBool();
    }else if(std::string(parameterValue.typeName()) == "QString")
    {
        configuration[parameterName.toStdString()] = parameterValue.toInt();
    }else{
        throw std::invalid_argument("Processor widget receive unknown parameter: "+std::string(parameterValue.typeName()));
    }

    emit sendProcessorConfiguration(index, configuration);
}

void ProcessorWidget::setDebugImage(const cv::Mat& image)
{
    imageLabel.setImage(image);
}