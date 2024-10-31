#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/Audio.h"

//タイトルシーン
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle() {}

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
    Sprite* game = nullptr;
    Sprite* tutorial=nullptr;
    Sprite* explain[4] = {};
    Sprite* batu = nullptr;
    Sprite* next = nullptr;

    std::unique_ptr<AudioSource> TitleMusic;

    float gameScale = 1.0f;
    float tutorialScale = 1.0f;
    float nextScale = 1.0f;
    float batuScale = 0.0f;
    int count = 0;
    int explain_x = 0;
    int explain_y = 0;

    bool gamefalg = false;
    bool nextfalg = false;
    bool tutorialfalg = false;

    int SceneNum = 0;
    
};