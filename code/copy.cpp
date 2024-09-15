#include"copy.h"

#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QMutex>
#include <QString>
#include <qapplication.h>
#include <QMessageBox>
#include <QDir>



//拷贝文件： 静态函数
bool Copy::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
	toDir.replace("\\","/");
	if (sourceDir == toDir){
		return true;
	}
	if (!QFile::exists(sourceDir)){
		return false;
	}
	QDir *createfile     = new QDir;
	bool exist = createfile->exists(toDir);
	if (exist){
		if(coverFileIfExist){
			createfile->remove(toDir);
		}
	}//end if
 
	if(!QFile::copy(sourceDir, toDir))
	{
		return false;
	}
	return true;
}
 
//拷贝文件夹：
bool Copy::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
	QDir sourceDir(fromDir);
	QDir targetDir(toDir);
	if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
		if(!targetDir.mkdir(targetDir.absolutePath()))
			return false;
	}
 
	QFileInfoList fileInfoList = sourceDir.entryInfoList();
	foreach(QFileInfo fileInfo, fileInfoList){
		if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;
 
		if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
			if(!copyDirectoryFiles(fileInfo.filePath(), 
				targetDir.filePath(fileInfo.fileName()),
				coverFileIfExist))
				return false;
		}
		else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
			if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
				targetDir.remove(fileInfo.fileName()); 
			}
 
			/// 进行文件copy
			if(!QFile::copy(fileInfo.filePath(), 
				targetDir.filePath(fileInfo.fileName()))){
					return false;
			}
		}
	}
	return true;
}


