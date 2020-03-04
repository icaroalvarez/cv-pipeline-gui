#include "qwidgethandler.h"
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include "easylogging++.h"

void QWidgetHandler::receiveInt(int value)
{
    QObject* pObject = sender();
    configureProcessor(pObject->objectName(), value);
}

void QWidgetHandler::receiveDouble(double value)
{
    QObject* pObject = sender();
    configureProcessor(pObject->objectName(), value);
}

void QWidgetHandler::receiveBool(bool value)
{
    QObject* pObject = sender();
    configureProcessor(pObject->objectName(), value);
}

void QWidgetHandler::receiveOption(int option_selected)
{
    QObject* pObject = sender();
    configureProcessor(pObject->objectName(), option_selected);
}

void QWidgetHandler::addIntControlTo(QLayout *layout, std::string name, int value, int minValue, int maxValue) {
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    QLabel *label = new QLabel(QString::fromStdString(name));
    QSpinBox *spinBox = new QSpinBox();
    spinBox->setMaximum(maxValue);
    spinBox->setMinimum(minValue);
    spinBox->setValue(value);
    spinBox->QObject::setObjectName(QString::fromStdString(name));

    QObject::connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(receiveInt(int)));
    hBoxLayout->addWidget(label);
    hBoxLayout->addWidget(spinBox);
    QWidget *widgetInt = new QWidget();
    widgetInt->setLayout(hBoxLayout);
    layout->addWidget(widgetInt);
}

void QWidgetHandler::addFloatControlTo(QLayout *layout, std::string name, float value, float minValue,
                                       float maxValue, float step, int decimals) {
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    QLabel *label = new QLabel(QString::fromStdString(name));
    QDoubleSpinBox *spinBox = new QDoubleSpinBox();
    spinBox->setDecimals(decimals);
    spinBox->setMaximum(maxValue);
    spinBox->setMinimum(minValue);
    spinBox->setSingleStep(step);
    spinBox->setValue(value);
    spinBox->QObject::setObjectName(QString::fromStdString(name));

    QObject::connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(receiveDouble(double)));
    hBoxLayout->addWidget(label);
    hBoxLayout->addWidget(spinBox);
    QWidget *widget = new QWidget();
    widget->setLayout(hBoxLayout);
    layout->addWidget(widget);
}

void QWidgetHandler::addBooleanControlTo(QLayout *layout, std::string name, bool value) {
    QCheckBox *checkbox = new QCheckBox(QString::fromStdString(name));
    checkbox->setChecked(value);
    checkbox->QObject::setObjectName(QString::fromStdString(name));
    QObject::connect(checkbox, SIGNAL(toggled(bool)), this, SLOT(receiveBool(bool)));
    layout->addWidget(checkbox);
}

void QWidgetHandler::addOptionsControlTo(QLayout *layout, std::string name,
        std::vector<std::string> options, int selected)
{
    QComboBox *comboBox = new QComboBox();
    for (auto& option : options) {
        comboBox->addItem(QString::fromStdString(option));
        comboBox->QObject::setObjectName(QString::fromStdString(name));
        comboBox->setCurrentIndex(selected);
        QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(receiveOption(int)));
        layout->addWidget(comboBox);
    }
}

// overloaded helper from std::variant documentation
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20
QWidget* QWidgetHandler::createQWidgetFromParameters(const Parameters& parameters)
{
    QWidget *widget = new QWidget();
    widget->setLayout(new QVBoxLayout());

    for(const auto &parameter : parameters)
    {
        const auto parameterName{parameter.first};
        std::visit(overloaded{
                [&](IntegerParameter integerParameter)
                {
                    addIntControlTo(widget->layout(), parameterName, integerParameter.value,
                                    integerParameter.minValue, integerParameter.maxValue);
                },
                [&](DecimalParameter decimalParameter)
                {
                    addFloatControlTo(widget->layout(), parameterName, decimalParameter.value,
                                      decimalParameter.minValue, decimalParameter.maxValue,
                                      decimalParameter.incrementalStep, decimalParameter.decimalsPlaces);
                },
                [&](bool value)
                {
                    addBooleanControlTo(widget->layout(), parameterName, value);
                },
                [&](OptionsParameter optionsParameter)
                {
                    addOptionsControlTo(widget->layout(), parameterName,
                            optionsParameter.options, optionsParameter.selectedOptionIndex);
                }
        }, parameter.second);
    }
    return widget;
}
