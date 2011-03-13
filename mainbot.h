#ifndef MAINBOT_H
#define MAINBOT_H

#include <QObject>
#include "qNet.h"
#include "qCS.h"
#include "qwar3.h"
#include "qQuiz.h"
#include "qQuake3.h"
#include <QFile>

class MBot : public QObject
{
    Q_OBJECT

private:
    CSCheck* cs;
    WarCheck* war;
    QuizCheck* quiz;
    Q3Check* q3;
    QFile* botLog;
public:
    MBot(QObject *parent = 0);
    void WriteLog(QString msg);
    ~MBot();

private slots:

    void processData1();

};
#endif // MAINBOT_H
