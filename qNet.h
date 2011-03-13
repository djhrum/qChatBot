#ifndef QNET_H
#define QNET_H



#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QByteArray>
#include <QList>


extern QUdpSocket* globalSocket;
extern QHostAddress broadcast;
extern quint16 port;

extern QList<QString> ChatNick, ChatIp;
extern QList<quint16> AskCount;


enum BotType
{
    bGeneral = 5,
    bCS = 6,
    bWar = 7,
    bQ3 = 8,
    bQuiz = 9
};

void sendMessage(QString msg,BotType bt);
void UserCheck (QString lnick,QString lip);
void UserDel (QString lnick,QString lip);



#endif // QNET_H
