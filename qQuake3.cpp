#include "qQuake3.h"


QUdpSocket* Q3Socket;

void Q3Check::Q3Reply ()
{
    while (Q3Socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress addr;
        quint16 pport;
        QString ServerName,Map,GameType,Players,MaxPleyers;
        quint8 i;


        datagram.resize(Q3Socket->pendingDatagramSize());
        Q3Socket->readDatagram(datagram.data(), datagram.size(),&addr,&pport);      

        datagram.remove(0,4);
        if (datagram.left(datagram.length())=="getinfo xxx")
            continue;

        FindServerTimer->stop();

        if (datagram[43]=='0')
            GameType="Free for all";
        if (datagram[43]=='3')
            GameType="Team deathmatch";
        if (datagram[43]=='1')
            GameType="Tournament";
        if (datagram[43]=='4')
            GameType="Capture the flag";

        MaxPleyers = datagram.at(59);
        Players = datagram.at(69);
        Map = datagram.mid(79,i=datagram.indexOf("\\",79)-79);
        ServerName = datagram.mid(89+i,datagram.indexOf("\\",89+i)-i-89);

        sendMessage(GameType+" "+Players+"/"+MaxPleyers+"<br>"+
                    "ServerName: "+ServerName+"<br>ip:"+addr.toString()+
                    "<br>Map: "+Map+"<br>Game type:"+GameType+
                    "<br>Players: "+Players+"/"+MaxPleyers,bQ3);

/*free for all
0000   69 6e 66 6f 52 65 73 70 6f 6e 73 65 0a 5c 70 75  infoResponse.\pu
0010   6e 6b 62 75 73 74 65 72 5c 30 5c 70 75 72 65 5c  nkbuster\0\pure\
0020   31 5c 67 61 6d 65 74 79 70 65 5c 30 5c 73 76 5f  1\gametype\0\sv_
0030   6d 61 78 63 6c 69 65 6e 74 73 5c 38 5c 63 6c 69  maxclients\8\cli
0040   65 6e 74 73 5c 31 5c 6d 61 70 6e 61 6d 65 5c 51  ents\1\mapname\Q
0050   33 44 4d 31 5c 68 6f 73 74 6e 61 6d 65 5c 6e 6f  3DM1\hostname\no
0060   6e 61 6d 65 5c 70 72 6f 74 6f 63 6f 6c 5c 36 38  name\protocol\68
0070   5c 63 68 61 6c 6c 65 6e 67 65 5c 78 78 78 00     \challenge\xxx.
team deathmatch
0000   69 6e 66 6f 52 65 73 70 6f 6e 73 65 0a 5c 70 75  infoResponse.\pu
0010   6e 6b 62 75 73 74 65 72 5c 30 5c 70 75 72 65 5c  nkbuster\0\pure\
0020   31 5c 67 61 6d 65 74 79 70 65 5c 33 5c 73 76 5f  1\gametype\3\sv_
0030   6d 61 78 63 6c 69 65 6e 74 73 5c 38 5c 63 6c 69  maxclients\8\cli
0040   65 6e 74 73 5c 31 5c 6d 61 70 6e 61 6d 65 5c 51  ents\1\mapname\Q
0050   33 44 4d 36 5c 68 6f 73 74 6e 61 6d 65 5c 6e 6f  3DM6\hostname\no
0060   6e 61 6d 65 5c 70 72 6f 74 6f 63 6f 6c 5c 36 38  name\protocol\68
0070   5c 63 68 61 6c 6c 65 6e 67 65 5c 78 78 78 00     \challenge\xxx.
tournament
0000   69 6e 66 6f 52 65 73 70 6f 6e 73 65 0a 5c 70 75  infoResponse.\pu
0010   6e 6b 62 75 73 74 65 72 5c 30 5c 70 75 72 65 5c  nkbuster\0\pure\
0020   31 5c 67 61 6d 65 74 79 70 65 5c 31 5c 73 76 5f  1\gametype\1\sv_
0030   6d 61 78 63 6c 69 65 6e 74 73 5c 38 5c 63 6c 69  maxclients\8\cli
0040   65 6e 74 73 5c 31 5c 6d 61 70 6e 61 6d 65 5c 51  ents\1\mapname\Q
0050   33 44 4d 31 5c 68 6f 73 74 6e 61 6d 65 5c 6e 72  3DM1\hostname\nr
0060   75 6d 5c 70 72 6f 74 6f 63 6f 6c 5c 36 38 5c 63  um\protocol\68\c
0070   68 61 6c 6c 65 6e 67 65 5c 78 78 78 78           hallenge\xxxx
capture the flag
0000   69 6e 66 6f 52 65 73 70 6f 6e 73 65 0a 5c 70 75  infoResponse.\pu
0010   6e 6b 62 75 73 74 65 72 5c 30 5c 70 75 72 65 5c  nkbuster\0\pure\
0020   31 5c 67 61 6d 65 74 79 70 65 5c 34 5c 73 76 5f  1\gametype\4\sv_
0030   6d 61 78 63 6c 69 65 6e 74 73 5c 36 5c 63 6c 69  maxclients\6\cli
0040   65 6e 74 73 5c 31 5c 6d 61 70 6e 61 6d 65 5c 51  ents\1\mapname\Q
0050   33 43 54 46 31 5c 68 6f 73 74 6e 61 6d 65 5c 6e  3CTF1\hostname\n
0060   72 75 6d 5c 70 72 6f 74 6f 63 6f 6c 5c 36 38 5c  rum\protocol\68\
0070   63 68 61 6c 6c 65 6e 67 65 5c 78 78 78 78        challenge\xxxx
*/
    }

}

void Q3Check::Q3ping()
{

    QByteArray dg;

    dg.append(0xff);
    dg.append(0xff);
    dg.append(0xff);
    dg.append(0xff);
    dg.append("getinfo xxx");
    Q3Socket->writeDatagram(dg,Q3mask,Q3port);
    FindServerTimer->start(5000);
}
void Q3Check::NoServer()
{
    sendMessage("Servers not found",bQ3);
    FindServerTimer->stop();
}

Q3Check::Q3Check (QObject * parent): QObject(parent)
{
    Q3port=27960;
    Q3mask=QObject::tr("255.255.255.255");
    Q3Socket = new QUdpSocket();
    Q3Socket->bind(/*Q3port*/49677,QUdpSocket::ReuseAddressHint);

    FindServerTimer = new QTimer(this);


    QObject::connect(FindServerTimer,SIGNAL(timeout()),this,SLOT(NoServer()));
    QObject::connect(Q3Socket,SIGNAL(readyRead()),this,SLOT(Q3Reply()));
}

Q3Check::~Q3Check ()
{
    delete Q3Socket;
    delete FindServerTimer;

}

