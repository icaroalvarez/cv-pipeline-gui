#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProcessorWidget.h"
#include "easylogging++.h"

MainWindow::MainWindow(std::shared_ptr<PipelineController> pipelineController, QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        controller{std::move(pipelineController)},
        frameSourceNavigation(controller)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(new QHBoxLayout());
    ui->centralWidget->layout()->setSizeConstraint(QLayout::SetMinimumSize);

    // create tabs with each processors processor
    for(const auto &processor : controller->getPipelineDescription())
    {
        auto* processorWidget{new ProcessorWidget(controller, tabWidget.count())};
        tabWidget.addTab(processorWidget, QString::fromStdString(processor));
    }

    // add widgets to splitter
    auto *splitter{new QSplitter()};
    splitter->addWidget(&frameSourceNavigation);
    splitter->addWidget(&tabWidget);
    ui->centralWidget->layout()->addWidget(splitter);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 5);

    // register main window as an observer of controller to be notified when image processing is finished
    frameSourceNavigation.sliderReleased();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    frameSourceNavigation.setOriginalImage(controller->getCurrentLoadedImage());
    for(int processorImageIndex = 0; processorImageIndex < tabWidget.count(); processorImageIndex++)
    {
        try
        {
            const auto debugImage{controller->getDebugImage(static_cast<unsigned int>(processorImageIndex))};
            const auto processorWidget{dynamic_cast<ProcessorWidget *>(tabWidget.widget(processorImageIndex))};
            processorWidget->setDebugImage(debugImage);
        }catch(const std::exception& e)
        {
            LOG(WARNING) << e.what();
        }
    }
}
