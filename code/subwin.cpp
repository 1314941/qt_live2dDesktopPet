#include "subwin.h"
#include "ui_subwin.h"
#include"LogHandler.h"

#include <QDebug>
#include <iostream>
#include <Windows.h>
//设置面板


SubWin::SubWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubWin)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);

     //空字符串
    pngList=QStringList();
       
    modelListPath=QApplication::applicationDirPath() + "/output.txt";
    pngListPath=QApplication::applicationDirPath() + "/pngList.txt";

    getPngList();

    lw=new listWidgetUse(this,pngList,modelList);
    ui->tabWidget->insertTab(0,lw,"模型选择");

    connect(lw,&listWidgetUse::changeModel,[this](QString modelPath)
    {
        emit changeModelSignal(modelPath);
    });

    connect(ui->btnAdd,&QPushButton::clicked,[this]()
    {
        emit addModelSignal();
    });

    connect(ui->btnAddDir,&QPushButton::clicked,[this]()
    {
        emit addDirSignal();
    });


    isAutoStart=detectAutoStart();
    if(!isAutoStart){   
        ui->btnAutoStart->setText("开机自启");  //当前未开启
    }else{
        ui->btnAutoStart->setText("取消开机自启");  
    }
    connect(ui->btnAutoStart,&QPushButton::clicked,[this]()
    {
        autoStart();
    });
    //帧率
    ui->comRate->addItem("30");
    ui->comRate->addItem("60");
    ui->comRate->addItem("90");
    ui->comRate->addItem("120");
    ui->comRate->setCurrentIndex(1);
    connect(ui->comRate,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int index)
    {
        emit changeRate(index+1);
    });

    //播放方式
    ui->comRate_live2d->addItem("保持不变");
    ui->comRate_live2d->addItem("顺序播放");
    ui->comRate_live2d->addItem("随机播放");
    ui->comRate_live2d->setCurrentIndex(2);
    last_isChangeModel=true;
    isChangeModel=true;
    isRandom=false;
    interVal=1800000;  //默认30分钟切换一次  一分钟 60000毫秒
    ui->lineEdit_live2d->setText("30");  

    // 连接信号和槽
    connect(ui->comRate_live2d, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,&SubWin::onPlayModeChanged);

    connect(ui->lineEdit_live2d,&QLineEdit::textChanged,[this](const QString& text)
    {
        //改变切换间隔
        changeInterval(text);
    });

    //刷新按钮
    connect(ui->btnRefresh,&QPushButton::clicked,[this]()
    {
        emit refreshSignal();
        getPngList();   
        lw->refresh(pngList,modelList);
    });

    connect(ui->btnClose,&QPushButton::clicked,[this]()
    {
        this->hideWin();
    });
    ui->btnRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->btnClose->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));

    ui->verticalLayout->setContentsMargins(0,0,0,0);
    ui->verticalLayout->setSpacing(0);

    ui->viewScaleSlider->setMinimum(10);
    ui->viewScaleSlider->setMaximum(500);
    ui->viewScaleSlider->setValue(100);
    ui->viewScaleLine->setText("1.0");

    connect(ui->viewScaleSlider,&QSlider::valueChanged,[this](int value)
    {
        ui->viewScaleLine->setText(QString::number(value/100.0));
        emit changeViewScale(value);
    });
 
    
    loadQss();
    LogHandler::Get().installMessageHandler();
    qInfo() << "设置面板加载完成";
    LogHandler::Get().uninstallMessageHandler();
}



SubWin::~SubWin()
{
    delete ui;
}


void SubWin::loadQss()
{
//    QFile file(":/style/subWin.qss");
//    if(file.open(QIODevice::ReadOnly))
//    {
//        this->setStyleSheet(file.readAll());
//    }
}


void SubWin::mouseMoveEvent(QMouseEvent*event)
{
    if(isPressed)
    {
        this->move(event->pos()-curPos+this->pos());
    }
}




void SubWin::mousePressEvent(QMouseEvent*event)
{
    if(event->button()==Qt::LeftButton)
    {
        isPressed=true;
        curPos=event->pos();
    }
}

void SubWin::mouseReleaseEvent(QMouseEvent*event)
{
    isPressed=false;
}

void SubWin::hideWin()
{
    this->hide();
}


void SubWin::getPngList()
{
    LogHandler::Get().installMessageHandler();
    qInfo() << "开始获取模型图片列表";

    QString filePath = modelListPath;   //获取模型列表
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

    // 输出行数
    qDebug() << "模型共" << lines.size() << "个";
    pngList=QStringList();  //清空
    for (const QString& line : lines)
    {
        //qDebug() <<"model3.json:"<< line;
        //获取文件夹
        QFileInfo fileInfo(line);
        QString dirPath = fileInfo.absolutePath();
        QString pngPath = findExamplePng(dirPath);
        //如果存在example.png则添加到列表
        QFileInfo pngInfo(pngPath);
        if(pngInfo.exists()){
            //qDebug() << "example.png路径：" << pngPath;
            pngList.append(pngPath);
        }
    }
    qDebug() << "模型图片共:" << pngList.size() << "个";

    // 创建一个QFile对象
    QFile pngFile(pngListPath);

    // 以写入模式打开文件
    if (pngFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // 创建一个QTextStream对象
        QTextStream out(&pngFile);

        // 遍历pngList，将每个路径写入文件
        for (const QString &path : pngList) {
            out << path << "\n";
        }

        // 关闭文件
        pngFile.close();

        qDebug() << "路径已写入文件";
    } else {
        qDebug() << "无法打开文件";
    }
    modelList = lines;
    qInfo() << "获取模型图片列表成功";

    LogHandler::Get().uninstallMessageHandler();
}


QString SubWin::findExamplePng(const QString &folderPath)
 {
    QString result;

    // 创建一个QDir对象来操作文件夹
    QDir dir(folderPath);

    // 获取文件夹中的所有文件和目录
    QStringList filters;
    filters << "*.png";
    dir.setNameFilters(filters);

    // 遍历所有文件
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for (const QFileInfo &fileInfo : fileList) {
        if (fileInfo.fileName() == "example.png") {
            result = fileInfo.absoluteFilePath();
            return result;
        }
    }
    return "Resources/new.png";   //Resource前不能加“/”
}


void SubWin::onPlayModeChanged(int index)
{
    last_isChangeModel=isChangeModel;
    // 根据选择的播放方式执行相应的操作
    switch (index) {
    case 0: // 保持不变
        // 执行保持不变的逻辑
        isChangeModel=false;
        break;
    case 1: // 顺序播放
        // 执行顺序播放的逻辑
        isChangeModel=true;
        isRandom=false;
        break;
    case 2: // 随机播放
        // 执行随机播放的逻辑
        isChangeModel=true;
        isRandom=true;
        break;
    default:
        break;
    }
    emit changePlayMode();

}

void SubWin::changeInterval(const QString& text)
{
    LogHandler::Get().installMessageHandler();
    //如果不是数字，则不进行处理
    if(!text.toDouble()){    
        qWarning() <<text << "不是数字";
        return;
    }
    interVal=text.toDouble()*60000;
    qInfo()<<"切换间隔为"<<text<<"分钟";
    emit changeIntervalSignal();// 发射信号
    LogHandler::Get().uninstallMessageHandler();
}


/*
不懂，ai教的

使用 toStdWString() 方法将 curDir 转换为 std::wstring，
然后使用 c_str() 方法将 std::wstring 转换为 const wchar_t*。
我们使用 wcslen(path) + 1 来计算数据的大小，因为 wcslen 函数返回的是字符的数量，不包括终止的空字符。

请注意，RegSetValueExW 函数的最后一个参数是 DWORD 类型的，它表示要写入的数据的大小。
在这个例子中，我们使用 (wcslen(path) + 1) * sizeof(wchar_t) 来计算数据的大小。
这是因为 path 字符串中的每个字符都占用一个字节，所以数据的大小等于字符的数量加一乘以每个字符的大小。
*/


void SubWin::autoStart()
{
    LogHandler::Get().installMessageHandler();
    if (!isAutoStart) {
        // 获取当前可执行文件的路径
        QString curDir = QApplication::applicationDirPath();
        qInfo()<<"当前可执行文件路径为"<<curDir;
        std::wstring exepath = curDir.toStdWString() + L"/DesktopLive2D.exe";
        const wchar_t* path = exepath.c_str();

        // 打开注册表项
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            // 设置注册表项的值，将程序路径添加为开机启动项
            if (RegSetValueExW(hKey, L"Live2dPet", 0, REG_SZ, reinterpret_cast<const BYTE*>(path), (wcslen(path) + 1) * sizeof(wchar_t)) == ERROR_SUCCESS) {
                qDebug() << "程序已成功添加到开机启动项！";
            }
            else {
                qDebug() << "添加到开机启动项失败！";
            }
            // 关闭注册表项
            RegCloseKey(hKey);
            isAutoStart = true;
            ui->btnAutoStart->setText("取消开机自启");
        }
        else {
            qDebug() << "无法打开注册表项！";
        }
    }else {
        // 取消开机自启
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            // 删除注册表项的值
            if (RegDeleteValue(hKey, L"Live2dPet") == ERROR_SUCCESS) {
                qDebug() << "已成功取消开机自启！";
            }
            else {
                qDebug() << "取消开机自启失败！";
            }
            // 关闭注册表项
            RegCloseKey(hKey);
            isAutoStart = false;
            ui->btnAutoStart->setText("开机自启");
        }
        else {
            qDebug() << "无法打开注册表项！";
        }
    }
    LogHandler::Get().uninstallMessageHandler();
}

bool SubWin::detectAutoStart()
{
    bool flag=false;
    LogHandler::Get().installMessageHandler();
    // 检查是否设置了开机自启
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD size = MAX_PATH * sizeof(wchar_t);
        wchar_t value[MAX_PATH];
        if (RegQueryValueExW(hKey, L"Live2dPet", NULL, NULL, reinterpret_cast<LPBYTE>(value), &size) == ERROR_SUCCESS) {
            qDebug() << "开机自启已启用：" << value;
            flag=true;
        }
        else {
            qDebug() << "开机自启未启用！";
        }
        // 关闭注册表项
        RegCloseKey(hKey);
    }
    else {
        qDebug() << "无法打开注册表项！";
    }

    LogHandler::Get().uninstallMessageHandler();
    return flag;
}
