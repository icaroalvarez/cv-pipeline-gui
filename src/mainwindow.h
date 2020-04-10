#pragma once

#include <QMainWindow>
#include <QtCore/QVariant>
#include "qwidgethandler.h"
#include "FrameSourceNavigationWidget.h"
#include <QtWidgets/QSplitter>

namespace Ui {
    class MainWindow;
}

using ProcessorsParameters = std::vector<std::pair<std::string, Parameters>>;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void createProcessorTabs(const ProcessorsParameters& pipelineConfiguration);
    void setInputImage(const cv::Mat& image);
    void setDebugImage(unsigned int processorIndex, const cv::Mat& image);
    void setTotalFrames(unsigned int totalFrames);

private:
    Ui::MainWindow *ui;
    QTabWidget tabWidget;
    FrameSourceNavigationWidget frameSourceNavigation;

signals:
    void sendProcessorConfiguration(unsigned int index, const Configuration& configuration);
    void firePipelineProcessing(unsigned int frameIndex);
    void sendSavePipelineConfiguration(const std::string& path);

public slots:
    void savePipelineConfiguration();
};
