#include <iostream>
#include <QHBoxLayout>
#include "ProcessorWidget.h"
#include "easylogging++.h"

ProcessorWidget::ProcessorWidget(std::shared_ptr<PipelineController> controller, int indexTab)
        :controller(std::move(controller)), indexTab(indexTab)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(&imageLabel);

    const auto parameters{this->controller->getProcessorParameters(static_cast<unsigned int>(indexTab))};
    this->layout()->addWidget(widgetHandler.createQWidgetFromParameters(parameters));

    QObject::connect(&widgetHandler, &QWidgetHandler::configureProcessor,
                     this, &ProcessorWidget::configureProcessor);

    this->controller->firePipelineProcessing();
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

    controller->configureProcessor(static_cast<unsigned int>(indexTab), configuration);
    controller->firePipelineProcessing();
}

void ProcessorWidget::setDebugImage(const cv::Mat& image)
{
    imageLabel.setImage(image);
}