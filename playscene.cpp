#include "playscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include "mycoin.h"
#include <QDebug>
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QSound>
//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
//{

//}

PlayScene::PlayScene(int levelNum)
{
    this->levelIndex=levelNum;
    //初始化游戏场景
    this->setFixedSize(320,588);
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    this->setWindowTitle("翻金币场景");
    //菜单栏创建
    QMenuBar * bar= menuBar();
    setMenuBar(bar);
    //创建开始菜单
    QMenu *startMenu= bar->addMenu("开始");
    QAction * quitAction  = startMenu->addAction("退出");
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });
    //添加音效资源
    QSound * backSound= new QSound(":/res/BackButtonSound.wav",this);
    QSound * flip =new QSound(":/res/ConFlipSound.wav",this);
    QSound * winSound= new QSound(":/res/LevelWinSound.wav",this);
    //返回按钮
    MyPushButton * backBtn= new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());
    connect(backBtn,&QPushButton::clicked,[=](){
        backSound->play();
        QTimer::singleShot(500,this,[=](){
            emit this->chooseSceneBack();
        });
    });
    //显示当前第几关
    QLabel * label = new QLabel;
    label -> setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str1 =QString("Level: %1").arg(this->levelIndex);
    label->setFont(font);
    label->setText(str1);
    label ->setGeometry(30,this->height()-50,120,50);
    dataConfig config;
    //初始化每一个关卡
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            this->gameArray[i][j]=config.mData[this->levelIndex][i][j];
        }
    }
    //胜利的图片
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());
    //显示金币背景
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            QPixmap  pix = QPixmap(":/res/BoardNode.png");
            QLabel * label = new QLabel ;
            label->setGeometry(0,0,pix.width(),pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57+i*50,200+j*50);
            //创建金币
            QString str;
            if(this->gameArray[i][j]==1)
            {
                str=":/res/Coin0001.png";
            }
            else
            {
                str=":/res/Coin0008.png";
            }
            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59+i*50,204+j*50);
            //给金币的属性赋值
            coin->posX=i;
            coin->posY=j;
            coin->flag=this->gameArray[i][j];
            coinBtn[i][j]=coin;
            //点击金币进行翻转
            connect(coin,&MyCoin::clicked,[=](){
                flip->play();
                coin->changeFlag();
                this->gameArray[i][j]=this->gameArray[i][j]==0?1:0;
                QTimer::singleShot(300,this,[=](){
                    if(coin->posX+1<=3)
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY]=this->gameArray[coin->posX+1][coin->posY]==0?1:0;
                    }
                    if(coin->posX-1>=0)
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY]=this->gameArray[coin->posX-1][coin->posY]==0?1:0;
                    }
                    if(coin->posY+1<=3)
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1]=this->gameArray[coin->posX][coin->posY+1]==0?1:0;
                    }
                    if(coin->posY-1>0)
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1]=this->gameArray[coin->posX][coin->posY-1]==0?1:0;
                    }
                    //判断是否胜利
                    this->isWin=true;
                    for(int i=0;i<4;i++)
                    {
                        for(int j=0;j<4;j++)
                        {
                            if(coinBtn[i][j]->flag==false)
                            {
                                this->isWin=false;
                                break;
                            }
                        }
                    }
                    if(this->isWin==true)
                    {
                        //                        qDebug()<<"游戏胜利";
                        winSound->play();
                            for(int i=0;i<4;i++)
                        {
                            for(int j=0;j<4;j++)
                            {
                                coinBtn[i][j]->isWin=true;
                            }
                        }
                        //将胜利的图片移动下来
                        QPropertyAnimation * animation =new QPropertyAnimation(winLabel,"geometry");
                        animation->setDuration(1000);
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        animation->start();
                    }
                });

            });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix.load(":/res/Title.png");
    pix=pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);
}
