#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include "chooselevelscene.h"
QT_BEGIN_NAMESPACE
namespace Ui { class mainscene; }
QT_END_NAMESPACE

class mainscene : public QMainWindow
{
    Q_OBJECT

public:
    mainscene(QWidget *parent = nullptr);
    ~mainscene();
    //重写painterEvent事件
    void paintEvent(QPaintEvent *);
    ChooseLevelScene *chooseScene=NULL;
private:
    Ui::mainscene *ui;
};
#endif // MAINSCENE_H
