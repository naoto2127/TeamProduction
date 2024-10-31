#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/Audio.h"

//タイトルシーン
class GameOver : public Scene
{
public:
    GameOver() {}
    ~GameOver() {}

    //初期化
    void Initialize()override;

    //終了化
    void Finalize()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render()override;

private:
    Sprite* sprite = nullptr;
    Sprite* moji = nullptr;
 
    std::unique_ptr<AudioSource> ResultMusic;
};
