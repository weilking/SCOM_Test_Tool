#ifndef CWIDGETLOGBOX_H
#define CWIDGETLOGBOX_H

#include <QWidget>
#include <QtWidgets>

#define KEYWORD_LOG_FAIL    "[FAIL]"
#define KEYWORD_LOG_PASS    "[PASS]"
#define KEYWORD_LOG_ERROR   "[ERROR]"
#define KEYWORD_LOG_WARNING "[WARNING]"
#define KEYWORD_LOG_STATUS  "[STATUS]"

//Class for syntax highlighter
class CHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    CHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp qRegExpPattern;
        QTextCharFormat qTextCharFormat;
    };
    QVector<HighlightingRule> qVectorHighlightingRules;

    QTextCharFormat qTextCharFormatPrefixFailFormat;
    QTextCharFormat qTextCharFormatPrefixPassFormat;
    QTextCharFormat qTextCharFormatPrefixErrorFormat;
    QTextCharFormat qTextCharFormatPrefixWarningFormat;
    QTextCharFormat qTextCharFormatPrefixStatusFormat;

};

class CWidgetLogBox : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetLogBox(QWidget *parent = 0);

private:

    //Text edit
    QTextEdit* qTextEditLog;

    //Button to clear displayed text
    QPushButton* qPushButtonClearLog;

    //Button to save displayed text
    QPushButton* qPushButtonSaveLog;

signals:

public slots:
    //Public slot to add new log line
    void PublicSlotNewLog(QString qStringLog);

    //Public slot to update last log line
    void PublicSlotUpdateLog(QString qStringLog);

    //Public slot to save log
    void PublicSlotSaveLog(void);

    //Public slot to clear log
    void PublicSlotClearLog(void);
};

#endif // CWIDGETLOGBOX_H
