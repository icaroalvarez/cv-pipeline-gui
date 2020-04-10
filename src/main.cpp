#include "mainwindow.h"
#include <QApplication>
#include "easylogging++.h"
#include "Controller.h"

INITIALIZE_EASYLOGGINGPP

std::string getConfigurationFilePath(int argc, char *argv[])
{
    if (argc<2)
    {
        LOG(ERROR) << "incorrect number of arguments, usage: panoramix-qt configuration_file.json";
        exit(1);
    }
    return argv[1];
}

int main(int argc, char *argv[])
{
    const auto configurationFilePath{getConfigurationFilePath(argc, argv)};
    QApplication a(argc, argv);
    const auto window{std::make_shared<MainWindow>()};
    auto controller{Controller::createController(window)};
    controller->loadPipelineConfiguration(configurationFilePath);
    window->show();
    controller->firePipelineProcessing();
    //window.setWindowState(Qt::WindowMaximized);

    return a.exec();
}