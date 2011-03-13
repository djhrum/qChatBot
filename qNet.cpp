#include "qNet.h"

quint16 port = 49675;
QUdpSocket* globalSocket;
QHostAddress broadcast(QObject::tr("172.18.255.255"));

QList<QString> ChatNick, ChatIp;
QList<quint16> AskCount;

void sendMessage(QString msg,BotType bt)
{


    QByteArray dg;

    dg.append(char(6));
    dg.append(bt);
    dg.append(msg.toAscii());

    //dg.insert(0,char(0));
    //dg.insert(1,char(0));
    globalSocket->writeDatagram(dg,broadcast,port);
}

void UserCheck(QString lnick, QString lip)
{
    if (ChatNick.indexOf(lnick) == -1)
    {
        ChatNick.append(lnick);
        ChatIp.append(lip);
        AskCount.append(0);
    }

}

void UserDel(QString mnick, QString mip)
{
    if (ChatNick.indexOf(mnick) == -1)
    {
        ChatNick.removeOne(mnick);
        ChatIp.removeOne(mip);
        AskCount.removeAt(ChatNick.indexOf(mnick));
    }
}
