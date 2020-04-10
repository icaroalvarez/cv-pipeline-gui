#pragma once
#include <QGridLayout>
#include <QObject>
#include <QScrollArea>
#include <ProcessorParameters.h>
#include <QtCore/QVariant>

class QWidgetHandler: public QObject
{
Q_OBJECT

public:
    QWidget* createQWidgetFromParameters(const Parameters& parameters);

private:
    void addDoubleControlTo(QLayout *layout, const std::string& name, double value, double minValue,
                            double maxValue, double step, int decimals);

    void addIntControlTo(QLayout *layout, const std::string& name, int value, int minValue, int maxValue);

    void addBooleanControlTo(QLayout *layout, const std::string& name, bool value);

    void addOptionsControlTo(QLayout *layout, const std::string& name, const std::vector<std::string>& options, int selected);

public slots:
    void receiveInt(int value);
    void receiveDouble(double value);
    void receiveBool(bool value);
    void receiveOption(int option_selected);

signals:
    void configureProcessor(QString processorName, QVariant value);
};