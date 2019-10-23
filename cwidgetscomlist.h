#ifndef CWIDGETSCOMLIST_H
#define CWIDGETSCOMLIST_H

#include <QWidget>
#include <QtWidgets>
#include "cwidgetscomeditor.h"

using namespace HD_SCOM;


//Debug prefix
#define CScomList_DEBUG_PREFIX ("CScomList: ")

class CWidgetScomList : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetScomList(QWidget *parent = nullptr);

    // Output debug log
    void OutputDebugLog(QString sFunction, QString sType, hd_common::e_Error Code);

    // Output debug log
    void OutputDebugLog(QString sFunction, QString sType, QString sText);


private:
    //QTableview object to scomlist
    QTableWidget* qTableViewScomList;

    QPushButton* qPushButtonInsert;

    QPushButton* qPushButtonRemove;

    QPushButton* qPushButtonCopy;

    QPushButton* qPushButtonLoad;

    QPushButton* qPushButtonSave;

    QStringList sListCommandListHeader;

    bool bCommandListRun;

signals:

    void SignalAskForScomQueueItemFromEditorToList(void);

    void SignalMoveScomQueueItemFromListToEditor(ScomEditData mScomEditData);

    void SignalScomQueueItemToEditorReady(void);

    void SignalScomQueueItemEmpty(bool bEmpty);

    //signal to output to log box
    void SignalLogOutput(QString sLog);

private slots:
    void PrivateSlotInsertScomQueueItem(void);

    void PrivateSlotRemoveScomQueueItem(void);

    bool PrivateSlotCopyScomQueueItem(void);

    void PrivateSlotSaveList(void);

    void PrivateSlotLoadList(void);

public slots:

    //Reset all widgets
    void PublicSlotResetWidgets(void);

    //slot to accept scom queue item to be inserted
    void PublicSlotInsertScomQueueItemFromEditor(ScomEditData mScomEditData);

    //slot to load copy current item from list to editor
    void PublicSlotCopyScomQueueItemToEditor(void);

    //slot to receive run result of scom queue
    void PublicSlotScomQueueItemFinish(hd_common::e_Error eRet);

    //Slot to accept scom list run/cancel
    void PublicSlotScomQueueRunCancel(bool bRunCancel);
};

#endif // CWIDGETSCOMLIST_H
