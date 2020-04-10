#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProcessorWidget.h"
#include "easylogging++.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(new QHBoxLayout());
    ui->centralWidget->layout()->setSizeConstraint(QLayout::SetMinimumSize);

    // add widgets to splitter
    auto *splitter{new QSplitter()};
    splitter->addWidget(&frameSourceNavigation);
    splitter->addWidget(&tabWidget);
    ui->centralWidget->layout()->addWidget(splitter);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 5);

    QObject::connect(&frameSourceNavigation, &FrameSourceNavigationWidget::firePipelineProcessing,
                     this, &MainWindow::firePipelineProcessing);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createProcessorTabs(const ProcessorsParameters& processorsParameters)
{
    for(const auto& processorParameters : processorsParameters)
    {
        const auto& processorName{std::get<std::string>(processorParameters)};
        const auto& parameters{std::get<Parameters>(processorParameters)};
        auto* processorWidget{new ProcessorWidget(tabWidget.count(), parameters)};
        tabWidget.addTab(processorWidget, QString::fromStdString(processorName));

        QObject::connect(processorWidget, &ProcessorWidget::sendProcessorConfiguration,
                this, &MainWindow::sendProcessorConfiguration);
    }
}

void MainWindow::setInputImage(const cv::Mat& image)
{
    frameSourceNavigation.setOriginalImage(image);
}

void MainWindow::setDebugImage(unsigned int processorIndex, const cv::Mat &image)
{
    auto widget{tabWidget.widget(static_cast<int>(processorIndex))};
    const auto processorWidget{dynamic_cast<ProcessorWidget*>(widget)};
    processorWidget->setDebugImage(image);
}

void MainWindow::setTotalFrames(unsigned int totalFrames)
{
    frameSourceNavigation.setTotalFrames(totalFrames);
}
