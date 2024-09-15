#ifndef COPY_H
#define COPY_H

#include <QObject>

class Copy : public QObject
{
    Q_OBJECT
public:
    Copy(QObject *parent = nullptr);
    ~Copy();
    
    //静态函数：
    static bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
    //拷贝文件夹：
    static bool  copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
private:
    bool coverFileIfExist;
};

#endif // COPY_H
