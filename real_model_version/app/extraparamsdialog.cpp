#include "extraparamsdialog.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>

ExtraParamsDialog::ExtraParamsDialog(QWidget *parent) : QDialog(parent)
{
    QFont labelFont("Arial", 10); // Настройка шрифта

    // Управление
    QHBoxLayout *controlНLayout = new QHBoxLayout;
    QVBoxLayout *controlLVLayout = new QVBoxLayout;
    QVBoxLayout *controlEVLayout = new QVBoxLayout;
    QHBoxLayout *controlMenuLayout = new QHBoxLayout;
    QVBoxLayout *controLayout = new QVBoxLayout;

    labelControlType = new QLabel("Вид");
    labelControlType->setFont(labelFont);
    labelControlParam1 = new QLabel("Параметр 1");
    labelControlParam1->setFont(labelFont);
    labelControlParam2 = new QLabel("Параметр 2");
    labelControlParam2->setFont(labelFont);

    controlMenuLayout->addWidget(labelControlType);

    controlLVLayout->addWidget(labelControlParam1);
    controlLVLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    controlLVLayout->addWidget(labelControlParam2);

    controlComboBox = new QComboBox();
    controlComboBox->setFont(labelFont);
    controlComboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    controlComboBox->setMaximumWidth(300);
    controlComboBox->addItem("Sliding-mode");
    controlComboBox->addItem("Back stepping");
    controlComboBox->addItem("Адаптивное управление");
    controlComboBox->addItem("Model Predictive Control");

    editControlParam1 = new QLineEdit;
    editControlParam1->setFont(labelFont);
    editControlParam1->setMaximumWidth(120);
    editControlParam1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editControlParam2 = new QLineEdit;
    editControlParam2->setMaximumWidth(120);
    editControlParam2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editControlParam2->setFont(labelFont);

    controlMenuLayout->addWidget(controlComboBox);
    controlMenuLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    controlEVLayout->addWidget(editControlParam1);
    controlEVLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    controlEVLayout->addWidget(editControlParam2);

    controlНLayout->addLayout(controlLVLayout);
    controlНLayout->addLayout(controlEVLayout);

    controLayout->addLayout(controlMenuLayout);
    controLayout->addLayout(controlНLayout);

    QGroupBox *controlGroup = new QGroupBox("Управление");
    controlGroup->setLayout(controLayout);

    // Наблюдатель
    QHBoxLayout *observerНLayout = new QHBoxLayout;
    QVBoxLayout *observerLVLayout = new QVBoxLayout;
    QVBoxLayout *observerEVLayout = new QVBoxLayout;
    QHBoxLayout *observerMenuLayout = new QHBoxLayout;
    QVBoxLayout *observerLayout = new QVBoxLayout;

    labelObserverType = new QLabel("Вид");
    labelObserverType->setFont(labelFont);
    labelObserverParam1 = new QLabel("Параметр 1");
    labelObserverParam1->setFont(labelFont);
    labelObserverParam2 = new QLabel("Параметр 2");
    labelObserverParam2->setFont(labelFont);

    observerMenuLayout->addWidget(labelObserverType);

    observerLVLayout->addWidget(labelObserverParam1);
    observerLVLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    observerLVLayout->addWidget(labelObserverParam2);

    observerComboBox = new QComboBox();
    observerComboBox->setFont(labelFont);
    observerComboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    observerComboBox->setMaximumWidth(300);
    observerComboBox->addItem("Без наблюдателя");
    observerComboBox->addItem("Наблюдатель с большим коэффициентом усиления");
    observerComboBox->addItem("Наблюдатель Калмана");
    observerComboBox->addItem("Адаптивный наблюдатель");
    observerComboBox->addItem("Нелинейный наблюдатель");

    editObserverParam1 = new QLineEdit;
    editObserverParam1->setFont(labelFont);
    editObserverParam1->setMaximumWidth(120);
    editObserverParam1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editObserverParam2 = new QLineEdit;
    editObserverParam2->setMaximumWidth(120);
    editObserverParam2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editObserverParam2->setFont(labelFont);

    observerMenuLayout->addWidget(observerComboBox);
    observerMenuLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    observerEVLayout->addWidget(editObserverParam1);
    observerEVLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    observerEVLayout->addWidget(editObserverParam2);

    observerНLayout->addLayout(observerLVLayout);
    observerНLayout->addLayout(observerEVLayout);

    observerLayout->addLayout(observerMenuLayout);
    observerLayout->addLayout(observerНLayout);

    QGroupBox *observerGroup = new QGroupBox("Наблюдатель");
    observerGroup->setLayout(observerLayout);

    // Кнопка "Сохранить"
    saveButton = new QPushButton("Сохранить");
    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);

    // Основной макет
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(controlGroup);
    mainLayout->addWidget(observerGroup);
    mainLayout->addWidget(saveButton);

    setLayout(mainLayout);
    setWindowTitle("Параметры");
}
