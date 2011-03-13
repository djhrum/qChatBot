#ifndef QQUAKE3_H
#define QQUAKE3_H

#include <QtCore>
#include "qNet.h"

extern QUdpSocket* Q3Socket;

class Q3Check : public QObject
{
    Q_OBJECT
private:
    quint16 Q3port;
    QHostAddress Q3mask;
    QTimer* FindServerTimer;

public:
    void Q3ping();
    Q3Check (QObject * parent = 0);
    ~Q3Check ();



private slots:
    void NoServer();
    void Q3Reply();

};

#endif // QQUAKE3_H
