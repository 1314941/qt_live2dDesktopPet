/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.

 类名: LAppView
成员变量:
TouchManager* _touchManager: 指向 TouchManager 类型的指针，用于管理触摸事件。
Csm::CubismMatrix44* _deviceToScreen: 指向 CubismMatrix44 类型的指针，用于设备坐标到屏幕坐标的转换。
Csm::CubismViewMatrix* _viewMatrix: 指向 CubismViewMatrix 类型的指针，用于视图矩阵的设置。
GLuint _programId: 用于存储OpenGL着色器程序的ID。
LAppSprite* _back, _gear, _power: 指向 LAppSprite 类型的指针，用于存储背景、齿轮和电源的图像。
LAppSprite* _renderSprite: 指向 LAppSprite 类型的指针，用于在渲染到其他目标时绘制纹理。
Csm::Rendering::CubismOffscreenFrame_OpenGLES2 _renderBuffer: 用于存储离屏渲染缓冲区。
SelectTarget _renderTarget: 用于选择渲染目标。
float _clearColor[4]: 用于存储渲染目标的清除颜色。

Initialize(): 初始化函数，设置视图矩阵和触摸管理器。
Render(): 渲染函数，负责绘制视图中的所有元素。
InitializeSprite(): 初始化图像函数，设置背景、齿轮和电源的图像。
ResizeSprite(): 调整图像大小函数，根据窗口大小调整图像尺寸。
OnTouchesBegan(), OnTouchesMoved(), OnTouchesEnded(): 处理触摸事件的函数，分别对应触摸开始、触摸移动和触摸结束。
TransformViewX(), TransformViewY(), TransformScreenX(), TransformScreenY(): 坐标转换函数，将设备坐标转换为视图坐标或屏幕坐标。
PreModelDraw(LAppModel& refModel): 在绘制模型之前调用的函数。
PostModelDraw(LAppModel& refModel): 在绘制模型之后调用的函数。
GetSpriteAlpha(int assign) const: 获取精灵的透明度。
SwitchRenderingTarget(SelectTarget targetType): 切换渲染目标。
SetRenderTargetClearColor(float r, float g, float b): 设置渲染目标的清除颜色。
 
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Math/CubismMatrix44.hpp>
#include <Math/CubismViewMatrix.hpp>
#include "CubismFramework.hpp"
#include <Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp>

class TouchManager;
class LAppSprite;
class LAppModel;

/**
* @brief 描画クラス
*/
class LAppView
{
public:

    /**
     * @brief LAppModelのレンダリング先
     */
    enum SelectTarget
    {
        SelectTarget_None,                ///< デフォルトのフレームバッファにレンダリング
        SelectTarget_ModelFrameBuffer,    ///< LAppModelが各自持つフレームバッファにレンダリング
        SelectTarget_ViewFrameBuffer,     ///< LAppViewの持つフレームバッファにレンダリング
    };

    /**
    * @brief コンストラクタ
    */
    LAppView();

    /**
    * @brief デストラクタ
    */
    ~LAppView();

    /**
    * @brief 初期化する。
    */
    void Initialize();

    /**
    * @brief 描画する。
    */
    void Render();

    /**
    * @brief 画像の初期化を行う。
    */
    void InitializeSprite();

    /**
    * @brief スプライト系のサイズ再設定
    */
    void ResizeSprite();

    /**
    * @brief タッチされたときに呼ばれる。
    *
    * @param[in]       pointX            スクリーンX座標
    * @param[in]       pointY            スクリーンY座標
    */
    void OnTouchesBegan(float pointX, float pointY) const;

    /**
    * @brief タッチしているときにポインタが動いたら呼ばれる。
    *
    * @param[in]       pointX            スクリーンX座標
    * @param[in]       pointY            スクリーンY座標
    */
    void OnTouchesMoved(float pointX, float pointY) const;

    /**
    * @brief タッチが終了したら呼ばれる。
    *
    * @param[in]       pointX            スクリーンX座標
    * @param[in]       pointY            スクリーンY座標
    */
    void OnTouchesEnded(float pointX, float pointY) const;

    /**
    * @brief X座標をView座標に変換する。
    *
    * @param[in]       deviceX            デバイスX座標
    */
    float TransformViewX(float deviceX) const;

    /**
    * @brief Y座標をView座標に変換する。
    *
    * @param[in]       deviceY            デバイスY座標
    */
    float TransformViewY(float deviceY) const;

    /**
    * @brief X座標をScreen座標に変換する。
    *
    * @param[in]       deviceX            デバイスX座標
    */
    float TransformScreenX(float deviceX) const;

    /**
    * @brief Y座標をScreen座標に変換する。
    *
    * @param[in]       deviceY            デバイスY座標
    */
    float TransformScreenY(float deviceY) const;

    /**
     * @brief   モデル1体を描画する直前にコールされる
     */
    void PreModelDraw(LAppModel& refModel);

    /**
     * @brief   モデル1体を描画した直後にコールされる
     */
    void PostModelDraw(LAppModel& refModel);

    /**
     * @brief   別レンダリングターゲットにモデルを描画するサンプルで
     *           描画時のαを決定する
     */
    float GetSpriteAlpha(int assign) const;

    /**
     * @brief レンダリング先を切り替える
     */
    void SwitchRenderingTarget(SelectTarget targetType);

    /**
     * @brief レンダリング先をデフォルト以外に切り替えた際の背景クリア色設定
     * @param[in]   r   赤(0.0~1.0)
     * @param[in]   g   緑(0.0~1.0)
     * @param[in]   b   青(0.0~1.0)
     */
    void SetRenderTargetClearColor(float r, float g, float b);

private:
    TouchManager* _touchManager;                 ///< タッチマネージャー
    Csm::CubismMatrix44* _deviceToScreen;    ///< デバイスからスクリーンへの行列
    Csm::CubismViewMatrix* _viewMatrix;      ///< viewMatrix
    GLuint _programId;                       ///< シェーダID

    //设置默认为空 不进行设置
    LAppSprite* _back;                       ///< 背景画像
    LAppSprite* _gear;                       ///< ギア画像
    LAppSprite* _power;                      ///< 電源画像

    // レンダリング先を別ターゲットにする方式の場合に使用
    LAppSprite* _renderSprite;                                      ///< モードによっては_renderBufferのテクスチャを描画
    Csm::Rendering::CubismOffscreenFrame_OpenGLES2 _renderBuffer;   ///< モードによってはCubismモデル結果をこっちにレンダリング
    SelectTarget _renderTarget;     ///< レンダリング先の選択肢
    float _clearColor[4];           ///< レンダリングターゲットのクリアカラー
};
