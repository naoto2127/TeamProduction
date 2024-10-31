#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/Audio.h"

//�^�C�g���V�[��
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle() {}

    //������
    void Initialize()override;

    //�I����
    void Finalize()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
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