#ifndef QWAR3_H
#define QWAR3_H

#include <QtCore>
#include "qNet.h"

extern QUdpSocket* WarSocket;

class WarCheck : public QObject
{
    Q_OBJECT
private:
    quint16 WarPort;
    QHostAddress WarMask;
    QTimer* FindServerTimer;

public:
    void WarPing();
    WarCheck (QObject * parent = 0);
    ~WarCheck ();



private slots:
    void NoServer();
    void WarReply();

};

#endif // QWAR3_H
