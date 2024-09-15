#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LogHandler.h"


#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include <QJsonObject>
#include <QJsonValue>
#include <QAction>
#include <iostream>
#include <Windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setMouseTracking(true);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->setWindowFlag(Qt::Tool);      //hide application icon
    this->setAttribute(Qt::WA_TranslucentBackground);

    // 设置背景图片  buhui
    //this->setStyleSheet("QMainWindow { background-image: url(':/Resources/back_class_normal.png); }");

    this->resize(400,800);
  
    initRightClickMenu(); //初始化右键菜单

    sw = new SubWin();
    initSystemTray();
    connect(sw,&SubWin::addModelSignal,this,[this]()
    {
        QString filepath = QFileDialog::getOpenFileName(this,"add Live2d Model","","JSON(*.model3.json)");
        if(!filepath.isEmpty())
        {
             this->loadModelFromJson(filepath);
             ui->openGLWidget->addModel(filepath);
             initModelList(assetPath);   //更新模型列表   
             curModelPath=filepath;
        }
    });

    connect(sw,&SubWin::addDirSignal,this,[this]()
    {
        autoPlayForNoImageModel();
    });

    connect(sw,&SubWin::changeModelSignal,this,[this](QString filepath)
    {
       changeModel(filepath);
    });

    connect(sw,&SubWin::changeRate,this,[this](int rate)
    {
       ui->openGLWidget->setRate(rate*30);
    });

    connect(sw,&SubWin::changeViewScale,this,[this](int scale)
    {
        ui->openGLWidget->adjustViewScale(scale);

        //根据比例调整窗口大小
        int w = defaultWidth;
        int h = defaultHeight;
        int newW = w * scale / 100;
        int newH = h * scale / 100;
        this->resize(newW, newH);
    });

    connect(sw,&SubWin::refreshSignal,this,[this]()
    {
        initModelList(assetPath);   //更新模型列表
    });

    connect(sw,&SubWin::changePlayMode,this,[this]()
    {
        playMode(); //切换播放模式
    });

    connect(sw,&SubWin::changeIntervalSignal,this,[this]()
    {
        playInterval(); //切换播放间隔
    });

    //hotkey
    QHotkey * hotkey = new QHotkey(QKeySequence("Ctrl+W"), true);   //switch next model

    connect(hotkey, &QHotkey::activated,this,[this]()
    {
        ui->openGLWidget->nextModel();
    });


    curDir = QApplication::applicationDirPath();
    
    modelListPath=QApplication::applicationDirPath() + "/output.txt";
    assetPath=curDir + "/Resources";
    
    initModelList(assetPath); //初始化模型列表

    loadModelsTxt(modelListPath, assetPath, ui);

    timer=new QTimer(this);
    timer->setInterval(sw->interVal);  //设置计时器间隔
    connect(timer, &QTimer::timeout, this, [this](){
        switchNextModel();   //切换下一个模型
    });
}

//将json文件路径写入output.txt文件
void MainWindow::initModelList(QString resourcePath)
{
    // [[1]] 安装消息处理函数
    LogHandler::Get().installMessageHandler();


    QString outputPath="output.txt";

    QDir dir(resourcePath);
    if (!dir.exists()) {
        qDebug() << "Resource folder does not exist.";
        return;
    }

   
    // 获取所有文件夹名
    resourcePath = dir.fromNativeSeparators(resourcePath);//  "\\"转为"/" 
    qDebug() << "Resource folder: " << resourcePath;
    QStringList mImgNames = QStringList("");
	dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	dir.setSorting(QDir::Name);
	mImgNames = dir.entryList();

    QStringList filters;
    filters << "*.json"; // 只检测 .model3.json 文件

    
    QFile outputFile(outputPath);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open output file for writing.";
        return;
    }
   
    QTextStream out(&outputFile);
    outputFile.seek(0); // 将文件指针移回文件开头
    QStringList lines;


	for (int i = 0; i < mImgNames.size(); ++i)
	{
		//qInfo() << "entryList: " << i << "-" << resourcePath + "/" +mImgNames[i];
        QDir son_dir(resourcePath + "/" + mImgNames[i]);
        
        son_dir.setNameFilters(filters);

        QStringList files = son_dir.entryList(QDir::Files);
        if (files.isEmpty()) {
            //qInfo() << "No .model3.json file found in " << resourcePath + "/" + mImgNames[i];
            initModelList(resourcePath + "/" + mImgNames[i]);   //递归调用，检测所有子文件夹
            return;
        }
        QString absolutePath;
        foreach (QString filePath, files) {
            if (filePath.contains("model3")) {
                //qInfo()<<"filePath:"<<filePath;
                //QFileInfo fileInfo(filePath);
                //absolutePath = fileInfo.absoluteFilePath();   得到的路径总少了“/Resources/”，搞不懂
                
                //拼接
                absolutePath = QString("%1/%2/%3").arg(resourcePath,mImgNames[i],filePath);
                //qInfo() << "Found .model3.json file: " << absolutePath;

                // 检查路径是否已经存在
                //qInfo() << "Adding " << absolutePath << " to output file.";
                out << absolutePath << "\n";
                lines.append(absolutePath);
            }
        }
 	}

    outputFile.close();
    qDebug() << "Output file created successfully.";
    modelList=lines;
    sw->modelList=lines;

    LogHandler::Get().uninstallMessageHandler();

    return;
}



//加载模型json文件
void MainWindow::loadModelsTxt(const QString &txtFilePath, const QString &curDir, Ui::MainWindow *ui) {
    // [[1]] 安装消息处理函数
    LogHandler::Get().installMessageHandler();
    //qInfo() << "Loading models from " << txtFilePath;

    QFile file(txtFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << txtFilePath;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty()) {
            QString modelPath = QString("%1").arg(line);
            //qDebug() << "Loading model: " << modelPath; // 打印加载的模型路径
            ui->openGLWidget->addInitModel(modelPath);
        } else {
            qWarning() << "Invalid model path format";
        }
    }
    qInfo() << "Models loaded successfully.";
    qInfo() << "正在刷新模型列表...";
    getModelList();
    curModelPath = modelList[0];
    qInfo() <<"刷新完毕！";
    LogHandler::Get().uninstallMessageHandler();
}


//更新模型列表modelList
void MainWindow::getModelList()
{
    LogHandler::Get().installMessageHandler();

    QString filePath = modelListPath;
    QFile file(filePath);
    

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件：" << filePath;
        return;
    }

    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        lines.append(line);
    }

    file.close();

    // 输出读取到的每一行
    /*qDebug() << "读取到的模型列表：";
    for (const QString& line : lines)
    {
        qDebug() << line;
    }
    */
    modelList = lines;
    sw->modelList=lines;
    LogHandler::Get().uninstallMessageHandler();
}

void MainWindow::changeModel(const QString filepath)
{
    if(!filepath.isEmpty())
    {
        this->loadModelFromJson(filepath);
        ui->openGLWidget->addModel(filepath);
        initModelList(assetPath);   //更新模型列表   
        curModelPath=filepath;
    }else{
        qDebug()<<"filepath is empty";
    }
}


QString MainWindow::get_next_modelPath()
{
    //如果随机播放，则每次随机获取下一个模型
    if(sw->isRandom)
    {
        int index = rand() % modelList.size();
        return modelList[index];
    }
    int currentIndex = modelList.indexOf(curModelPath);
    if (currentIndex == -1 || currentIndex == modelList.size() - 1)
    {
        return modelList[0]; // 第一个模型
    }

    return modelList[currentIndex + 1];
}



void MainWindow::switchNextModel()
{
    LogHandler::Get().installMessageHandler();
    getModelList();
    qDebug() << "curModelPath:" << curModelPath;
    //将bool值输出
    qDebug() << "是否随机播放："<<sw->isRandom;
    curModelPath = get_next_modelPath();
    changeModel(curModelPath);
    LogHandler::Get().uninstallMessageHandler();
}


void MainWindow::playMode()
{
    LogHandler::Get().installMessageHandler();
    if(!sw->last_isChangeModel&&sw->isChangeModel)  //之前为保持不变，现在切换为播放，创建计时器
    {
        timer->start(sw->interVal);
        qInfo()<<"播放计时器启动成功";

    }
    if(sw->last_isChangeModel&&!sw->isChangeModel)  //之前为播放，现在切换为保持不变，停止计时器
    {
        timer->stop();
        qInfo()<<"播放计时器停止成功";
    }
    LogHandler::Get().uninstallMessageHandler();
}

void MainWindow::playInterval()
{
    LogHandler::Get().installMessageHandler();
    timer->setInterval(sw->interVal);
    qInfo()<<"播放间隔修改成功";
    LogHandler::Get().uninstallMessageHandler();
}


MainWindow::~MainWindow()
{
    sw->close();
    delete trayIcon;
    delete ui;
}


void MainWindow::initRightClickMenu()
{
      //新建了5个Action
    QAction* m_ActionSetting = new QAction(tr("Setting"), this);
    QAction* m_ActionHide = new QAction(tr("Hide pet"), this);
    QAction* m_ActionSelect = new QAction(tr("Next model"), this);
    QAction* m_ActionAdd = new QAction(tr("Add model"), this);
    QAction* m_ActionQuit = new QAction(tr("Exit"),this);
  
    //开始添加Action
    this->addAction(m_ActionSetting);    
    this->addAction(m_ActionHide);
    this->addAction(m_ActionSelect);
    this->addAction(m_ActionAdd);
    
    //创建并添加分割线
    QAction* separator = new QAction();
    separator->setSeparator(true);
    this->addAction(separator);
    
    this->addAction(m_ActionQuit);
    
    //设置右键菜单策略为上下文菜单
    this->setContextMenuPolicy(Qt::ActionsContextMenu);


    connect(m_ActionSetting,&QAction::triggered,[this]()
    {
        sw->show();
    });
    connect(m_ActionSelect,&QAction::triggered,this,[this]()
    {
       switchNextModel();
       //ui->openGLWidget->nextModel();
    });
    connect(m_ActionQuit,&QAction::triggered,[]()
    {
        qApp->quit();
    });

    connect(m_ActionAdd,&QAction::triggered,this,[this]()
    {
        QString filepath = QFileDialog::getOpenFileName(this,"Add Live2d model","","JSON(*.model3.json)");
        if(!filepath.isEmpty())
        {
            this->loadModelFromJson(filepath);
            ui->openGLWidget->addModel(filepath);
            initModelList(assetPath);    
            curModelPath=filepath;
        }
    });
}

void MainWindow::initSystemTray()
{

    trayIcon = new QSystemTrayIcon();


    QThread * trayThread = new QThread();
    trayIcon->moveToThread(trayThread);
    trayThread->start();


    trayIcon->setIcon(QIcon(":/icons/panda.png"));

    QMenu* trayMenu = new QMenu();


    QAction* settingAction = trayMenu->addAction("setting");
    QAction* addAction = trayMenu->addAction("add model");
    QAction* nextAction = trayMenu->addAction("next model");
    QAction* quitAction = trayMenu->addAction("exit");


    // QMenu QSS
    QString menuQSS = "QMenu {background-color: #ffffff; border: 1px solid #ccc;} "
                      "QMenu::item {padding: 13px 20px 13px 20px; border-bottom: 1px solid #ccc;} "
                      "QMenu::item:selected {background-color: #def;}"
                      "QMenu::icon:checked {background: gray;}"
                      "QMenu::icon {padding:10px;}";


    trayMenu->setStyleSheet(menuQSS);


    quitAction->setIcon(style()->standardIcon(QStyle::SP_MessageBoxCritical));
    addAction->setIcon(style()->standardIcon(QStyle::SP_ToolBarHorizontalExtensionButton));
    nextAction->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    settingAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogInfoView));


    trayIcon->setContextMenu(trayMenu);
    connect(settingAction,&QAction::triggered,[this]()
    {
        sw->show();
    });
    connect(nextAction,&QAction::triggered,this,[this]()
    {
       switchNextModel();
       //ui->openGLWidget->nextModel();
    });
    connect(quitAction,&QAction::triggered,[]()
    {
        qApp->quit();
    });

    connect(addAction,&QAction::triggered,this,[this]()
    {
        QString filepath = QFileDialog::getOpenFileName(this,"Add Live2d model","","JSON(*.model3.json)");
        if(!filepath.isEmpty())
        {
             this->loadModelFromJson(filepath);
             ui->openGLWidget->addModel(filepath);
             initModelList(assetPath);    
             curModelPath=filepath;
             //ui->openGLWidget->screenShot(curModelPath);
        }
    });
    trayIcon->show();

}



void MainWindow::mouseMoveEvent(QMouseEvent*event)
{
    if(isPressed)
    {
        this->move(event->pos()-curPos+this->pos());
    }

}
void MainWindow::mousePressEvent(QMouseEvent*event)
{
    if(event->button()==Qt::LeftButton)
    {
        isPressed=true;
        curPos=event->pos();
    }
}


void MainWindow::mouseReleaseEvent(QMouseEvent*event)
{
    isPressed=false;
}


void MainWindow::changeAllMotion(const QString filepath)
{

    QFileInfo info(filepath);
    QString path = QString("%1/%2").arg(info.path(),"motions");
    QDir mPath(path);
    if (mPath.exists())
    {
        mPath.setFilter(QDir::Files);
        QFileInfoList fileList = mPath.entryInfoList();

        for(QFileInfo & fileInfo : fileList)
        {
            QString path = fileInfo.absoluteFilePath();
            changeMotionJson(path);
        }
    }
}

void MainWindow::changeMotionJson(const QString filepath)
{

    QFile file(filepath);
    if(file.open(QIODevice::ReadOnly))
    {

        QByteArray data = file.readAll();

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data,&error);
        file.close();
        if(error.error == QJsonParseError::NoError)
        {
            QJsonObject json = doc.object();
            QJsonObject meta = json["Meta"].toObject();
            int cnt = 0;
            if(meta.contains("CurveCount"))
            {
                cnt = meta["CurveCount"].toInt();
                if(meta.contains("TotalSegmentCount"))
                {

                    meta["TotalSegmentCount"]=cnt*1000;
                }
                if(meta.contains("TotalPointCount"))
                {

                    meta["TotalPointCount"]=cnt*1000;
                }

                json["Meta"]=meta;
                file.setFileName(filepath);
                file.open(QIODevice::WriteOnly);
                file.write(QJsonDocument(json).toJson());
                file.close();
            }

        }
    }

}
void MainWindow::readJsonData(const QString filepath,const QString ext,QJsonArray&jsonArr)
{


    QFileInfo info(filepath);
    QString path = QString("%1/%2").arg(info.path(),ext);
    QDir mPath(path);
    if (mPath.exists())
    {
        mPath.setFilter(QDir::Files);
        QFileInfoList fileList = mPath.entryInfoList();
        QJsonObject tmp;
        for(QFileInfo & fileInfo : fileList)
        {
            qDebug()<<fileInfo.fileName();
            tmp["File"]=ext+"/"+fileInfo.fileName();
            if(ext=="expressions")
            {
                tmp["Name"]=fileInfo.baseName();
            }
            jsonArr.append(tmp);
        }
    }
}
void MainWindow::loadModelFromJson(QString filepath)
{


    changeAllMotion(filepath);

    //read all motions and create json obj
    QJsonArray mArr;    //motions
    QJsonArray expArr;  //expressions

    readJsonData(filepath,"motions",mArr);
    readJsonData(filepath,"expressions",expArr);

    if(!filepath.isEmpty())
    {
        QFile file(filepath);
        if(file.open(QIODevice::ReadOnly))
        {

            QByteArray data = file.readAll();

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(data,&error);
            if(error.error == QJsonParseError::NoError)
            {
                QJsonObject json = doc.object();
                QJsonObject fileRef = json["FileReferences"].toObject();
                QJsonObject motions = fileRef["Motions"].toObject();

                motions["Idle"] = mArr;
                fileRef["Motions"]=motions;

                fileRef["Expressions"]=expArr;
                json["FileReferences"]=fileRef;
                file.close();

                file.setFileName(filepath);
                file.open(QIODevice::WriteOnly);
                file.write(QJsonDocument(json).toJson());
                file.close();
            }
        }
    }
}

void MainWindow::autoPlayForNoImageModel()
{
    LogHandler::Get().installMessageHandler();
    QStringList filters;
    filters << "*.json"; // 只检测 .model3.json 文件

    QStringList pngList=sw->pngList;
    //去除文件名为"new.png"的图片
    for(int i=0;i<pngList.size();i++)
    {
        if(pngList[i].contains("new.png"))
        {
            pngList.removeAt(i);
            i--;
        }
    }
    qInfo()<<"图片数量：" << pngList.size();
    qInfo()<<"模型数量：" << modelList.size();
    qInfo()<<"使用new.png的模型数量:"<<modelList.size()-pngList.size();
    //对比png和modelList的差异，如果有差异，则自动播放
    if(pngList.size()!=modelList.size())
    {
        qInfo()<<"图片数量和模型数量不匹配，自动播放...";
        //分别得到对应的文件夹路径
        QStringList pngDirList;
        QStringList modelDirList;
        for(int i=0;i<modelList.size();i++)   //一般都是modelList.size()>=pngList.size()
        {
            QFileInfo info(pngList[i]);
            pngDirList.append(info.absolutePath());   //获取文件所在文件夹路径
            QFileInfo info2(modelList[i]);
            modelDirList.append(info2.absolutePath());
        }
        //diff两个列表，得到两个列表的差集
        QStringList diffList;
        for(int i=0;i<modelDirList.size();i++)
        {
            if(!pngDirList.contains(modelDirList[i]))
            {
                diffList.append(modelList[i]);
            }
        }
        //如果差集不为空，则自动播放
        qInfo()<<"差集数量：" << diffList.size();
        //qInfo()<<"差集：" << diffList;
        if(diffList.size()!=0)
        {
            for(int i=0;i<diffList.size();i++)
            {
                qDebug() << "Loading model: " << diffList[i]; // 打印加载的模型路径
                changeModel(diffList[i]);
            }
        }
    }
    LogHandler::Get().uninstallMessageHandler();
}




