#include "cwidgetlogbox.h"

CWidgetLogBox::CWidgetLogBox(QWidget *parent) : QWidget(parent)
{
    QGridLayout *qGridLayoutMain = new QGridLayout();


    //qGridLayoutMain->setSizeConstraint(QLayout::SetFixedSize);

    //(0,0) Log Box
    qTextEditLog = new QTextEdit(this);
    qTextEditLog->setReadOnly(true);

    qGridLayoutMain->addWidget(qTextEditLog, 0, 0, 1, 2);

    //(8,0) Button Clear log
    qPushButtonClearLog = new QPushButton(this);
    qPushButtonClearLog->setText("Clear Log");
    qGridLayoutMain->addWidget(qPushButtonClearLog, 1, 0, 2, 1);

    //(8,2) Button Save log
    qPushButtonSaveLog = new QPushButton(this);
    qPushButtonSaveLog->setText("Save Log");
    qGridLayoutMain->addWidget(qPushButtonSaveLog, 1, 1, 2, 1);

    //Finish layout setting
    setLayout(qGridLayoutMain);

    //Apply High lighter
    CHighlighter *mHighlighter = new CHighlighter(qTextEditLog->document());


    qTextEditLog->append("[FAIL]:Test");
    qTextEditLog->append("[PASS]:Test");
    qTextEditLog->append("[ERROR]:Test");
    qTextEditLog->append("[WARNING]:Test");
    qTextEditLog->append("[STATUS]:Test");

    //Connect signal

    connect(qPushButtonClearLog, SIGNAL(clicked()), this, SLOT(PublicSlotClearLog()));
    connect(qPushButtonSaveLog, SIGNAL(clicked()), this, SLOT(PublicSlotSaveLog()));
}

void CWidgetLogBox::PublicSlotNewLog(QString qStringLog)
{   
    //Add timestamp
    qStringLog = "[" + (QTime::currentTime().toString()) + "] " + qStringLog;

    //Add to log box
    qTextEditLog->append(qStringLog);
}

void CWidgetLogBox::PublicSlotUpdateLog(QString qStringLog)
{
    QTextCursor* qTextCursorLog = new QTextCursor(qTextEditLog->document());

    //Move to the end of the document
    qTextCursorLog->movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    qTextCursorLog->movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor, 1);
    //Change Text
    qTextCursorLog->insertText(qStringLog);
}

void CWidgetLogBox::PublicSlotSaveLog(void)
{
    QString qStringLog = qTextEditLog->toPlainText();

    QString qStringFilename =  QFileDialog::getSaveFileName(
              this,
              "Open Document",
              QDir::currentPath(),
              "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");

    QFile QFileSaved(qStringFilename);
    if (!QFileSaved.open(QIODevice::WriteOnly | QIODevice::Text))
        return;


    QTextStream out(&QFileSaved);
        out << qStringLog;
}

void CWidgetLogBox::PublicSlotClearLog()
{
    qTextEditLog->clear();
}


CHighlighter::CHighlighter(QTextDocument *parent)
 : QSyntaxHighlighter(parent)
{
    HighlightingRule mRule;

    //Fail syntax
    qTextCharFormatPrefixFailFormat.setForeground(Qt::red);
    qTextCharFormatPrefixFailFormat.setFontWeight(QFont::Bold);

    mRule.qRegExpPattern = QRegExp("\\bFAIL\\b");
    mRule.qTextCharFormat = qTextCharFormatPrefixFailFormat;

    qVectorHighlightingRules.append(mRule);


    //Pass Syntax
    qTextCharFormatPrefixPassFormat.setForeground(Qt::green);
    qTextCharFormatPrefixPassFormat.setFontWeight(QFont::Bold);

    mRule.qRegExpPattern = QRegExp("\\bPASS\\b");
    mRule.qTextCharFormat = qTextCharFormatPrefixPassFormat;

    qVectorHighlightingRules.append(mRule);

    //Error Syntax
    qTextCharFormatPrefixErrorFormat.setForeground(Qt::darkMagenta);
    qTextCharFormatPrefixErrorFormat.setFontWeight(QFont::Bold);

    mRule.qRegExpPattern = QRegExp("\\bERROR\\b");
    mRule.qTextCharFormat = qTextCharFormatPrefixErrorFormat;

    qVectorHighlightingRules.append(mRule);

    //Warning Syntax
    qTextCharFormatPrefixWarningFormat.setForeground(Qt::darkYellow);
    qTextCharFormatPrefixWarningFormat.setFontWeight(QFont::Bold);

    mRule.qRegExpPattern = QRegExp("\\bWARNING\\b");
    mRule.qTextCharFormat = qTextCharFormatPrefixWarningFormat;

    qVectorHighlightingRules.append(mRule);

    //Status Syntax
    qTextCharFormatPrefixStatusFormat.setForeground(Qt::darkBlue);
    qTextCharFormatPrefixStatusFormat.setFontWeight(QFont::Bold);

    mRule.qRegExpPattern = QRegExp("\\bSTATUS\\b");
    mRule.qTextCharFormat = qTextCharFormatPrefixStatusFormat;

    qVectorHighlightingRules.append(mRule);

}

void CHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, qVectorHighlightingRules) {
        QRegExp expression(rule.qRegExpPattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.qTextCharFormat);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
}
