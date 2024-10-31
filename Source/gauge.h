#pragma once


#include "Effect.h"
#include"Graphics/Sprite.h"



//ゲージ
class Gauge
{
public:
    Gauge();
    ~Gauge();

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc);

    float gaugeNum = 0;


private:
    ////追加
    ////ステート
    //enum class State
    //{
    //    Move,
    //    Pause1,
    //    Pause2,
    //    Pause3,
    //    Pause4,
    //    Hit,
    //};

private:
    Sprite* sprGauge = nullptr;

    

  
};