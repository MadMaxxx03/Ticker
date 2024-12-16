#ifndef EXTRAPARAMSDIALOG_H
#define EXTRAPARAMSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

class ExtraParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtraParamsDialog(QWidget *parent = nullptr);

private:
    QLabel *labelControlType;
    QLabel *labelControlParam1;
    QLabel *labelControlParam2;
    QLabel *labelObserverType;
    QLabel *labelObserverParam1;
    QLabel *labelObserverParam2;
    QComboBox *controlComboBox;
    QComboBox *observerComboBox;
    QLineEdit *editControlParam1;
    QLineEdit *editControlParam2;
    QLineEdit *editObserverParam1;
    QLineEdit *editObserverParam2;
    QPushButton *saveButton;
};

#endif // EXTRAPARAMSDIALOG_H
