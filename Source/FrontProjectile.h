#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"

//���i�e��
class FrontProjectile :public Projectile
{
public:
    //ProjectileStraight();
    FrontProjectile(ProjectileManager* manager);
    ~FrontProjectile() override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
   
    float   speed = 10.0f;
    float   lifeTimer = 2.0f;
};