#include "LogHandler.h"
#include "imagewin.h"
#include <QEvent>
#include <QApplication>
#include <QLabel>
#include <QPixmap>

imageWin::imageWin(QListWidgetItem *item, QWidget *parent): QWidget(parent)
{
    qApp->installEventFilter(this);


    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->setWindowIcon(item->icon());
    this->setWindowModality(Qt::ApplicationModal);//阻塞除当前窗体外的其他所有窗体

      // 设置窗口的背景颜色为黑色
    this->setStyleSheet("background-color: black;");
 
    //通过QLabel加载item上的图片
    QLabel *lab = new QLabel(this);
    lab->setFixedSize(this->width(), this->height());
    lab->setPixmap(item->icon().pixmap(QSize(lab->width(),lab->height())).scaled(lab->width(),lab->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // 设置标签的对齐方式为居中
    lab->setAlignment(Qt::AlignCenter);
    // 使窗口全屏显示
    this->showFullScreen();
}
 
imageWin::~imageWin()
{
    delete this;
}
 
//事件过滤器，实现点击窗口任意位置，窗口就可以关闭
bool imageWin::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        this->close();
    }
    return QWidget::eventFilter(obj, e);
}