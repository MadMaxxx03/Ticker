#ifndef EXTRAPARAMSDIALOG_H
#define EXTRAPARAMSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QList>
#include <QSerialPort>
#include <QTextEdit>
#include <QCheckBox>

class ExtraParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtraParamsDialog(QWidget *parent = nullptr,
                               QSerialPort *port = nullptr,
                               QTextEdit *logOutput = nullptr,
                               QCheckBox *swing = nullptr,
                               QCheckBox *stab = nullptr,
                               QCheckBox *filter = nullptr);
    void setSerialPort(QSerialPort* port);

private slots:
    void updateFields(int index);
    void loadSettings();
    void saveSettings();

private:
    QLabel *labelControlType;

    QComboBox *controlComboBox;

    QPushButton *saveButton;

    // Новые элементы для PID и Bang параметров
    QList<QLabel*> pidLabels;
    QList<QLineEdit*> pidEdits;

    QLabel *bangLabelFi;
    QLineEdit *bangEditFi;
    QLabel *bangLabelPWM;
    QLineEdit *bangEditPWM;

    QGridLayout *paramsLayout;

    QSerialPort* serialPort = nullptr;
    QTextEdit *logsEdit;

    QCheckBox *checkBoxSwing = nullptr;
    QCheckBox *checkBoxStab = nullptr;
    QCheckBox *checkBoxFilter = nullptr;
};

#endif // EXTRAPARAMSDIALOG_H
