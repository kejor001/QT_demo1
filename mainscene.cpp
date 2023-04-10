#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QSound>
mainscene::mainscene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainscene)
{
    ui->setupUi(this);
    //设置固定大小
    setFixedSize(320,588);
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    setWindowTitle("翻金币主场景");
    //退出按钮实现
    connect(ui->actionquix,&QAction::triggered,[=](){
        this->close();
    });
    //准备开始按钮的音效
    QSound * startSound = new QSound(":/res/TapButtonSound.wav",this);
    //开始按钮实现
    MyPushButton * startBtn= new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.7);
    chooseScene =new ChooseLevelScene;
    //监听选择关卡返回按钮的信号
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,this,[=](){
        chooseScene->close();
        this->show();
    });
    connect(startBtn,&MyPushButton::clicked,[=](){
        startSound->play();
        startBtn->zoom1();
        startBtn->zoom2();
        //进入选择关卡环节中
        QTimer::singleShot(500,this,[=](){
            this->hide();
            chooseScene->show();

        });
    });
}
void mainscene::paintEvent(QPaintEvent *)
{
    //创建画家类对象
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix.load(":/res/Title.png");
    pix= pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix);
}

mainscene::~mainscene()
{
    delete ui;
}

