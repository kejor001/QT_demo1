 #ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"
class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    //    explicit PlayScene(QWidget *parent = nullptr);
    PlayScene(int levelNum);
    int levelIndex;
    void paintEvent(QPaintEvent *);
    int  gameArray[4][4];//二维数组维护每一关具体的数据
    MyCoin * coinBtn[4][4];
    bool isWin =false;
signals:
    void chooseSceneBack();
};

#endif // PLAYSCENE_H
