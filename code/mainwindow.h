#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <subwin.h>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QStyle>
#include <QHotKey>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   
    //检测resources文件夹下的live2d文件夹下的模型文件
    void initModelList(QString path);
    //加载模型json文件路径
    void loadModelsTxt(const QString &txtFilePath, const QString &curDir, Ui::MainWindow *ui);
    void initRightClickMenu();
    void initSystemTray();

    void readJsonData(const QString filepath,const QString ext,QJsonArray&jsonArr);
    void loadModelFromJson(QString filepath);
    void changeMotionJson(const QString filepath);
    void changeAllMotion(const QString filepath);
    void changeModel(const QString filepath);
    void switchNextModel();
    void autoPlayForNoImageModel(); //自动播放没有图片的模型

    void getModelList();
    QString get_next_modelPath();

    void playMode();
    void playInterval();
protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event );
    void mouseMoveEvent(QMouseEvent* event);


   // bool eventFilter(QObject *target, QEvent *event);
private:
    Ui::MainWindow *ui;
    bool isPressed;
    QPoint curPos;
    SubWin * sw;
   
    QSystemTrayIcon* trayIcon;
    QString curDir; 
    QString assetPath;    //resource文件夹

    QString curModelPath;   
    QStringList modelList;   //存储所有模型路径
    QString modelListPath;   //txt文件路径


    //默认长宽
    int defaultWidth = 400;
    int defaultHeight = 800;

    QTimer* timer;  //模型切换定时器
};
#endif // MAINWINDOW_H
