#ifndef CWIDGETSCOMEDITOR_H
#define CWIDGETSCOMEDITOR_H

#include <QWidget>
#include <QtWidgets>
#include "cscomqueue.h"

using namespace HD_SCOM;
using namespace  hd_common;

//Debug prefix
#define CScomEditor_DEBUG_PREFIX ("CWidgetScomEditor: ")

//Struct to hold scom editor data
struct ScomEditData
{
    QString qStringCmdID;
    QString qStringAddr;
    QString qStringData;
    QString qStringFile;
    QString qStringExpACK;
    QString qStringExpData;
    QString qStringMaxRepeatTimes;
    QString qStringDelayAfter;
    bool bWaitUntil;
    bool bCritical;
};

class CWidgetSCOMEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetSCOMEditor(QWidget *parent = 0);

private:
    //Groupbox for command set
    QGroupBox* qGroupBoxCommand;
    //Combobox for SCOM Command
    QComboBox* qComboBoxCommand;
    //Label for Address
    QLabel* qLabelAddress;
    //EditBox for Address
    QLineEdit* qLineEditAddress;
    //Radiobutton to select data
    QRadioButton* qRadioButtonData;
    //EditBox for data
    QLineEdit* qLineEditData;
    //Radiobutton to select file directory
    QCheckBox* qCheckBoxFile;
    //EditBox for data
    QLineEdit* qLineEditFile;
    //Pushbutton for browse file
    QPushButton* qPushButtonBrowse;


    //Groupbox for expected result
    QGroupBox* qGroupBoxExpectedResult;
    //Combobox for Expected Command
    QComboBox* qComboBoxExpectedCommand;
    //Checkbox to enable expected command data
    QCheckBox* qCheckBoxExpectedData;
    //EditBox for expected data
    QLineEdit* qLineEditExpectedData;

    //Groupbox for additional requirement
    QGroupBox* qGroupBoxAdditional;
    //Label for Times
    QLabel* qLabelTimes;
    //EditBox for Times
    QLineEdit* qLineEditTimes;
    //Checkbox to enable wait for expected result
    QCheckBox* qCheckBoxWaitUntil;
    //Checkbox to set if the item is critical
    QCheckBox* qCheckBoxCritical;
    //Label for delay after action
    QLabel* qLabelDelayAfter;
    //EditBox for delay after action
    QLineEdit* qLineEditDelayAfter;



    //QString for valid SCOM command
    QStringList qStringListSCOMCommand;

    //QString for valid Answers
    QStringList qStringListSCOMAnswer;

    void OutputDebugLog(QString sFunction, QString sType, e_Error code);

signals:
    SignalResponseScomEditorDataToScomQueue(SCOMQueueItem mScomQueueItem);

    SignalResponseScomEditorDataToList(ScomEditData mScomEditData);


    SignalLogOutput(QString sStringOutput);

public slots:
    void PublicSlotRequestScomEditorDataToScomQueue(void);

    void PublicSlotRequestScomEditorDataToList(void);

    void PublicSlotMoveScomEditorDataFromList(ScomEditData mScomEditData);

private slots:
    void PrivateSlotLineEditData(void);

    void PrivateSlotLineEditExpectedData(void);

    void PrivateSlotPushButtonBrowse(void);

};

#endif // CWIDGETSCOMEDITOR_H
