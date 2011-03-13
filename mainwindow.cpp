#include "mainwindow.h"

void MainWindow::processData()
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
              chatArea->insertHtml("cs<br>");
              cs->CSping();
          } else
          if (tr(datagram.data())=="!dota")
          {
              chatArea->insertHtml("dota<br>");
              war->WarPing();
          } else
          if (tr(datagram.data())=="!q3")
          {
              chatArea->insertHtml("q3<br>");
              q3->Q3ping();
          } else
          if ((tr(datagram.data())=="!start")||(tr(datagram.data())=="!старт"))
          {
              chatArea->insertHtml("start<br>");
              quiz->Start();
          } else
          if ((tr(datagram.data())=="!stop")||(tr(datagram.data())=="!стоп"))
          {
              chatArea->insertHtml("stop<br>");
              quiz->Stop();
          } else
          if ((tr(datagram.data())=="!hint")||(tr(datagram.data())=="!хинт")||(tr(datagram.data())=="!h")||(tr(datagram.data())=="!х"))
          {
              chatArea->insertHtml("hint<br>");
              quiz->GetHint();
          } else
          if ((tr(datagram.data())=="!next")||(tr(datagram.data())=="!дальше")||(tr(datagram.data())=="!n")||(tr(datagram.data())=="!д"))
          {
              chatArea->insertHtml("next<br>");
              if (quiz->ServerRun())
                  quiz->Next();
          } else
          if ((tr(datagram.data())=="!q")||(tr(datagram.data())=="!в"))
          {
              chatArea->insertHtml("quest<br>");
              if (quiz->ServerRun())
                  quiz->Repeat();
          } else
          if ((tr(datagram.data())=="!top")||(tr(datagram.data())=="!топ"))
          {
              chatArea->insertHtml("top<br>");
              quiz->Top();
          } else
          if ((tr(datagram.data())=="!help")||(tr(datagram.data())=="!помощь"))
          {
              chatArea->insertHtml("help<br>");
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
                          "!top, !топ - топ викторины"
                          "!help, !помощь - помощь",bGeneral);
          }
          else
          {
              if (quiz->ServerRun())
                  quiz->Answer(tr(datagram.data()),addr.toString());

          }

        }

    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    chatArea = new QTextBrowser(this);
    chatArea->setOpenLinks(false);
    setCentralWidget(chatArea);

    globalSocket = new QUdpSocket(this);
    globalSocket->bind(port,QUdpSocket::ReuseAddressHint);

    connect(globalSocket,SIGNAL(readyRead()),this,SLOT(processData()));

    /*botLog = new QFile("bot.log");
    botLog->open(QIODevice::Append | QIODevice::Text);
*/
    cs = new CSCheck;
    war = new WarCheck;
    quiz = new QuizCheck;
    q3 = new Q3Check;
    //cs->CSping();

    sendMessage("run",bGeneral);
    //WriteLog("run");

    chatArea->insertHtml("run<br>");

}


MainWindow::~MainWindow()
{
    sendMessage("die",bGeneral);
}

