#include <QtWidgets/QVBoxLayout>
#include "FrameSourceNavigationWidget.h"
#include "easylogging++.h"

FrameSourceNavigationWidget::FrameSourceNavigationWidget(std::shared_ptr<PipelineController> controller)
        :slider(Qt::Horizontal), controller(std::move(controller))
{
    slider.setMaximum(this->controller->getTotalFrames()-1);

    QObject::connect(&slider, &QSlider::valueChanged,
            this, &FrameSourceNavigationWidget::sliderValueChanged);
    QObject::connect(&slider, &QSlider::sliderReleased,
            this, &FrameSourceNavigationWidget::sliderReleased);

    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(&originalImageLabel);
    this->layout()->addWidget(&slider);
    this->layout()->addWidget(&indexLabel);

    originalImageLabel.setImage(this->controller->getCurrentLoadedImage());
    indexLabel.setText(QString::number(slider.tickInterval()));
}

void FrameSourceNavigationWidget::sliderValueChanged(int value)
{
    if(not slider.isSliderDown())
    {
        fireNewImage(value);
    }
}

void FrameSourceNavigationWidget::fireNewImage(int index)
{
    LOG(INFO) << "Slider value changed: " << index;

    try{
        indexLabel.setText(QString::number(index));
        controller->setFrameSourceIndex(index);
        controller->firePipelineProcessing();
    }catch(const std::exception& e)
    {
        LOG(WARNING) << "Not possible to process image at index: " << index <<
                     ". Exception message: " << e.what();
    }
}

void FrameSourceNavigationWidget::sliderReleased()
{
    fireNewImage(slider.value());
}

void FrameSourceNavigationWidget::setOriginalImage(const cv::Mat &image)
{
    originalImageLabel.setImage(image);
}
