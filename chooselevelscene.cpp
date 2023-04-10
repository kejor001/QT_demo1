#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QLabel>
#include <QDebug>
#include <QSound>
ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
        //配置关卡基础信息
        this->setFixedSize(320,588);
        this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
        this->setWindowTitle("选择关卡场景");
        //菜单栏创建
        QMenuBar * bar= menuBar();
        setMenuBar(bar);
        //创建开始菜单
        QMenu *startMenu= bar->addMenu("开始");
       QAction * quitAction  = startMenu->addAction("退出");
       connect(quitAction,&QAction::triggered,[=](){
           this->close();
       });
       QSound * chooseSound= new QSound(":/res/TapButtonSound.wav",this);
       QSound * backSound=new QSound(":/res/BackButtonSound.wav",this);
       MyPushButton * backBtn= new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
       backBtn->setParent(this);
       backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());
       connect(backBtn,&QPushButton::clicked,[=](){
           backSound->play();
           emit this->chooseSceneBack();
       });
       //创建选择关卡按钮
       for(int i=0;i<20;i++)
       {
           MyPushButton * menuBtn=new MyPushButton(":/res/LevelIcon.png");
           menuBtn->setParent(this);
           menuBtn->move(25+i%4*70,130+i/4*70);
           connect(menuBtn,&MyPushButton::clicked,[=](){
               chooseSound->play();
               this->hide();
               play = new PlayScene(i+1);
               play->show();
               connect(play,&PlayScene::chooseSceneBack,[=](){
                   this->show();
                   delete play;
                   play=NULL;
               });
           });
           QLabel * label= new QLabel;
           label->setParent(this);
           label->setFixedSize(menuBtn->width(),menuBtn->height());
           label->setText(QString::number(i+1));
           label->move(25+i%4*70,130+i/4*70);
           //设置label的对齐方式
           label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
           label->setAttribute(Qt::WA_TransparentForMouseEvents);
       }

}

void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width()-pix.width())*0.5,30,pix.width(),pix.height(),pix);
}
