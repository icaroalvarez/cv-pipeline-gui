#include "mainwindow.h"
#include <QApplication>
#include <Registerer.h>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const auto controller{std::make_shared<PipelineController>()};
    Registerer::registerProcessors(controller);
    const std::vector<std::string> pipeLineDescription{"adaptive_threshold", "roi", "resize"};
    controller->loadPipeline(pipeLineDescription);

    std::string path("/home/ialvarez/Code/panoramix/test/fixtures/Lenna.png");
    controller->loadFrameSourceFrom(path);

    const auto window{std::make_shared<MainWindow>(controller)};
    controller->registerObserver(window);
    window->show();
    //window.setWindowState(Qt::WindowMaximized);

    return a.exec();
}