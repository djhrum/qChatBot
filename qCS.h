#ifndef QCS_H
#define QCS_H

#include <QtCore>
#include "qNet.h"

extern QUdpSocket* CSSocket;

class CSCheck : public QObject
{
    Q_OBJECT
private:
    quint16 CSport;
    QHostAddress CSmask;
    QTimer* FindServerTimer;

public:
    void CSping();
    CSCheck (QObject * parent = 0);
    ~CSCheck ();



private slots:
    void NoServer();
    void CSReply();

};


#endif // QCS_H
