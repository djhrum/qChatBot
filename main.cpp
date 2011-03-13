#include <QtGui/QApplication>
#include <QTextCodec>
//#include "mainwindow.h"
#include "mainbot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("cp1251"));
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("cp1251"));
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("cp1251"));
    //MainWindow w;
    MBot w;
    //w.show();

    return a.exec();
}
