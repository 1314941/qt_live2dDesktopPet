# 声明
基于[QtLive2dDesktop](https://github.com/FLCYR/QtLive2dDesktop)，加了一点自己想要的东西。
## new
>将选中的model3.json文件所在文件夹复制到Resource文件夹内(不会重复复制)
>每次启动时自动检测Resource文件夹内的模型，并记录在output.txt文件内
# 起因
最近对桌宠很感兴趣，见到了[基于Qt的Live2D模型显示以及控制](https://blog.csdn.net/y123456wydhckd/article/details/134899183)这篇博客后当即就clone了[QtLive2dDesktop](https://github.com/FLCYR/QtLive2dDesktop)这个仓库下来。试了一下后，感觉很惊喜。就是有些功能不太完善，所以决定自己改一丢丢。
# 如何跑起来
## 环境(我用的)
>win10
>qt creator 13.0.0
>qt 6.7.2
>>![image](https://github.com/user-attachments/assets/38e884fa-1012-4df4-8c2f-a10d2a490acc)



>构建套件 msvc2019 在安装时记得勾选上
>![image](https://github.com/user-attachments/assets/5f16eb40-0b59-4374-ab9a-54723b4f30b6)

>原作者提到的安装visual studio2019,由于我已经有了2022版的，实在懒得重装，就硬着头皮试了一下，实测可以运行
>另外，虽然原作者用到是qt5，但qt6也是ok的哦

## 运行
编译得到exe文件后，还需要最后一步。
[QT打包成一个exe文件。（多种方法）](https://blog.csdn.net/xideaha/article/details/132382764)
>来自笔者的惨痛经历
>如果同时安装了mingw和msvc的话，不要粗心的进错了终端，不然会在打开exe文件时出现类似“无法找到动态链接库”的报错。
>本项目用到的是msvc的终端！

# 目标
先立一个flag
**接入ai**

## 20240914更新
>更友好的桌宠选择界面，单击预览(有点简陋)，双击加载模型
>![image](https://github.com/1314941/qt_live2dDesktopPet/blob/dev/Resources/video.gif)
>实现桌宠缩放功能，默认1，可放大至五倍(还可以更大，但会遮挡完屏幕，所以设置为最大五倍)，缩小至0.1倍
>![image](https://github.com/1314941/qt_live2dDesktopPet/blob/dev/Resources/video_2.gif)



关于接入ai功能的实现，查了一下，感觉可以调用python接口来实现，可能会在中秋假期后继续完善
