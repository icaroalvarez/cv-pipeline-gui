#include "qwidgethandler.h"
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include "easylogging++.h"

void QWidgetHandler::receiveInt(int value)
{
    configureProcessor(sender()->objectName(), value);
}

void QWidgetHandler::receiveDouble(double value)
{
    configureProcessor(sender()->objectName(), value);
}

void QWidgetHandler::receiveBool(bool value)
{
    configureProcessor(sender()->objectName(), value);
}

void QWidgetHandler::receiveOption(int option_selected)
{
    configureProcessor(sender()->objectName(), option_selected);
}

void QWidgetHandler::addIntControlTo(QLayout *layout, const std::string& name, int value, int minValue, int maxValue)
{
    auto *hBoxLayout{new QHBoxLayout()};
    auto *label{new QLabel(QString::fromStdString(name))};
    auto *spinBox{new QSpinBox()};
    spinBox->setMaximum(maxValue);
    spinBox->setMinimum(minValue);
    spinBox->setValue(value);
    spinBox->QObject::setObjectName(QString::fromStdString(name));

    QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QWidgetHandler::receiveInt);
    hBoxLayout->addWidget(label);
    hBoxLayout->addWidget(spinBox);
    auto *widgetInt{new QWidget()};
    widgetInt->setLayout(hBoxLayout);
    layout->addWidget(widgetInt);
}

void QWidgetHandler::addFloatControlTo(QLayout *layout, const std::string& name, float value, float minValue,
                                       float maxValue, float step, int decimals)
{
    auto *hBoxLayout{new QHBoxLayout()};
    auto *label{new QLabel(QString::fromStdString(name))};
    auto *spinBox{new QDoubleSpinBox()};
    spinBox->setDecimals(decimals);
    spinBox->setMaximum(maxValue);
    spinBox->setMinimum(minValue);
    spinBox->setSingleStep(step);
    spinBox->setValue(value);
    spinBox->QObject::setObjectName(QString::fromStdString(name));

    QObject::connect(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                     this, &QWidgetHandler::receiveDouble);
    hBoxLayout->addWidget(label);
    hBoxLayout->addWidget(spinBox);
    auto *widget{new QWidget()};
    widget->setLayout(hBoxLayout);
    layout->addWidget(widget);
}

void QWidgetHandler::addBooleanControlTo(QLayout *layout, const std::string& name, bool value)
{
    auto *checkbox{new QCheckBox(QString::fromStdString(name))};
    checkbox->setChecked(value);
    checkbox->QObject::setObjectName(QString::fromStdString(name));
    QObject::connect(checkbox, &QCheckBox::toggled, this, &QWidgetHandler::receiveBool);
    layout->addWidget(checkbox);
}

void QWidgetHandler::addOptionsControlTo(QLayout *layout, const std::string& name,
                                         const std::vector<std::string>& options, int selected)
{
    auto *comboBox{new QComboBox()};
    for (const auto& option : options)
    {
        comboBox->addItem(QString::fromStdString(option));
        comboBox->QObject::setObjectName(QString::fromStdString(name));
        comboBox->setCurrentIndex(selected);
        QObject::connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                         this, &QWidgetHandler::receiveOption);
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
