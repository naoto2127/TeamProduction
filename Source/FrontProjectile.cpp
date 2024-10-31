#include "FrontProjectile.h"
#include <Collision.h>
#include "StageManager.h"


//�R���X�g���N�^
FrontProjectile::FrontProjectile(ProjectileManager* manager) :Projectile(manager)
{
 
    scale.x = scale.y = scale.z = 3.0f;
}

//�f�X�g���N�^
FrontProjectile::~FrontProjectile()
{
    //delete model;
}

//�X�V����
void FrontProjectile::Update(float elapsedTime)
{
    //��������
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        //�������폜
        Destroy();
    }

    //�ړ�
    float speed = this->speed * elapsedTime;
#if 1
    ////�ʒu+=�O����*����
    //position.x += direction.x * speed;
    //position.y += direction.y * speed;
    //position.z += direction.z * speed;
#endif

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    //model->UpdateTransform(transform);
}

//�`�揈��
void FrontProjectile::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //shader->Draw(dc, model);
}

//����
void FrontProjectile::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}

