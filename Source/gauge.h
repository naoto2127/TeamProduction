#pragma once


#include "Effect.h"
#include"Graphics/Sprite.h"



//�Q�[�W
class Gauge
{
public:
    Gauge();
    ~Gauge();

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc);

    float gaugeNum = 0;


private:
    ////�ǉ�
    ////�X�e�[�g
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