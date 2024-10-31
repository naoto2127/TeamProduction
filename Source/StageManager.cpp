#include "StageManager.h"
#include "Collision.h"

//�X�V����
void StageManager::Update(float elapsedTime)
{
    for (Stage* stage : stages)
    {
        stage->Update(elapsedTime);
    }
}

//�`�揈��
void StageManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Stage* stage : stages)
    {
        stage->Render(context, shader);
    }
}

//�X�e�[�W�o�^
void StageManager::Register(Stage* stage)
{
    stages.emplace_back(stage);
}

//�X�e�[�W�S�폜
void StageManager::Clear()
{
    for (Stage* stage : stages)
    {
        delete stage;
    }
    stages.clear();
}

bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    bool result = false;

    //���ڂ̔���͋����ő�ɂ��Ă����K�v������
    hit.distance = FLT_MAX;

    //�S�ẴX�e�[�W�ƃ��C�L���X�g
    for (Stage* stage : stages)
    {
        //���C�L���X�g
        HitResult tmp;
        if (stage->RayCast(start,end,tmp))
        {
            //�������Ă�����A����̃X�e�[�W���߂����ǂ����`�F�b�N
            if (hit.distance > tmp.distance)
            {
                //����̕����߂�������hit�����㏑����result��true
                hit = tmp;
                result = true;
            }
        }
    }
    return result;
}