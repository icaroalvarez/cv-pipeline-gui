#pragma once
#include "PipelineController.h"
#include "mainwindow.h"
#include <QObject>

using PipelineObserver = Observer;

/**
 * Responsibility: Manage interaction between pipelineController and GUI Main Window.
 */
class Controller: public QObject, public PipelineObserver, public std::enable_shared_from_this<Controller>
{
Q_OBJECT
public:

    static std::shared_ptr<Controller> createController(std::shared_ptr<MainWindow> mainWindow);

    void loadPipelineConfiguration(const std::string& configurationFilePath);

    void update() override;

    void firePipelineProcessing();

private:
    std::shared_ptr<PipelineController> pipelineController;
    std::shared_ptr<MainWindow> mainWindow;
    unsigned int totalPipelineProcessors;

    explicit Controller(std::shared_ptr<MainWindow> mainWindow);

public slots:
    void receiveProcessorConfiguration(unsigned int index, const Configuration& configuration);
    void firePipelineProcessing(unsigned int frameIndex);
    void savePipelineConfiguration(const std::string& path);
};



