#include "liquidManager.h"

//�R���X�g���N�^
LiquidManager::LiquidManager()
{
}

//�f�X�g���N�^
LiquidManager::~LiquidManager()
{
    Clear();
}

//�X�V����
void LiquidManager::Update(float elapsedTime)
{
    //�X�V����
    for (Liquid* projectile : projectiles)
        projectile->Update(elapsedTime);

    //�j������
    //��projectiles�͈̔�for������erase()����ƕs����������Ă��܂����߁A
    //�@�X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����
    for (Liquid* projectile : removes)
    {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Liquid*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);

        if (it != projectiles.end())
        {
            projectiles.erase(it);
        }
        //�e�۔j������
        delete projectile;
    }
    //�j�����X�g���N���A
    removes.clear();
}

//�`�揈��
void LiquidManager::Render(ID3D11DeviceContext* context,  const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection,
    const DirectX::XMFLOAT3 Pos, const DirectX::XMFLOAT3 Front)
{
    for (Liquid* projectile : projectiles)
        projectile->Render(context,   view,
            projection,
             Pos,Front);
}

//�f�o�b�O�v���~�e�B�u�`��
void LiquidManager::DrawDebugPrimitive()
{
    for (Liquid* projectile : projectiles)
        projectile->DrawDebugPrimitive();
}

//�e�ۓo�^
void LiquidManager::Register(Liquid* projectile)
{
    projectiles.emplace_back(projectile);
}

//�e�ۑS�폜
void LiquidManager::Clear()
{
    for (Liquid* projectile : projectiles)
    {
        delete projectile;//�����������Ă�
    }
    projectiles.clear();//������

}

//�e�ۍ폜
void LiquidManager::Remove(Liquid* projectile)
{
    //�j�����X�g�ɒǉ�
    removes.insert(projectile);
}