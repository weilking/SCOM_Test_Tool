#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cwidgetserialport.h"
#include "cwidgetlogbox.h"
#include "cwidgetscomeditor.h"
#include "cwidgetcontrolpanel.h"
#include "cscomqueue.h"
#include "cwidgetscomlist.h"

using namespace HD_SCOM;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QWidget *centralWidget;

    CWidgetSerialPort* mWidgetSerialPort;
    CWidgetLogBox* mWidgetLogBox;
    CWidgetSCOMEditor* mWidgetSCOMEditor;
    CWidgetControlPanel* mWidgetControlPanel;
    CWidgetScomList* mWidgetScomList;

    CScomQueue* mScomQueue;

private:
    void ConnectSignalSlots(void);

};

#endif // MAINWINDOW_H
