#include "mainwindow.h"
#include <QApplication>
#include <Registerer.h>
#include "easylogging++.h"
#include <nlohmann/json.hpp>

INITIALIZE_EASYLOGGINGPP

using json = nlohmann::json;

json loadConfigurationFile(int argc, char *argv[])
{
    if (argc<2)
    {
        LOG(ERROR) << "incorrect number of arguments, usage: panoramix-qt configuration_file.json";
        exit(1);
    }
    std::string configurationPath(argv[1]);
    std::ifstream configurationFile(configurationPath);

    if (not configurationFile.good())
    {
        LOG(ERROR) << "couldn't load configuration file: "+configurationPath;
        exit(1);
    }
    json configurationJsonFile;
    configurationFile >> configurationJsonFile;
    return configurationJsonFile;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    const auto configurationFile = loadConfigurationFile(argc, argv);

    const auto controller{std::make_shared<PipelineController>()};
    Registerer::registerProcessors(controller);
    controller->loadPipelineFromJson(configurationFile);

    const auto window{std::make_shared<MainWindow>(controller)};
    controller->registerObserver(window);
    window->show();
    //window.setWindowState(Qt::WindowMaximized);

    return a.exec();
}