#include <iostream>
#include <QHBoxLayout>
#include "ProcessorWidget.h"

ProcessorWidget::ProcessorWidget(unsigned int processorIndex, const Parameters& processorParameters)
        :index(processorIndex)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(&imageLabel);
    imageLabel.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->layout()->addWidget(widgetHandler.createQWidgetFromParameters(processorParameters));

    QObject::connect(&widgetHandler, &QWidgetHandler::configureProcessor,
                     this, &ProcessorWidget::configureProcessor);
}

void ProcessorWidget::configureProcessor(const QString& parameterName, const QVariant& parameterValue)
{
    Configuration configuration;

    if(parameterValue.type() == QVariant::Type::Int)
    {
        configuration[parameterName.toStdString()] = parameterValue.toInt();
    }else if(parameterValue.type() == QVariant::Type::Double)
    {
        configuration[parameterName.toStdString()] = parameterValue.toDouble();
    }else if(parameterValue.type() == QVariant::Type::Bool)
    {
        configuration[parameterName.toStdString()] = parameterValue.toBool();
    }else if(parameterValue.type() == QVariant::Type::UInt)
    {
        configuration[parameterName.toStdString()] = static_cast<std::size_t>(parameterValue.toInt());
    }else{
        throw std::invalid_argument("Processor widget receive unknown parameter: "+std::string(parameterValue.typeName()));
    }

    emit sendProcessorConfiguration(index, configuration);
}

void ProcessorWidget::setDebugImage(const cv::Mat& image)
{
    imageLabel.setImage(image);
}