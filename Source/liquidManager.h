#pragma once

#include <vector>
#include "liquid.h"
#include <set>

//�e�ۃ}�l�[�W���[
class LiquidManager
{
public:
    LiquidManager();
    ~LiquidManager();

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc,  const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection,
        const DirectX::XMFLOAT3 Pos, const DirectX::XMFLOAT3 Front);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�e�ۓo�^
    void Register(Liquid* projectile);

    //�e�ۑS�폜
    void Clear();

    //�e�ې��擾
    int GetACount() const { return static_cast<int>(projectiles.size()); }

    //�e�ێ擾
    Liquid* GetA(int index) { return projectiles.at(index); }

    void Remove(Liquid* projectile);

private:
    std::vector<Liquid*>  projectiles;

    std::set<Liquid*> removes;
};