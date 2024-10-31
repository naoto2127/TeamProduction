#include "StageMain.h"

//static Stage* instance = nullptr;

//�C���X�^���X�擾
//Stage& Stage::Instance()
//{
//    return *instance;
//}

//�R���X�g���N�^
StageMain::StageMain()
{
    //instance = this;

    //�X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/stage/stage5.mdl");
}

StageMain::~StageMain()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void StageMain::Update(float elapsedTime)
{
    //���͓��ɖ���
}

//�`�揈��
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

//���C�L���X�g
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}