#include "listwidgetuse.h"
#include "imagewin.h"
#include "LogHandler.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>
 
listWidgetUse::listWidgetUse(QWidget *parent,QStringList pngList,QStringList modelList) : QWidget(parent)
{
    LogHandler::Get().installMessageHandler();
 
    imgList = new QListWidget;
    imgList->setIconSize(QSize(125,125));//设置单个Icon大小
    imgList->setViewMode(QListView::IconMode);//设置显示模式
    imgList->setFlow(QListView::LeftToRight);//从左到右
    imgList->setResizeMode(QListView::Adjust);//大小自适应
    imgList->setMovement(QListView::Static);//设置列表每一项不可移动

    qInfo()<<"模型选择界面构建中...";
    files=pngList;
    for(int i=0;i<files.size();i++)
    {
        //qDebug()<<files.at(i);
        QListWidgetItem *newitem=new QListWidgetItem(QIcon(QPixmap(files.at(i)).scaled(125, 125)),files.at(i));
        //QListWidgetItem *newitem=new QListWidgetItem;
        newitem->setIcon(QIcon(files.at(i)));
        QFileInfo fi(files.at(i));

        newitem->setText(fi.dir().dirName());
        //根据图像像素大小设置item大小
        newitem->setSizeHint(QSize(125, 150));
        newitem->setFlags(newitem->flags() | Qt::ItemIsEnabled | Qt::ItemIsSelectable); // 设置项可以接收鼠标事件
        newitem->setTextAlignment(Qt::AlignHCenter);
        newitem->setData(Qt::UserRole, modelList.at(i));   //与模型文件名绑定

        imgList->addItem(newitem);
    }

    // 连接itemClicked信号到槽函数
    connect(imgList, &QListWidget::itemClicked, this, &listWidgetUse::onItemClicked);

    qInfo()<<"模型选择界面构建成功！";
    //样式表设计
    imgList->setStyleSheet("QListWidget{border:none; color:black;outline:0px; }"
                             "QListWidget::Item{padding-left:0px;padding-top:20px; padding-bottom:4px;color:black}"
                             "QListWidget::Item:hover{background:lightgray; color:green;}"
                             "QListWidget::item:selected{background:lightgray; color:green; }"
                             );
    connect(imgList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onItemDoubleClicked(QListWidgetItem *)));
 

    //双击时会触发单击事件，因此需要区分
    doubleClicked = new QTimer(this);
    connect(doubleClicked,&QTimer::timeout, this, &listWidgetUse::noDoubleClicked);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(imgList);

    LogHandler::Get().uninstallMessageHandler();
}


void listWidgetUse::onItemClicked()
{
    doubleClicked->start(100); // 启动一个300毫秒的定时器
}

void listWidgetUse::noDoubleClicked()
{
    qInfo()<<"one clicked!";
    enlargeImage(); // 打开图片窗口
}

 
void listWidgetUse::enlargeImage()
{
    QListWidgetItem *item = imgList->currentItem();
    //通过自定义的窗口显示图片
    imageWin *showImageWidget = new imageWin(item);
    showImageWidget->show();
    doubleClicked->stop(); // 停止定时器,即不会触发单击函数
}



void listWidgetUse::onItemDoubleClicked(QListWidgetItem *item)
{
    doubleClicked->stop(); // 停止定时器,即不会触发单击函数
    qInfo()<<"item  doubleclicked:"<<item->text();
    emit changeModel(item->data(Qt::UserRole).toString()); // 发射信号，传递模型文件名
}



//刷新
void listWidgetUse::refresh(QStringList pngList,QStringList modelList)
{
    LogHandler::Get().installMessageHandler();    
    qInfo()<<"刷新模型选择界面...";
    imgList->clear();
    files=pngList;
    for(int i=0;i<files.size();i++)
    {
        QListWidgetItem *newitem=new QListWidgetItem(QIcon(QPixmap(files.at(i)).scaled(125, 125)),files.at(i));
        newitem->setIcon(QIcon(files.at(i)));
        QFileInfo fi(files.at(i));
        newitem->setText(fi.dir().dirName());
        newitem->setSizeHint(QSize(125, 150));
        newitem->setFlags(newitem->flags() | Qt::ItemIsEnabled | Qt::ItemIsSelectable); // 设置项可以接收鼠标事件
        newitem->setTextAlignment(Qt::AlignHCenter);
        newitem->setData(Qt::UserRole, modelList.at(i));   //与模型文件名绑定
        imgList->addItem(newitem);
    }
    qInfo()<<"刷新模型选择界面成功！";
    LogHandler::Get().uninstallMessageHandler();
 
}


