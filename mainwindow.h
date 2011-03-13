#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include "qNet.h"
#include "qCS.h"
#include "qwar3.h"
#include "qQuiz.h"
#include "qQuake3.h"
#include <QtGui/QMainWindow>
#include <QTextBrowser>
//#include <QFile>


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QTextBrowser* chatArea;
    CSCheck* cs;
    WarCheck* war;
    QuizCheck* quiz;
    Q3Check* q3;
//    QFile* botLog;
public:
    MainWindow(QWidget *parent = 0);
   // void WriteLog(QString msg);
    ~MainWindow();

private slots:

    void processData();
};

#endif // MAINWINDOW_H
