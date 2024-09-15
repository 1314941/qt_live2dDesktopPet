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
## bug 
>打开exe后，界面不再实时刷新，需要重新打开，麻烦
在选择界面切换模型不会拍照，无法在选择界面正常显示新角色
目标文件夹没有图片时为啥会取最后一个读取成功的图片？
## 20240915更新
>修复了一些bug(代码水平有限，结构太乱了，昨天写的，今天就有点忘了，眼都快看花了，才找到问题...)
>>![image](https://github.com/user-attachments/assets/77b4b22c-ead2-432b-a866-85cb8199fcda)
>>添加了刷新功能，可以做到在程序打开后，加入新模型时不再需要重启程序了
>>修改了一些逻辑，实现手工复制大量模型文件夹到Resources文件夹里面后，可自动检测出未带有预览图片的模型并顺序加载
>![image](https://github.com/user-attachments/assets/29580f53-342c-4968-b639-9a5bfb80f856)

>>加上了开机自启功能(就这个最无语，缺少了一个附加库，搜了好久，最后靠ai才成功实现)
>>增加了播放设置，分为保存不变，顺序播放，随机播放，可自由设置切换时长，以分为单位。默认为随机播放，30分钟换一次。(但没实现设置保存，所以如果不满意默认设置，每次打开都要改一下)
## 咯咯咯(和平的鸽子)
感觉还能实现很多功能，如播放列表，删除模型，收藏夹，自动打开上次的模型等。
但感觉太麻烦啦！！！对于我来说，实在是个大工程。而且不太好做优化，太多功能就会卡顿了。
目前的功能对我来说已经够用，所以要去折腾别的东西了。以后看情况再更新。




