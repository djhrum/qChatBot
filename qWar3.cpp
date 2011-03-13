#include "qWar3.h"


QUdpSocket* WarSocket;

void WarCheck::WarReply ()
{
    while (WarSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress addr;
        quint16 pport;
        QString Server,ServerName,Map,MapPath,GameType,Game,Players,MaxPlayers,MaxSlots,Bots;
        char k;


        datagram.resize(WarSocket->pendingDatagramSize());
        WarSocket->readDatagram(datagram.data(), datagram.size(),&addr,&pport);


        FindServerTimer->stop();

        Server = datagram.mid( datagram.indexOf("(")+1 , datagram.indexOf(")") - datagram.indexOf("(") - 1 );

        k = 51;
        unsigned char m,p=0,h;
        m = datagram.at(k);
        for (quint8 j=0,i=50; datagram[i] != 0x01; i++,j++)   //расшифровывает полный путь до карты.
        {                                                     //который групируется по 8 байт, 2-й из которых битовая маска
            if (j == 8)                                       //если бит = 1, то соответствующий байт оставить без изменений
            {                                                 //если = 0, то отнять 1, Оставшийся бит отвечает за 1-й байт следующей группы
                j = 0;
                k += 8;
                if (m == 0) p=1;
                m = datagram.at(k);
            }
            if (j != 1)
            {
               if (m & 1)
                    MapPath.append(datagram.at(i)-p);
                else
                    MapPath.append(datagram.at(i)-1-p);
                m = m >> 1;
                if ((datagram.at(i)-p == '\\') || (datagram.at(i)-p == ']'))
                    h=MapPath.size();
                p = 0;
            }

        }

        Map = MapPath.mid(h, MapPath.size()-h);


        Players = QString::number(datagram[datagram.length()-14]);
        MaxPlayers = QString::number(datagram[datagram.length()-22]);
        MaxSlots = QString::number(datagram[datagram.length()-10]);
        Bots = QString::number(MaxPlayers.toInt()-MaxSlots.toInt());

        sendMessage(Server+" "+Map+" "+Players+"/"+MaxSlots+"/"+Bots+"<br>"+
                    "Server: "+Server+"<br>Map: "+Map+
                    "<br>MapPath: "+MapPath+"<br>Players: "+Players+
                    "<br>Bots: "+Bots+"<br>MaxSlot: "+MaxSlots+
                    "<br>MaxPlayers: "+MaxPlayers,bWar);
    }

}

void WarCheck::WarPing()  //стандартный запрос на проверку наличия сервера
{

    QByteArray dg;

    dg.append(0xf7);
    dg.append(0x2f);
    dg.append(0x10);
    dg.append(char(0));
    dg.append("PX3W");
    dg.append(0x18);
    dg.append(char(0));
    dg.append(char(0));
    dg.append(char(0));
    dg.append(char(0));
    dg.append(char(0));
    dg.append(char(0));
    dg.append(char(0));
    WarSocket->writeDatagram(dg,WarMask,WarPort);
    FindServerTimer->start(5000);

}

void WarCheck::NoServer()
{
    sendMessage("Servers not found",bWar);
    FindServerTimer->stop();
}

WarCheck::WarCheck (QObject * parent): QObject(parent)
{
    WarPort=6112;
    WarMask=QObject::tr("255.255.255.255");
    WarSocket = new QUdpSocket();
    WarSocket->bind(/*WarPort*/49678,QUdpSocket::ReuseAddressHint);  //забинден на нестандартный, что бы не принимал не свои ответы
    FindServerTimer = new QTimer(this);

    QObject::connect(WarSocket,SIGNAL(readyRead()),this,SLOT(WarReply()));
    QObject::connect(FindServerTimer,SIGNAL(timeout()),this,SLOT(NoServer()));
}

WarCheck::~WarCheck ()
{
    delete WarSocket;
    delete FindServerTimer;

}

