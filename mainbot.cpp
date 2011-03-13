#include "mainbot.h"

void MBot::processData1()
{
    while (globalSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress addr;
        qint8 stat, type;
        quint16 pport;
        datagram.resize(globalSocket->pendingDatagramSize());
        globalSocket->readDatagram(datagram.data(), datagram.size(),&addr,&pport);

        stat = datagram[0];
        type = datagram[1];
        datagram.remove(0,2);
        if (stat == 1)
            UserCheck(tr(datagram.data()),addr.toString());
        if (stat == 4)
            UserDel(tr(datagram.data()),addr.toString());
        if (stat == 0)
        {
          if (tr(datagram.data())=="!cs")
          {
              WriteLog("cs");
              cs->CSping();
          } else
          if (tr(datagram.data())=="!dota")
          {
              WriteLog("dota");
              war->WarPing();
          } else
          if (tr(datagram.data())=="!q3")
          {
              WriteLog("q3");
              q3->Q3ping();
          } else
          if ((tr(datagram.data())=="!start")||(tr(datagram.data())=="!старт"))
          {
              WriteLog("start");
              quiz->Start();
          } else
          if ((tr(datagram.data())=="!stop")||(tr(datagram.data())=="!стоп"))
          {
              WriteLog("stop");
              quiz->Stop();
          } else
          if ((tr(datagram.data())=="!hint")||(tr(datagram.data())=="!хинт")||(tr(datagram.data())=="!h")||(tr(datagram.data())=="!х"))
          {
              WriteLog("hint");
              quiz->GetHint();
          } else
          if ((tr(datagram.data())=="!next")||(tr(datagram.data())=="!дальше")||(tr(datagram.data())=="!n")||(tr(datagram.data())=="!д"))
          {
              WriteLog("next");
              if (quiz->ServerRun())
                  quiz->Next();
          } else
          if ((tr(datagram.data())=="!q")||(tr(datagram.data())=="!в"))
          {
              WriteLog("quest");
              if (quiz->ServerRun())
                  quiz->Repeat();
          } else
          if ((tr(datagram.data())=="!top")||(tr(datagram.data())=="!топ"))
          {
              WriteLog("top");
              quiz->Top();
          } else
          if ((tr(datagram.data())=="!ChatServBotKillYouSelf"))
          {
              WriteLog("ChatServBotKillYouSelf "+QDateTime::currentDateTime().toString("dd:MM:yy - hh:mm:ss"));
              sendMessage("die",bGeneral);
              exit(0);
          } else
          if ((tr(datagram.data())=="!help")||(tr(datagram.data())=="!помощь"))
          {
              WriteLog("help");
              sendMessage("Возможные команды:<br>"
                          "!cs - список серверов Counter-Strike<br>"
                          "!dota - список серверов Warcraft III<br>"
                          "!q3 - список серверов Quake 3<br><br>"
                          "Команды викторины:<br>"
                          "!q, !в - повторить вопрос<br>"
                          "!start, !старт - начать игру<br>"
                          "!stop, !стоп - остановить игру<br>"
                          "!next, !дальше, !n, !д - следующий вопрос<br>"
                          "!hint, !хинт, !h, !х - подсказка<br>"
                          "!top, !топ - топ викторины<br>"
                          "!help, !помощь - помощь",bGeneral);
          }
          else
          {
              if (quiz->ServerRun())
                  quiz->Answer(QString(datagram.data()),addr.toString());

          }
        }
    }
}
void MBot::WriteLog(QString msg)
{
    QTextStream out(botLog);
    out << msg<<endl;
}

MBot::MBot(QObject *parent)
{
    globalSocket = new QUdpSocket(this);
    globalSocket->bind(port,QUdpSocket::ReuseAddressHint);

    connect(globalSocket,SIGNAL(readyRead()),this,SLOT(processData1()));

    botLog = new QFile("bot.log");
    botLog->open(QIODevice::Append | QIODevice::Text);

    cs = new CSCheck;
    war = new WarCheck;
    quiz = new QuizCheck;
    q3 = new Q3Check;

    sendMessage("run",bGeneral);

    WriteLog("run");
}

MBot::~MBot()
{
    sendMessage("die",bGeneral);
    delete cs;
    delete war;
    delete quiz;
    delete q3;
    botLog->close();
    delete botLog;
    delete globalSocket;
}
