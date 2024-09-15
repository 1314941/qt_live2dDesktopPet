#include "control/LAppDelegate.hpp" //set in the front
#include "control/LAppView.hpp"
#include "control/LAppPal.hpp"
#include "control/LAppLive2DManager.hpp"
#include "control/LAppDefine.hpp"
#include "live2dwidget.h"
#include "copy.h"
#include "LogHandler.h"

#include <QTextStream>
#include <QApplication>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QThread>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include <QJsonObject>
#include <QJsonValue>
#include <QTime>
#include<QPixmap>
#include<QPainter>
#include<QImage>
#include<QDatetime>

Live2DWidget::Live2DWidget(QWidget *parent)
    : QOpenGLWidget{parent}
{

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);
    this->setGeometry(1200, 100, 400, 800); // 将窗口移动到(100, 100)位置，并调整为800x600大小

    fpsTimer = new QTimer(this);
    isPressed = false;
    fpsTimer->start((1.0/60.0)*1000);
    connect(fpsTimer,&QTimer::timeout,this,&Live2DWidget::onTimerUpdate);

}

Live2DWidget::~Live2DWidget()
{

    LAppDelegate :: GetInstance()->Release();
    LAppDelegate::ReleaseInstance();
}


void Live2DWidget::nextModel()
{
    this->makeCurrent();
    LAppLive2DManager::GetInstance()->NextScene();
    this->doneCurrent();
}

void Live2DWidget::onTimerUpdate()
{
    update();
}

int Live2DWidget::getModelsSize()
{
    return  LAppDefine::ModelDir.size();
}





void Live2DWidget::addModel(const QString path)
{
    // [[1]] 安装消息处理函数
    LogHandler::Get().installMessageHandler();

    // [[2]] 输出测试，查看是否写入到文件
    qDebug() << "Hello";
    qDebug() << "当前时间是: " << QTime::currentTime().toString("hh:mm:ss");
    //qInfo() << QString("God bless you!");

    QFile file(path);
    if(file.exists())
    {
        LAppDefine::addModel(path);

        int index = LAppDefine::ModelDir.indexOf(path);
        index=(index==-1?LAppDefine::ModelDir.size()-1:index);
        this->makeCurrent();
        LAppLive2DManager::GetInstance()->ChangeScene(index);
        this->doneCurrent();
        screenShot(path);
        qDebug()<<"模型验证成功！";

        QString outputPath="output.txt";
        QFile outputFile(outputPath);
        //将模型路径写入到output.txt文件中
        qDebug() << "Output file path: " << outputPath;
        if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open output file for writing.";
            return;
        }
       
        // 获取模型文件所在文件夹的路径
        QString folderPath = QFileInfo(path).dir().path();

        // 检查文件夹是否已经存在于Resources文件中
        QString curDir = QApplication::applicationDirPath();
        QString resourcePath = curDir + "/Resources/" + QFileInfo(folderPath).fileName();
        qDebug()<<"folderPath: "<<folderPath;
        qDebug() << "resourcePath: " << resourcePath;
        
        QString newModelPath = resourcePath + "/" + QFileInfo(path).fileName();
        qDebug()<<"newModelPath: "<<newModelPath<<"\r\n";
        try{
            if (!QFile(newModelPath).exists())
            {
                qDebug()<<"模型文件不存在，开始复制文件夹。";
                // 如果文件夹不存在，则复制文件夹到Resources文件中
                if (!QDir().exists(resourcePath))
                {
                    if (!QDir().mkpath(resourcePath))
                    {
                        qDebug()<<"Faild to create directory:"<<resourcePath<<"\r\n";
                    }
                }
                    //else if (!QFile::copy(folderPath, resourcePath))
                if (!Copy::copyDirectoryFiles(folderPath, resourcePath,false))
                {
                    qDebug()<<"Failed to copy directory:"<<folderPath<<"to" << resourcePath << "\r\n";
                }
                
            }else{
                qDebug()<<"模型文件已存在，无需复制文件夹。";
            }
        } catch (const std::exception& e) {
            qDebug() << "Exception caught: " << e.what();
        }

        qDebug()<<"Model path added to txt file.\r\n";
        outputFile.close();
    }

    LogHandler::Get().uninstallMessageHandler();
}



void Live2DWidget::initializeGL()
{
    LAppDelegate::GetInstance()->Initialize(this);
}

void Live2DWidget::enterEvent(QEvent*e)
{

}
void Live2DWidget::resizeGL(int w, int h)
{
    LAppDelegate::GetInstance()->resize(w,h);
}

void Live2DWidget::paintGL()
{
    LAppDelegate::GetInstance()->update();
    deltaTime = LAppPal::GetDeltaTime();


}

void Live2DWidget::mouseMoveEvent(QMouseEvent *e)
{
     LAppDelegate::GetInstance()->GetView()->OnTouchesMoved(e->x(),e->y());
     update();
     e->ignore();
}
void Live2DWidget::mousePressEvent(QMouseEvent * e)
{


    if(e->button() == Qt::LeftButton)
    {
        isPressed = true;
        LAppDelegate::GetInstance()->GetView()->OnTouchesBegan(e->x(),e->y());

    }
     e->ignore();

}

void Live2DWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton)
    {
        isPressed = false;
        LAppDelegate::GetInstance()->GetView()->OnTouchesEnded(e->x(), e->y());
        update();
    }
     e->ignore();
}

void Live2DWidget::addInitModel(QString modelPath)
{
    QFile file(modelPath);
    if(file.exists())
    {
        LAppDefine::addModel(modelPath);
    }
}

void Live2DWidget::changeModel(QString path)
{
    LogHandler::Get().installMessageHandler();
    qInfo()<<"changeModel";
    qInfo()<<"path:"<<path;
    addModel(path);
    screenShot(path);
    LogHandler::Get().uninstallMessageHandler();
}



void Live2DWidget::setRate(float rate)
{
    fpsTimer->stop();
    fpsTimer->start((1.0/rate)*1000);
}


void Live2DWidget::adjustViewScale(int scale)
{
    LogHandler::Get().installMessageHandler();
    qInfo()<<"changeViewScale";
    qInfo()<<"scale:"<<scale/100.0f;
    //LAppDefine::adjustViewScale(scale);   没用
    LogHandler::Get().uninstallMessageHandler();
     
}


void Live2DWidget::screenShot(QString modelPath)
{
    LogHandler::Get().installMessageHandler();

    // 模型加载成功后进行截图
    QPixmap screenshot(size()); // 创建一个与Live2DWidget大小相同的QPixmap
    QPainter painter(&screenshot);
    render(&painter); // 使用QPainter绘制Live2DWidget的内容到screenshot
    painter.end();

    QImage image = screenshot.toImage(); // 将QPixmap转换为QImage

    // 保存截图
    QFileInfo fileInfo(modelPath);
    QString savePath = fileInfo.absolutePath() + "/example.png";
    
    image.save(savePath, "PNG");

    qDebug() << "截图已保存到：" << savePath;
    
    LogHandler::Get().uninstallMessageHandler();
}
