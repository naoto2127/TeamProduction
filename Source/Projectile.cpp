#include "Projectile.h"
#include "ProjectileManager.h"
#include "Graphics/Graphics.h"

//�R���X�g���N�^
Projectile::Projectile(ProjectileManager* manager) :manager(manager)
{
    manager->Register(this);
}

//�f�o�b�O�v���~�e�B�u�`��
void Projectile::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�O����`��
   // debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));


}

//�s��X�V����
void Projectile::UpdateTransform()
{


    DirectX::XMVECTOR Front, Up, Right;

    //�O�x�N�g��
    Front = DirectX::XMLoadFloat3(&direction);
    //���K������
    Front = DirectX::XMVector3Normalize(Front);

    //���̏�x�N�g�����Z�o
    Up = DirectX::XMVectorSet(0, 1, 0, 0);
    //���K������ 
    Up = DirectX::XMVector3Normalize(Up);

    //�E�x�N�g�����Z�o
    Right = DirectX::XMVector3Cross(Up, Front);
    //���K��
    Right = DirectX::XMVector3Normalize(Right);

    //��x�N�g�����Z�o
    Up = DirectX::XMVector3Cross(Front, Right);

    //�v�Z���ʂ����o��
    DirectX::XMFLOAT3 right, up, front;
    DirectX::XMStoreFloat3(&right, Right);
    DirectX::XMStoreFloat3(&up, Up);
    DirectX::XMStoreFloat3(&front, Front);


    //�Z�o�������x�N�g������s����쐬
    transform._11 = right.x * scale.x;
    transform._12 = right.y * scale.x;
    transform._13 = right.z * scale.x;
    transform._14 = 0.0f;

    transform._21 = up.x * scale.y;
    transform._22 = up.y * scale.y;
    transform._23 = up.z * scale.y;
    transform._24 = 0.0f;

    transform._31 = front.x * scale.z;
    transform._32 = front.y * scale.z;
    transform._33 = front.z * scale.z;
    transform._34 = 0.0f;

    transform._41 = position.x;
    transform._42 = position.y;
    transform._43 = position.z;
    transform._44 = 1.0f;

    //���˕���
    this->direction = front;
}

//�j��
void Projectile::Destroy()
{
    manager->Remove(this);
}