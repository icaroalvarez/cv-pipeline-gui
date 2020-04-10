#include <Registerer.h>
#include "Controller.h"

Controller::Controller(std::shared_ptr<MainWindow> mainWindow)
        :pipelineController{std::make_shared<PipelineController>()},
         mainWindow{std::move(mainWindow)},
         totalPipelineProcessors{0}
{
    Registerer::registerProcessors(pipelineController);

    QObject::connect(this->mainWindow.get(), &MainWindow::sendProcessorConfiguration,
                     this, &Controller::receiveProcessorConfiguration);

    QObject::connect(this->mainWindow.get(), &MainWindow::firePipelineProcessing,
                     this, QOverload<unsigned int>::of(&Controller::firePipelineProcessing));
}

std::shared_ptr<Controller> Controller::createController(std::shared_ptr<MainWindow> mainWindow)
{
    return std::shared_ptr<Controller>(new Controller(std::move(mainWindow)));
}

void Controller::loadPipelineConfiguration(const std::string &configurationFilePath)
{
    pipelineController->registerObserver(shared_from_this());
    pipelineController->loadPipelineFromJsonFile(configurationFilePath);
    const auto pipelineConfiguration{pipelineController->getPipelineConfiguration()};
    totalPipelineProcessors = pipelineConfiguration.imageProcessors.size();
    mainWindow->setTotalFrames(pipelineController->getTotalFrames());

    ProcessorsParameters processorsParameters;
    for(int i=0; i < pipelineConfiguration.imageProcessors.size(); i++)
    {
        processorsParameters.emplace_back(std::get<std::string>(pipelineConfiguration.imageProcessors[i]),
                                          pipelineController->getProcessorParameters(i));
    }
    mainWindow->createProcessorTabs(processorsParameters);
}

void Controller::update()
{
    mainWindow->setInputImage(pipelineController->getCurrentLoadedImage());

    for(auto processorImageIndex = 0; processorImageIndex < totalPipelineProcessors; processorImageIndex++)
    {
        mainWindow->setDebugImage(processorImageIndex, pipelineController->getDebugImageFrom(processorImageIndex));
    }
}

void Controller::receiveProcessorConfiguration(unsigned int index, const Configuration &configuration)
{
    pipelineController->configureProcessor(index, configuration);
    firePipelineProcessing();
}

void Controller::firePipelineProcessing(unsigned int frameIndex)
{
    pipelineController->setFrameSourceIndex(frameIndex);
    firePipelineProcessing();
}

void Controller::firePipelineProcessing()
{
    pipelineController->firePipelineProcessing();
}
