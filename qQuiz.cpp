#include "qQuiz.h"

bool QuizCheck::ServerRun()
{
    return st;
}

void QuizCheck::Next()
{
    QuizTimer->stop();
    sendMessage("Ответ: "+CurrAsk,bQuiz);
    QEventLoop loop; QTimer::singleShot(1000, &loop, SLOT(quit())); loop.exec();
    GetQuestion();
}

void QuizCheck::Answer(QString msg, QString ip)
{
    if (msg==CurrAsk)
    {
        QuizTimer->stop();
        QString nick = 0;
        quint16 idt = ChatIp.indexOf(ip);

        if (idt != -1)
            nick = ChatNick.at(idt);

        if (AskCount.at(idt) == 0)
            AskCount[idt] = UserStats->value("all_stat/"+nick).toInt(); // загрузка значения из файла

        AskCount[idt] += 1;


        sendMessage("верно,  "+nick,bQuiz);
        sendMessage("Всего правильных ответов "+QString::number(AskCount.at(idt)),bQuiz);
        UserStats->setValue("all_stat/"+nick,AskCount.at(idt));

        for (quint8 i=0; i<10;++i)
        {
            if (AskCount[idt] > TopCount[i])
            {
                if (TopNick[i] != nick)
                {
                    if (i < 9)              //если уже в топе, то поместить на уровень выше, если нет, добавить на последнее место
                    {
                        TopNick[i+1] = TopNick[i];
                        TopCount[i+1] = TopCount[i];
                        UserStats->setValue("top/pos"+QString::number(i+1)+"n",TopNick[i+1]);
                        UserStats->setValue("top/pos"+QString::number(i+1)+"c",TopCount[i+1]);
                    }

                    TopNick[i] = nick;
                    UserStats->setValue("top/pos"+QString::number(i)+"n",TopNick[i]);
                    sendMessage("Вы заняли "+QString::number(i+1)+"-е место",bQuiz);
                }
                TopCount[i] = AskCount[idt];
                UserStats->setValue("top/pos"+QString::number(i)+"c",TopCount[i]);
                break;
            }
        }


        QEventLoop loop; QTimer::singleShot(1000, &loop, SLOT(quit())); loop.exec(); //подождать немножко, до выдачи следующего вопроса

        GetQuestion();
    }

}

void QuizCheck::Top()
{

    sendMessage("Топ викторины:",bQuiz);

    for (quint8 i=0;i != 10;++i)
    {
        sendMessage(TopNick[i]+" - "+(TopCount[i]+48),bQuiz);
    }
}

void QuizCheck::GetHint()
{
    if (!st)
        return;

    AutoHint();
    QuizTimer->start(5000);
}

void QuizCheck::Repeat()
{
    sendMessage(CurrQuestion+"  "+QString::number(CurrAsk.length())+" букв",bQuiz);
}

void QuizCheck::AutoHint()
{
    if (HintCount+2 == CurrAsk.length())
    {
        sendMessage("Никто не ответил на вопрос",bQuiz);
        sendMessage("Ответ: "+CurrAsk,bQuiz);
        QuizTimer->stop();
        GetQuestion();
        return;
    }

    int k=0;

    do
    {
        k=qrand()%(CurrAsk.length()-1);
    }
    while (MaskAsk[k] == CurrAsk[k]);

    MaskAsk[k]=CurrAsk[k];

    HintCount++;

    sendMessage("Подсказка: "+MaskAsk,bQuiz);

}

void QuizCheck::GetQuestion()
{
    quint32 k;

    bool m=false;
    char pfix=' ';

    HintCount = 0;

    CurrAsk.clear();
    CurrQuestion.clear();
    MaskAsk.clear();

    k=qrand()%count;
    for (int i=0;i<Questions[k].length();i++)
    {

        if (Questions[k].at(i)=='*')
        {
            m=true;
            continue;
        }
        if (m)
        {
            CurrAsk.append(Questions[k].at(i));
            if ((Questions[k].at(i)==' ')||(Questions[k].at(i)=='-'))
                MaskAsk.append(Questions[k].at(i));
            else
                MaskAsk.append("*");
        }
        else
            CurrQuestion.append(Questions[k].at(i));
    }

    CurrAsk.remove(CurrAsk.length()-1,1);
    MaskAsk.remove(MaskAsk.length()-1,1);
    if (CurrAsk.length()==1) pfix='a'; else                                 //всунуть правильное окончание слова "буква"
    if ((CurrAsk.length()==2)||(CurrAsk.length()==3)||(CurrAsk.length()==4)) pfix='ы'; else
    if ((CurrAsk.length()>20)&&(CurrAsk.length()%10==1)) pfix='a'; else
    if ((CurrAsk.length()>20)&&((CurrAsk.length()%10==2)||(CurrAsk.length()%10==3)||(CurrAsk.length()%10==4))) pfix='ы';
    sendMessage(CurrQuestion+"  "+QString::number(CurrAsk.length())+" букв"+QString(pfix),bQuiz);
    QuizTimer->start(5000);
}

void QuizCheck::Stop()
{
    if (st)
    {
        QuizTimer->stop();
        st = false;
        sendMessage("Игра остановлена",bQuiz);
        sendMessage("Правильный ответ: "+CurrAsk,bQuiz);
    }
}

void QuizCheck::Start ()
{
    sendMessage("В базе "+QString::number(count)+" вопросов.",bQuiz);
    sendMessage("Начинаем",bQuiz);

    //QuizTimer->start(5000);
    GetQuestion();
    st=1;

}


QuizCheck::QuizCheck (QObject * parent): QObject(parent)
{
    BaseName.setFileName("questions.txt");

    BaseName.open(QIODevice::ReadOnly|QIODevice::Text);


    count=0;

    while (BaseName.pos()<BaseName.size())  //посчитать кол-во строк
    {
        BaseName.readLine(500);
        count++;
    }


    BaseName.seek(0);
    Questions = new QString [count];   //создать нужный массив

    for (quint32 i=0;BaseName.pos()<BaseName.size();i++)  //заполнить его
        Questions[i] = QString(BaseName.readLine(500));

    BaseName.close();

    sendMessage("Загружено "+QString::number(count)+" вопросов.",bGeneral);

    st=0;
    qsrand(QTime::currentTime().msecsTo(QTime()));

    QuizTimer = new QTimer(this);

    QObject::connect(QuizTimer,SIGNAL(timeout()),this,SLOT(AutoHint()));

    UserStats = new QSettings("userstats.ini",QSettings::IniFormat);

    for (quint8 i=0; i<10;++i)  //заполнить топ
    {
        TopNick[i]=UserStats->value("top/pos"+QString::number(i)+"n").toString();
        TopCount[i]=UserStats->value("top/pos"+QString::number(i)+"c").toInt();
    }


}

QuizCheck::~QuizCheck()
{
    delete QuizTimer;
    delete UserStats;
    delete[] Questions;

}
