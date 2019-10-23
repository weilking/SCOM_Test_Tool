#include "cwidgetscomeditor.h"

CWidgetSCOMEditor::CWidgetSCOMEditor(QWidget *parent) : QWidget(parent)
{

    QGridLayout *qGridLayoutMain = new QGridLayout;
    //qGridLayoutMain->setSizeConstraint(QLayout::SetFixedSize);

    //GroupBox Genesis Command
    qGroupBoxCommand = new QGroupBox("SCOM Command", this);

    qComboBoxCommand = new QComboBox(this);
    qComboBoxCommand->setEditable(true);

    qStringListSCOMCommand << "BRI" << "BRM" << "BRT" << "BRU" << "BRL"
                           << "---" << "GDC" << "TBC" << "GBF" << "HDL" << "CAL" << "ODM" << "TGP" << "REC" << "GMB" << "CFB" << "LED"
                           << "---" << "BZZ" << "BUC"
                           << "---" << "GPI" << "CIP"
                           << "---" << "CDP" << "RDP" << "GPO"
                           << "---" << "SNB" << "PSN"
                           << "---" << "TYP" << "PDT"
                           << "---" << "PWR" << "CXP" << "ETC" << "SWI" << "SWV" << "TMP" << "LIS" << "USI" << "UCF" << "CF?" << "DCF"
                           << "UIC" << "SDM" << "DLN" << "DL?" << "EDC" << "EDS" << "CBR" << "BUS" << "ATX"
                           << "---" << "WDC" << "WDF"
                           << "---" << "MCC";

    qComboBoxCommand->addItems(qStringListSCOMCommand);

    qLabelAddress = new QLabel("Address", this);
    qLabelAddress->setFixedWidth(50);

    qLineEditAddress = new QLineEdit(this);
    qLineEditAddress->setInputMask("HH;_");
    qLineEditAddress->setText("FF");
    qLineEditAddress->setFixedWidth(50);

    qRadioButtonData = new QRadioButton("Data", this);
    qRadioButtonData->setChecked(true);
    qRadioButtonData->setEnabled(false);

    qLineEditData =  new QLineEdit(this);
    qLineEditData->setInputMask("HH;_");
    connect(qLineEditData, SIGNAL(textChanged(QString)), this, SLOT(PrivateSlotLineEditData()));

    connect(qRadioButtonData, SIGNAL(toggled(bool)), qLineEditData, SLOT(setEnabled(bool)));

    qCheckBoxFile = new QCheckBox("File", this);
    qCheckBoxFile->setChecked(false);

    qLineEditFile = new QLineEdit(this);
    qLineEditFile->setEnabled(false);

    qPushButtonBrowse = new QPushButton(this);
    qPushButtonBrowse->setText("Browse..");
    qPushButtonBrowse->setEnabled(false);
    connect(qPushButtonBrowse, SIGNAL(clicked()), this, SLOT(PrivateSlotPushButtonBrowse()));

    connect(qCheckBoxFile, SIGNAL(toggled(bool)), qLineEditFile, SLOT(setEnabled(bool)));
    connect(qCheckBoxFile, SIGNAL(toggled(bool)), qPushButtonBrowse, SLOT(setEnabled(bool)));

    QGridLayout* qGridLayoutGroupBoxCommand = new QGridLayout;
    qGridLayoutGroupBoxCommand->addWidget(qComboBoxCommand, 0, 0);
    qGridLayoutGroupBoxCommand->addWidget(qLabelAddress, 0, 1);
    qGridLayoutGroupBoxCommand->addWidget(qLineEditAddress, 0, 2);
    qGridLayoutGroupBoxCommand->addWidget(qRadioButtonData, 1, 1);
    qGridLayoutGroupBoxCommand->addWidget(qLineEditData, 1, 2, 1, 9);
    qGridLayoutGroupBoxCommand->addWidget(qCheckBoxFile, 2, 1);
    qGridLayoutGroupBoxCommand->addWidget(qLineEditFile, 2, 2);
    qGridLayoutGroupBoxCommand->addWidget(qPushButtonBrowse, 2, 9);

    qGroupBoxCommand->setLayout(qGridLayoutGroupBoxCommand);

    //GroupBox ExpectedExpected Result
    qGroupBoxExpectedResult = new QGroupBox("Expected Result", this);

    qComboBoxExpectedCommand = new QComboBox(this);
    qComboBoxExpectedCommand->setEditable(true);

    qStringListSCOMAnswer << "ACK" << "NAK" << "CMD";

    qComboBoxExpectedCommand->addItems(qStringListSCOMAnswer);

    qCheckBoxExpectedData = new QCheckBox("Data",this);

    qLineEditExpectedData = new QLineEdit(this);
    qLineEditExpectedData->setInputMask("HH;_");
    qLineEditExpectedData->setEnabled(false);

    connect(qCheckBoxExpectedData, SIGNAL(toggled(bool)), qLineEditExpectedData, SLOT(setEnabled(bool)));
    connect(qLineEditExpectedData, SIGNAL(textChanged(QString)), this, SLOT(PrivateSlotLineEditExpectedData()));

    QGridLayout* qGridLayoutGroupBoxExpectedResult = new QGridLayout;
    qGridLayoutGroupBoxExpectedResult->addWidget(qComboBoxExpectedCommand, 0, 0);
    qGridLayoutGroupBoxExpectedResult->addWidget(qCheckBoxExpectedData, 0, 1);
    qGridLayoutGroupBoxExpectedResult->addWidget(qLineEditExpectedData, 0, 2);

    qGroupBoxExpectedResult->setLayout(qGridLayoutGroupBoxExpectedResult);

    //Groupbox for additional requirement
    qGroupBoxAdditional = new QGroupBox("Additional Options", this);

    qLabelTimes = new QLabel("Times", this);
    qLabelTimes->setFixedWidth(50);


    qLineEditTimes = new QLineEdit(this);
    qLineEditTimes->setInputMask("99;");
    qLineEditTimes->setText("01");
    qLineEditTimes->setFixedWidth(50);

    qCheckBoxWaitUntil = new QCheckBox("Wait Until", this);

    qCheckBoxCritical = new QCheckBox("Critical", this);

    qLabelDelayAfter = new QLabel("Delay After Action", this);
    //qLabelDelayAfter->setFixedWidth(50);

    qLineEditDelayAfter =  new QLineEdit(this);
    qLineEditDelayAfter->setInputMask("99999;");
    qLineEditDelayAfter->setText("0");
    qLineEditDelayAfter->setFixedWidth(50);

    QGridLayout* qGridLayoutGroupBoxAdditional = new QGridLayout;
    qGridLayoutGroupBoxAdditional->addWidget(qLabelTimes, 0, 0);
    qGridLayoutGroupBoxAdditional->addWidget(qLineEditTimes, 0, 1);
    qGridLayoutGroupBoxAdditional->addWidget(qCheckBoxWaitUntil, 0, 2);
    qGridLayoutGroupBoxAdditional->addWidget(qCheckBoxCritical, 1, 0);
    qGridLayoutGroupBoxAdditional->addWidget(qLabelDelayAfter, 2, 0);
    qGridLayoutGroupBoxAdditional->addWidget(qLineEditDelayAfter, 2, 1);

    qGroupBoxAdditional->setLayout(qGridLayoutGroupBoxAdditional);

    qGridLayoutMain->addWidget(qGroupBoxCommand, 0, 0);
    qGridLayoutMain->addWidget(qGroupBoxExpectedResult, 1, 0);
    qGridLayoutMain->addWidget(qGroupBoxAdditional, 2, 0);

    setLayout(qGridLayoutMain);

}

void CWidgetSCOMEditor::OutputDebugLog(QString sFunction, QString sType, e_Error code)
{
    QString sStringOutput;

    sStringOutput = QString("[%3]%1[%2][0x%4]").arg(CScomEditor_DEBUG_PREFIX, sFunction, sType, QString::number(code,16));

    qDebug() << sStringOutput;

    //Emit signal to log box receiving slot
    emit SignalLogOutput(sStringOutput);

    return;
}

//Public slot: Request SCOM editor data to SCOM queue
void CWidgetSCOMEditor::PublicSlotRequestScomEditorDataToScomQueue()
{
    SCOM_MSG mScomMsgCommand, mScomMsgExpected;

    //1. Collect Command data from editor
    mScomMsgCommand.att = SCOM_ATT_CMD;
    mScomMsgCommand.address = (unsigned char)qLineEditAddress->text().toInt(NULL,16);

    QByteArray qByteArrayCmdID = qComboBoxCommand->currentText().toLocal8Bit();
    if(qByteArrayCmdID.length() < 3)
    {
        OutputDebugLog(__FUNCTION__,"ERROR",e_SCOM_DATA_LENGTH_INV);
        return;
    }

    memcpy(mScomMsgCommand.cmdid.id, qByteArrayCmdID.data(),3);

    QByteArray qByteArrayData;
    unsigned char u8IDCHK = 0;
    QStringList qStringListData = qLineEditData->text().split(" ");
    foreach(QString qStringSingleData, qStringListData)
    {

        qStringSingleData.remove('_');
        if(qStringSingleData.length() != 0)
        {
            //Add to bytearray
            qByteArrayData += (unsigned char)qStringSingleData.toInt(NULL, 16);
            u8IDCHK += (unsigned char)qByteArrayData.back();
        }

    }

    memcpy(mScomMsgCommand.data,qByteArrayData.data(),qByteArrayData.size());
    mScomMsgCommand.len = qByteArrayData.size();
    mScomMsgCommand.idchk = ~u8IDCHK;

    //Calculate checksum
    unsigned char u8IHCHK = 0;


    //Calculate header checksum
    u8IHCHK += mScomMsgCommand.att;
    u8IHCHK += mScomMsgCommand.cmdid.id[0];
    u8IHCHK += mScomMsgCommand.cmdid.id[1];
    u8IHCHK += mScomMsgCommand.cmdid.id[2];
    u8IHCHK += mScomMsgCommand.address;
    u8IHCHK += qByteArrayData.size();

    mScomMsgCommand.ihchk = ~u8IHCHK;

    //2. Collect Expected response from editor
    QString qStringExpectedAtt = qComboBoxExpectedCommand->currentText();
    if(qStringExpectedAtt == "ACK")
    {
        mScomMsgExpected.att = SCOM_ATT_ACK;
    }
    else if(qStringExpectedAtt == "NAK")
    {
        mScomMsgExpected.att = SCOM_ATT_NACK;
    }
    else
    {
        mScomMsgExpected.att = SCOM_ATT_CMD;
    }

    mScomMsgExpected.address = mScomMsgCommand.address;
    memcpy(mScomMsgExpected.cmdid.id, qByteArrayCmdID.data(),3);


    if(qCheckBoxExpectedData->isChecked())
    {
        //Checked data box
        qStringListData = qLineEditExpectedData ->text().split(" ");
        qByteArrayData.clear();
        foreach(QString qStringSingleData, qStringListData)
        {

            qStringSingleData.remove('_');
            if(qStringSingleData.length() != 0)
            {
                //Add to bytearray
                qByteArrayData += (unsigned char)qStringSingleData.toInt(NULL, 16);
            }

        }
        memcpy(mScomMsgExpected.data,qByteArrayData.data(),qByteArrayData.size());
        mScomMsgExpected.len = qByteArrayData.size();
    }
    else
    {
        mScomMsgExpected.len = 0;
    }

    //3. Emit collected data
    SCOMQueueItem mScomQueueItem;

    mScomQueueItem.mScomMsgSend = mScomMsgCommand;
    mScomQueueItem.mScomMsgExpectedAnswer = mScomMsgExpected;
    mScomQueueItem.bCritical = qCheckBoxCritical->isChecked();
    mScomQueueItem.uintDelayAfter = qLineEditDelayAfter->text().toInt();
    mScomQueueItem.uintMaxRepeatTime = qLineEditTimes->text().toInt();

    emit SignalResponseScomEditorDataToScomQueue(mScomQueueItem);

    return;

}

void CWidgetSCOMEditor::PublicSlotRequestScomEditorDataToList()
{
    ScomEditData mScomEditData;

    mScomEditData.qStringCmdID = qComboBoxCommand->currentText();
    mScomEditData.qStringAddr = qLineEditAddress->text();
    mScomEditData.qStringData = qLineEditData->text();
    if(qCheckBoxFile->isChecked())
    {
        mScomEditData.qStringFile = qLineEditFile->text();
    }
    else
    {
        mScomEditData.qStringFile = "";
    }

    mScomEditData.qStringExpACK = qComboBoxExpectedCommand->currentText();
    if(qCheckBoxExpectedData->isChecked())
    {
        mScomEditData.qStringExpData = qLineEditExpectedData->text();
    }

    mScomEditData.qStringMaxRepeatTimes = qLineEditTimes->text();
    mScomEditData.qStringDelayAfter = qLineEditDelayAfter->text();

    mScomEditData.bWaitUntil = qCheckBoxWaitUntil->isChecked();
    mScomEditData.bCritical = qCheckBoxCritical->isChecked();

    emit SignalResponseScomEditorDataToList(mScomEditData);

}

void CWidgetSCOMEditor::PublicSlotMoveScomEditorDataFromList(ScomEditData mScomEditData)
{
    qComboBoxCommand->setCurrentText(mScomEditData.qStringCmdID);
    qLineEditAddress->setText(mScomEditData.qStringAddr);

    //Disconnect lineedit signal
    disconnect(qLineEditData, SIGNAL(textChanged(QString)), this, NULL);

    QString qStringTmpMask = "";

    for(int iTmp = 0; iTmp < (mScomEditData.qStringData.size()/3);iTmp++)
    {
        qStringTmpMask += "HH ";
    }
    qStringTmpMask += "HH;_";

    qLineEditData->setInputMask(qStringTmpMask);
    qLineEditData->setCursorPosition(0);

    qLineEditData->setText(mScomEditData.qStringData);

    //Re-connect lineedit signal
    connect(qLineEditData, SIGNAL(textChanged(QString)), this, SLOT(PrivateSlotLineEditData()));

    if(!mScomEditData.qStringFile.isEmpty())
    {
        qCheckBoxFile->setChecked(true);
        qLineEditFile->setText(mScomEditData.qStringFile);
    }
    else
    {
        qCheckBoxFile->setChecked(false);
        qLineEditFile->clear();
    }

    qComboBoxExpectedCommand->setCurrentText(mScomEditData.qStringExpACK);

    if(!mScomEditData.qStringExpData.isEmpty())
    {
        qCheckBoxExpectedData->setChecked(true);

        //Disconnect lineedit signal
        disconnect(qLineEditExpectedData, SIGNAL(textChanged(QString)), this, NULL);

        qStringTmpMask = "";

        for(int iTmp = 0; iTmp < (mScomEditData.qStringExpData.size()/3);iTmp++)
        {
            qStringTmpMask += "HH ";
        }
        qStringTmpMask += "HH;_";

        qLineEditExpectedData->setInputMask(qStringTmpMask);
        qLineEditExpectedData->setCursorPosition(0);


        qLineEditExpectedData->setText(mScomEditData.qStringExpData);

        //Re-connect lineedit signal
        connect(qLineEditExpectedData, SIGNAL(textChanged(QString)), this, SLOT(PrivateSlotLineEditExpectedData()));


    }
    else
    {
        qCheckBoxExpectedData->setChecked(false);
        qLineEditExpectedData->clear();
    }

    qLineEditTimes->setText(mScomEditData.qStringMaxRepeatTimes);
    qCheckBoxWaitUntil->setChecked(mScomEditData.bWaitUntil);

    qLineEditDelayAfter->setText(mScomEditData.qStringDelayAfter);
    qCheckBoxCritical->setChecked(mScomEditData.bCritical);

}

void CWidgetSCOMEditor::PrivateSlotLineEditData()
{
    QString qStringTmpMask = qLineEditData->inputMask();
    QString qStringTmpMask2 = qStringTmpMask;
    qStringTmpMask2.resize(qStringTmpMask.size() - 2);

    int iTextLength = qStringTmpMask2.size();

    int iTextEditLength = qLineEditData->text().size();
    if(iTextEditLength ==  iTextLength)
    {
        //Extended mask

        qStringTmpMask.resize(qStringTmpMask.size() - 2);

        qStringTmpMask += " HH;_";
        qLineEditData->setInputMask(qStringTmpMask);
        qLineEditData->setCursorPosition(qStringTmpMask.size() - 4);
    }
    else if(iTextLength > (iTextEditLength + 2))
    {
        qStringTmpMask.resize(qStringTmpMask.size() - 5);
        qStringTmpMask += ";_";
        qLineEditData->setInputMask(qStringTmpMask);
        qLineEditData->setCursorPosition(qStringTmpMask.size() - 3);
    }
    else
    {

    }

}

void CWidgetSCOMEditor::PrivateSlotLineEditExpectedData()
{
    QString qStringTmpMask = qLineEditExpectedData->inputMask();
    QString qStringTmpMask2 = qStringTmpMask;
    qStringTmpMask2.resize(qStringTmpMask.size() - 2);

    int iTextLength = qStringTmpMask2.size();

    int iTextEditLength = qLineEditExpectedData->text().size();
    if(iTextEditLength ==  iTextLength)
    {
        //Extended mask

        qStringTmpMask.resize(qStringTmpMask.size() - 2);

        qStringTmpMask += " HH;_";
        qLineEditExpectedData->setInputMask(qStringTmpMask);
        qLineEditExpectedData->setCursorPosition(qStringTmpMask.size() - 4);
    }
    else if(iTextLength > (iTextEditLength + 2))
    {
        qStringTmpMask.resize(qStringTmpMask.size() - 5);
        qStringTmpMask += ";_";
        qLineEditExpectedData->setInputMask(qStringTmpMask);
        qLineEditExpectedData->setCursorPosition(qStringTmpMask.size() - 3);
    }
}

void CWidgetSCOMEditor::PrivateSlotPushButtonBrowse()
{
    QString qStringOpenFilePath;
    qStringOpenFilePath = QFileDialog::getOpenFileName(this,tr("Select File"),"",tr("Config file (*.hdcfg)"));

    qLineEditFile->setText(qStringOpenFilePath);

}
