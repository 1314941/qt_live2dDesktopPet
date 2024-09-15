#ifndef LISTWIDGETUSE_H
#define LISTWIDGETUSE_H
 
#include <QWidget>
#include <QListWidget>
#include <QString>
#include <QListWidgetItem>
#include <QPixmap>
#include <QIcon>
#include <QTimer>


class listWidgetUse : public QWidget
{
    Q_OBJECT
public:
    listWidgetUse(QWidget *parent = 0,QStringList pngList = QStringList(),QStringList modelList=QStringList());
    void refresh(QStringList pngList, QStringList modelList);
private:
    QListWidget *imgList;
    QString path;
    QStringList modelfiles;
    QStringList files;
    QTimer *doubleClicked; //双击定时器
signals:
    void changeModel(QString);
public slots:
    void noDoubleClicked();
    void onItemClicked();
    void enlargeImage();//放大图片
    void onItemDoubleClicked(QListWidgetItem *item);
};
 
#endif // LISTWIDGETUSE_H