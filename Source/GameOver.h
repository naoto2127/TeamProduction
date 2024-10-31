#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/Audio.h"

//�^�C�g���V�[��
class GameOver : public Scene
{
public:
    GameOver() {}
    ~GameOver() {}

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
    Sprite* moji = nullptr;
 
    std::unique_ptr<AudioSource> ResultMusic;
};
