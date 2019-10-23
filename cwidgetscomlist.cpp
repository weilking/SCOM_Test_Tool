#include "cwidgetscomlist.h"

CWidgetScomList::CWidgetScomList(QWidget *parent) : QWidget(parent)
{
    QGridLayout *qGridLayoutMain = new QGridLayout();



    QVBoxLayout *qVBoxLayout = new QVBoxLayout();

    qVBoxLayout->addStretch(1);

    //(0,0) Insert Button
    qPushButtonInsert = new QPushButton(this);
    qPushButtonInsert->setText("Insert");

    qVBoxLayout->addWidget(qPushButtonInsert, 0);

    qVBoxLayout->addStretch(1);

    //(1, 0) Copy Button
    qPushButtonCopy = new QPushButton(this);
    qPushButtonCopy->setText("Copy");

    qVBoxLayout->addWidget(qPushButtonCopy, 0);

     qVBoxLayout->addStretch(1);

    //(2,0) Remove Button
    qPushButtonRemove = new QPushButton(this);
    qPushButtonRemove->setText("Remove");

    qVBoxLayout->addWidget(qPushButtonRemove, 0);

    qVBoxLayout->addStretch(4);

    qGridLayoutMain->addLayout(qVBoxLayout, 0, 0);

    //(0,1) Scom List table
    QGridLayout *qGridLayoutSub1 = new QGridLayout();

    QHBoxLayout *qHBoxLayout = new QHBoxLayout();

    qHBoxLayout->addStretch(3);

    qPushButtonLoad = new QPushButton(this);
    qPushButtonLoad->setText("Load");

    qHBoxLayout->addWidget(qPushButtonLoad, 0);

    qHBoxLayout->addStretch(8);

    qPushButtonSave = new QPushButton(this);
    qPushButtonSave->setText("Save");

    qHBoxLayout->addWidget(qPushButtonSave, 0);

    qGridLayoutSub1->addLayout(qHBoxLayout, 0, 0, 1, 1);


    qTableViewScomList = new QTableWidget(this);


    qTableViewScomList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    qTableViewScomList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    qTableViewScomList->horizontalHeader()->setHighlightSections(true);
    qTableViewScomList->horizontalHeader()->setStretchLastSection(false);


    sListCommandListHeader << "CMDID" << "ADDR" << "DATA" << "FILE"
                           << "EXPACK" << "EXPDATA"
                           << "DELAY" << "TIMES" << "WAIT" << "CRITICAL";

    qTableViewScomList->setRowCount(1);
    qTableViewScomList->setColumnCount(sListCommandListHeader.size());
    qTableViewScomList->setHorizontalHeaderLabels(sListCommandListHeader);

    qGridLayoutSub1->addWidget(qTableViewScomList, 1, 0, 1, 1);


    qGridLayoutMain->addLayout(qGridLayoutSub1, 0, 1, 1, 1);

    //Finish layout setting
    setLayout(qGridLayoutMain);


    //Connect Signals
    connect(qPushButtonInsert, SIGNAL(clicked(bool)), this, SLOT(PrivateSlotInsertScomQueueItem()));
    connect(qPushButtonRemove, SIGNAL(clicked(bool)), this, SLOT(PrivateSlotRemoveScomQueueItem()));
    connect(qPushButtonCopy, SIGNAL(clicked(bool)), this, SLOT(PrivateSlotCopyScomQueueItem()));

    connect(qPushButtonSave,SIGNAL(clicked(bool)), this, SLOT(PrivateSlotSaveList()));
    connect(qPushButtonLoad, SIGNAL(clicked(bool)), this, SLOT(PrivateSlotLoadList()));


    //Initialize command list
    bCommandListRun = false;
}

void CWidgetScomList::OutputDebugLog(QString sFunction, QString sType, e_Error Code)
{
    QString sStringOutput;

    sStringOutput = QString("[%3]%1[%2][0x%4]").arg(CScomList_DEBUG_PREFIX, sFunction, sType, QString::number(Code,16));

    qDebug() << sStringOutput;

    //Emit signal to log box receiving slot
    emit SignalLogOutput(sStringOutput);

    return;
}

void CWidgetScomList::OutputDebugLog(QString sFunction, QString sType, QString sText)
{
    QString sStringOutput;

    sStringOutput = QString("[%3]%1[%2][%4]").arg(CScomList_DEBUG_PREFIX, sFunction, sType, sText);

    qDebug() << sStringOutput;

    //Emit signal to log box receiving slot
    emit SignalLogOutput(sStringOutput);

    return;
}

void CWidgetScomList::PrivateSlotInsertScomQueueItem()
{
    emit SignalAskForScomQueueItemFromEditorToList();
}

void CWidgetScomList::PrivateSlotRemoveScomQueueItem()
{
    if(!qTableViewScomList->item(qTableViewScomList->currentRow(),0) || qTableViewScomList->item(qTableViewScomList->currentRow(),0)->text().isEmpty())
    {
        //Empty command ID, error?

    }
    else
    {
        qTableViewScomList->removeRow(qTableViewScomList->currentRow());
    }

}

bool CWidgetScomList::PrivateSlotCopyScomQueueItem()
{
    ScomEditData mScomEditData;
    bool bRet = true;

    //Get current selected item intable
    if(qTableViewScomList->currentRow() < 0)
    {
        qTableViewScomList->setCurrentCell(0,0);
    }

    if(!qTableViewScomList->item(qTableViewScomList->currentRow(),0) || qTableViewScomList->item(qTableViewScomList->currentRow(),0)->text().isEmpty())
    {
        //Empty command ID, error?
        bRet = false;
    }
    else
    {
        //Collect all items in the current selected row and emit signal
        mScomEditData.qStringCmdID = qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("CMDID"))->text();
        mScomEditData.qStringAddr = qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("ADDR"))->text();
        mScomEditData.qStringData = qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("DATA"))->text();
        mScomEditData.qStringFile = qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("FILE"))->text();
        mScomEditData.qStringExpACK = qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("EXPACK"))->text();
        mScomEditData.qStringExpData = qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("EXPDATA"))->text();
        mScomEditData.qStringDelayAfter = qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("DELAY"))->text();
        mScomEditData.qStringMaxRepeatTimes = qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("TIMES"))->text();

        QString qStringTmp = qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("CRITICAL"))->text();
        mScomEditData.bCritical = qStringTmp.isEmpty();

        qStringTmp = qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("WAIT"))->text();
        mScomEditData.bWaitUntil = qStringTmp.isEmpty();

        //Emit signal to scom edit box
        emit SignalMoveScomQueueItemFromListToEditor(mScomEditData);
    }

    return bRet;
}

void CWidgetScomList::PrivateSlotSaveList()
{
    QString qStringFilename =  QFileDialog::getSaveFileName(
              this,
              "Save Command List",
              QDir::currentPath(),
              "Command List (*.scomlst) ;; All files (*.*)");

    QFile QFileSaved(qStringFilename);
    if (!QFileSaved.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        OutputDebugLog(__FUNCTION__,"ERROR","Unable to save list.");
        return;
    }

    QTextStream out(&QFileSaved);
    out << "SCOM_COMMAND_LIST\n";
    out << "1.0\n";
    out << QDate::currentDate().toString("dd.MM.yyyy") << " " << QTime::currentTime().toString("hh:mm:ss") << "\n";

    QStringList qStringListRow;

    for(int iTmpRow = 0; iTmpRow < qTableViewScomList->rowCount() - 1; iTmpRow++ )
    {
        qStringListRow.clear();
        for (int iTmpColumn = 0; iTmpColumn < qTableViewScomList->columnCount(); iTmpColumn++)
        {

            if (qTableViewScomList->item(iTmpRow, iTmpColumn)->text().length() > 0)
            {
                qStringListRow.append("\"" + qTableViewScomList->item(iTmpRow, iTmpColumn)->text() + "\"");
            }
            else
            {
                qStringListRow.append("");
            }


        }
        out << qStringListRow.join(";") + "\n";
    }

    QFileSaved.close();

    OutputDebugLog(__FUNCTION__,"STATUS","List is saved.");
}

void CWidgetScomList::PrivateSlotLoadList()
{
    QString qStringFilename =  QFileDialog::getOpenFileName(
              this,
              "Load Command List",
              QDir::currentPath(),
              "Command List (*.scomlst) ;; All files (*.*)");

    QFile QFileLoaded(qStringFilename);
    if (!QFileLoaded.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        OutputDebugLog(__FUNCTION__,"ERROR","Can't open list file.");
        return;
    }

    QTextStream in(&QFileLoaded);

    QString qString1stLine = in.readLine();
    if(!qString1stLine.contains("SCOM_COMMAND_LIST"))
    {
        //Invalid Scom list file
        OutputDebugLog(__FUNCTION__,"ERROR","Invalid List File.");
        return;
    }

    qString1stLine = in.readLine();

    int iRowCount = 0;

    if(qString1stLine.contains("1.0"))
    {
        //Read extra line for date
        in.readLine();


        //Clear current table view
        qTableViewScomList->setRowCount(0);
        //Leave first row as blank
        qTableViewScomList->setRowCount(1);

        while (!in.atEnd())
        {

            // read one line from textstream(separated by "\n")
            QString qStringRow = in.readLine();


            //Insert one row to list
            qTableViewScomList->insertRow(iRowCount);

            // parse the read line into separate pieces(tokens) with "," as the delimiter
            QStringList qStringListRow = qStringRow.split(";");

            // load parsed data to model accordingly
            for (int iTmp = 0; iTmp < qStringListRow.size(); iTmp++) {
                    QString qStringValue = qStringListRow.at(iTmp);
                    qStringValue.remove('\"');

                    qTableViewScomList->setItem(iRowCount, iTmp ,new QTableWidgetItem(qStringValue));
                }

            iRowCount++;
        }

        OutputDebugLog(__FUNCTION__,"STATUS","List is loaded.");
    }
    else
    {
        OutputDebugLog(__FUNCTION__,"ERROR","Invalid List File.");
    }


    return;
}

void CWidgetScomList::PublicSlotResetWidgets()
{

}

void CWidgetScomList::PublicSlotInsertScomQueueItemFromEditor(ScomEditData mScomEditData)
{
    QTableWidgetItem *QTableWidgetItemNew = new QTableWidgetItem(0);

    //insert row in tableview
    if(qTableViewScomList->currentRow() < 0)
    {
        qTableViewScomList->setCurrentCell(0, 0);
    }

    //Insert row
    qTableViewScomList->insertRow(qTableViewScomList->currentRow());
    qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("CMDID") ,new QTableWidgetItem(mScomEditData.qStringCmdID));
    qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("ADDR") ,new QTableWidgetItem(mScomEditData.qStringAddr));
    qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("DATA") ,new QTableWidgetItem(mScomEditData.qStringData));
    qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("FILE") ,new QTableWidgetItem(mScomEditData.qStringFile));
    qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("EXPACK") ,new QTableWidgetItem(mScomEditData.qStringExpACK));
    qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("EXPDATA") ,new QTableWidgetItem(mScomEditData.qStringExpData));
    qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("DELAY") ,new QTableWidgetItem(mScomEditData.qStringDelayAfter));
    qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("TIMES") ,new QTableWidgetItem(mScomEditData.qStringMaxRepeatTimes));

    if(mScomEditData.bCritical)
    {
        qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("CRITICAL") ,new QTableWidgetItem("Y"));
    }
    else
    {
        qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("CRITICAL") ,new QTableWidgetItem("N"));
    }

    if(mScomEditData.bWaitUntil)
    {
        qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("WAIT") ,new QTableWidgetItem("Y"));
    }
    else
    {
        qTableViewScomList->setItem(qTableViewScomList->currentRow()-1, sListCommandListHeader.indexOf("WAIT") ,new QTableWidgetItem("N"));
    }


}

//Public slot of copy scom queue item from list to editor
void CWidgetScomList::PublicSlotCopyScomQueueItemToEditor(void)
{

    //Call copy
    if(!PrivateSlotCopyScomQueueItem())
    {
        //No data in current row
        //notify the list is finished.
        OutputDebugLog(__FUNCTION__, "STATUS" , "SCOM List Run is over!");
        emit SignalScomQueueItemEmpty(true);
    }
    else
    {
        //emit signal: Editor data ready
        emit SignalScomQueueItemToEditorReady();
    }

}

void CWidgetScomList::PublicSlotScomQueueItemFinish(e_Error eRet)
{
    //Color the item
    if(eRet == e_OK)
    {
        qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("CMDID"))->setBackgroundColor(Qt::green);
    }
    else
    {
        qTableViewScomList->item(qTableViewScomList->currentRow(),sListCommandListHeader.indexOf("CMDID"))->setBackgroundColor(Qt::red);
    }

    //call public slot: get next item
    qTableViewScomList->setCurrentCell(qTableViewScomList->currentRow() + 1, 0);

    if(!bCommandListRun)
    {
        OutputDebugLog(__FUNCTION__, "STATUS" , "SCOM List Run is cancelled!");
    }
    else
    {
        PublicSlotCopyScomQueueItemToEditor();
    }
}

void CWidgetScomList::PublicSlotScomQueueRunCancel(bool bRunCancel)
{
    bCommandListRun = bRunCancel;

    if(true == bCommandListRun)
    {
        OutputDebugLog(__FUNCTION__, "STATUS" , "SCOM List Run is started!");
        PublicSlotCopyScomQueueItemToEditor();
    }
}
