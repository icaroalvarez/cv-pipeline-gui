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
    void addFloatControlTo(QLayout *layout, std::string name, float value, float minValue,
                           float maxValue, float step, int decimals);

    void addIntControlTo(QLayout *layout, std::string name, int value, int minValue, int maxValue);

    void addBooleanControlTo(QLayout *layout, std::string name, bool value);

    void addOptionsControlTo(QLayout *layout, std::string name, std::vector<std::string> options, int selected);

public slots:
    void receiveInt(int value);
    void receiveDouble(double value);
    void receiveBool(bool value);
    void receiveOption(int option_selected);

signals:
    void configureProcessor(QString processorName, QVariant value);
};