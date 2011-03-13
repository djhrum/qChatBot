#include "qCS.h"


QUdpSocket* CSSocket;

void CSCheck::CSReply ()
{
    while (CSSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress addr;
        quint16 pport;
        QString Server,ServerName,Map,GameType,Game,Players,MaxPleyers;
        quint8 i,j;

        FindServerTimer->stop();
        datagram.resize(CSSocket->pendingDatagramSize());
        CSSocket->readDatagram(datagram.data(), datagram.size(),&addr,&pport);

        Server= datagram.mid(5,i=datagram.indexOf(char(0),5)-5);

        i +=6;
        ServerName = datagram.mid(i,j=datagram.indexOf(char(0),i)-i);
        i =i+j+1;
        Map = datagram.mid(i,j=datagram.indexOf(char(0),i)-i);
        i=i+j+1;
        GameType = datagram.mid(i,j=datagram.indexOf(char(0),i)-i);
        i=i+j+1;
        Game = datagram.mid(i,j=datagram.indexOf(char(0),i)-i);
        i=i+j+1;
        Players = QString::number(datagram[i]);
        MaxPleyers = QString::number(datagram[i+1]);

        sendMessage(Game+" "+ServerName+" "+Players+"/"+MaxPleyers+"<br>"+
                    "ip: "+Server+"/"+addr.toString()+"<br>Name: "+ServerName+
                    "<br>Map: "+Map+"<br>Game type:"+GameType+"<br>Game: "+Game+
                    "<br>Players: "+Players+"/"+MaxPleyers,bCS);

/*
0000   ff ff ff ff 6d 31 39 32 2e 31 36 38 2e 31 30 2e  ....m192.168.10.
0010   33 3a 32 37 30 31 35 00 44 69 6d 2d 4f 4e 00 64  3:27015.Dim-ON.d
0020   65 5f 63 62 62 6c 65 00 63 73 74 72 69 6b 65 00  e_cbble.cstrike.
0030   43 6f 75 6e 74 65 72 2d 53 74 72 69 6b 65 00 01  Counter-Strike..
0040   17 2f 64 77 00 01 77 77 77 2e 63 6f 75 6e 74 65  ./dw..www.counte
0050   72 2d 73 74 72 69 6b 65 2e 6e 65 74 00 00 00 01  r-strike.net....
0060   00 00 00 00 9e f7 0a 00 01 00 00                 ...........

0000   ff ff ff ff 6d 31 39 32 2e 31 36 38 2e 31 30 2e  ....m192.168.10.
0010   33 3a 32 37 30 31 35 00 44 69 6d 2d 4f 4e 00 64  3:27015.Dim-ON.d
0020   65 5f 63 62 62 6c 65 00 63 73 74 72 69 6b 65 00  e_cbble.cstrike.
0030   43 6f 75 6e 74 65 72 2d 53 74 72 69 6b 65 00 00  Counter-Strike..
0040   17 2f 64 77 00 01 77 77 77 2e 63 6f 75 6e 74 65  ./dw..www.counte
0050   72 2d 73 74 72 69 6b 65 2e 6e 65 74 00 00 00 01  r-strike.net....
0060   00 00 00 00 9e f7 0a 00 01 00 00                 ...........
*/
    }

}

void CSCheck::CSping()
{

    QByteArray dg;

    dg.append(0xff);
    dg.append(0xff);
    dg.append(0xff);
    dg.append(0xff);
    dg.append("TSource Engine Query");
    dg.append(char(0));
    CSSocket->writeDatagram(dg,CSmask,CSport);
    FindServerTimer->start(5000);
}
void CSCheck::NoServer()
{
    sendMessage("Servers not found",bCS);
    FindServerTimer->stop();
}

CSCheck::CSCheck (QObject * parent): QObject(parent)
{
    CSport=27016;
    CSmask=QObject::tr("255.255.255.255");
    CSSocket = new QUdpSocket();
    CSSocket->bind(/*CSport*/49676,QUdpSocket::ReuseAddressHint);

    FindServerTimer = new QTimer(this);


    QObject::connect(FindServerTimer,SIGNAL(timeout()),this,SLOT(NoServer()));
    QObject::connect(CSSocket,SIGNAL(readyRead()),this,SLOT(CSReply()));
}

CSCheck::~CSCheck ()
{
    delete CSSocket;
    delete FindServerTimer;
}
