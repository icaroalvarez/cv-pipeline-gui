#include <QtWidgets/QVBoxLayout>
#include "FrameSourceNavigationWidget.h"
#include "easylogging++.h"

FrameSourceNavigationWidget::FrameSourceNavigationWidget()
        :slider(Qt::Horizontal)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(&originalImageLabel);
    originalImageLabel.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto navigationWidget{new QWidget()};
    navigationWidget->setLayout(new QHBoxLayout());
    navigationWidget->layout()->addWidget(&slider);
    navigationWidget->layout()->addWidget(&indexLabel);
    this->layout()->addWidget(navigationWidget);

    QObject::connect(&slider, &QSlider::valueChanged,
                     this, &FrameSourceNavigationWidget::sliderValueChanged);
    QObject::connect(&slider, &QSlider::sliderReleased,
                     this, &FrameSourceNavigationWidget::sliderReleased);
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
    try{
        indexLabel.setText(QString::number(index)+("/")+totalFramesString);
        emit firePipelineProcessing(index);
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

void FrameSourceNavigationWidget::setTotalFrames(unsigned int totalFrames)
{
    totalFramesString = QString::number(totalFrames);
    slider.setMaximum(static_cast<int>(totalFrames));
    indexLabel.setText(QString::number(slider.value())+("/")+totalFramesString);
}