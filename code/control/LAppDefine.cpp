﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.

设置看这里

 */

#include "LAppDefine.hpp"
#include <CubismFramework.hpp>

namespace LAppDefine
{

    using namespace Csm;

    // 画面
    csmFloat32 ViewScale = 1.0f;
    const csmFloat32 ViewMaxScale = 2.0f;
    const csmFloat32 ViewMinScale = 0.8f;

    const csmFloat32 ViewLogicalLeft = -1.0f;
    const csmFloat32 ViewLogicalRight = 1.0f;
    const csmFloat32 ViewLogicalBottom = -1.0f;
    const csmFloat32 ViewLogicalTop = -1.0f;

    const csmFloat32 ViewLogicalMaxLeft = -2.0f;
    const csmFloat32 ViewLogicalMaxRight = 2.0f;
    const csmFloat32 ViewLogicalMaxBottom = -2.0f;
    const csmFloat32 ViewLogicalMaxTop = 2.0f;

    // 相対パス
    const csmChar* ResourcesPath = "Resources/";

    // モデルの後ろにある背景の画像ファイル
    const csmChar* BackImageName = "back_class_normal.png";
    // 歯車
    const csmChar* GearImageName = "icon_gear.png";
    // 終了ボタン
    const csmChar* PowerImageName = "close.png";

    // モデル定義------------------------------------------
    // モデルを配置したディレクトリ名の配列
    // ディレクトリ名とmodel3.jsonの名前を一致させておくこと


//    const csmChar* ModelDir[] =
//    {
//        "fangcao",
//        "Hiyori",
//        "Rice",
//        "Haru",
//        "Mark",
//        "Natori",
//        "Mao"
//    };
//    const csmInt32 ModelDirSize = sizeof(ModelDir) / sizeof(const csmChar*);

    // 外部定義ファイル(json)と合わせる
    const csmChar* MotionGroupIdle = "Idle"; // アイドリング
    const csmChar* MotionGroupTapBody = "TapBody"; // 体をタップしたとき

    // 外部定義ファイル(json)と合わせる
    const csmChar* HitAreaNameHead = "Head";
    const csmChar* HitAreaNameBody = "Body";

    // モーションの優先度定数
    const csmInt32 PriorityNone = 0;
    const csmInt32 PriorityIdle = 1;
    const csmInt32 PriorityNormal = 2;
    const csmInt32 PriorityForce = 3;

    // デバッグ用ログの表示オプション
    const csmBool DebugLogEnable = true;
    const csmBool DebugTouchLogEnable = false;

    // Frameworkから出力するログのレベル設定
    const CubismFramework::Option::LogLevel CubismLoggingLevel = CubismFramework::Option::LogLevel_Verbose;

    // デフォルトのレンダーターゲットサイズ
    const csmInt32 RenderTargetWidth = 1900;
    const csmInt32 RenderTargetHeight = 1000;


     //csmChar ** ModelDir=nullptr;               ///< モデルを配置したディレクトリ名の配列. ディレクトリ名とmodel3.jsonの名前を一致させておく.
    // csmInt32 ModelDirSize=0;             ///< モデルディレクトリ配列のサイズ
        //ModelDir = nullptr;
       // ModelDirSize=0;
    // const int MAX_MODELS=128;

    //new add



    QStringList ModelDir;

    void addModel(const QString modelPath)
    {
        if(!ModelDir.contains(modelPath))
        {
            ModelDir.append(modelPath);
        }
    }

     // 用于调整缩放率的函数
    void adjustViewScale(int value)
    {
        // 将拖拽条的值转换为缩放率
        csmFloat32 newScale = static_cast<csmFloat32>(value) / 100.0f;

        if (newScale < ViewMinScale) {
            ViewScale = ViewMinScale;
        } else if (newScale > ViewMaxScale) {
            ViewScale = ViewMaxScale;
        } else {
            ViewScale = newScale;
        }
    }
}
