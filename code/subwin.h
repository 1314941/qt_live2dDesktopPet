#ifndef SUBWIN_H
#define SUBWIN_H

#include "preDefine.h"
#include"listWidgetUse.h"

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class SubWin;
}

class SubWin : public QWidget
{
    Q_OBJECT

public:
    explicit SubWin(QWidget *parent = nullptr);
    ~SubWin();
    QString pngListPath;
    QStringList pngList;  
    QStringList modelList;   //存储所有模型路径
    QString modelListPath;   //txt文件路径 

    //切换标识
    bool last_isChangeModel;
    bool isChangeModel;
    bool isRandom;
    int interVal;

    void hideWin();
    void onCloseBtn();
    void loadQss();
    void getPngList();
    QString findExamplePng(const QString &folderPath);
    void changeInterval(const QString &text);
protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event );
    void mouseMoveEvent(QMouseEvent* event);
    void autoStart();
    bool detectAutoStart();
signals:
    void addModelSignal();
    void addDirSignal();
    void changeRate(int rate);
    void changeModelSignal(QString modelPath);
    void changeViewScale(int value);
    void changePlayMode();
    void changeIntervalSignal();
    void refreshSignal();
private:
    Ui::SubWin *ui;

    listWidgetUse * lw;
    bool isPressed;
    QPoint curPos;
    QString assetPath;    //resource文件夹
    QString curModelPath;
    
    
    bool isAutoStart;
public slots:
    //void changeModelSlot(QString modelPath);
private slots:
    void onPlayModeChanged(int index);
};

#endif // SUBWIN_H
