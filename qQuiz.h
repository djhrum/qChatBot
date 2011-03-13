#ifndef QQUIZ_H
#define QQUIZ_H

#include <QtCore>
#include <QFile>
#include <qthread.h>
#include <QSettings>
#include "qNet.h"

class QuizCheck : public QObject
{
    Q_OBJECT
private:
    QTimer* QuizTimer;
    QFile BaseName;
    QString* Questions;
    quint32 count;
    bool st;   //идентефикатор работы викторины
    QString CurrQuestion, CurrAsk, MaskAsk;
    int HintCount;
    //QElapsedTimer ;
    QSettings* UserStats;

    QString TopNick[10];  //топ викторины
    quint8 TopCount[10];

    void GetQuestion();


public:
    bool ServerRun();
    void Start();
    void Stop();
    void GetHint();
    void Next();
    void Repeat();
    void Top();
    void Answer(QString msg,QString ip);
    QuizCheck (QObject * parent = 0);
    ~QuizCheck ();

private slots:
    void AutoHint();
};

#endif // QQUIZ_H
