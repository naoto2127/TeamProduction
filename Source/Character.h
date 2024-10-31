#pragma once

#include <DirectXMath.h>

//�L�����N�^�[
class Character
{
public:
    Character() {}
    virtual ~Character() {}

    //�s���X�V����
    void UpdateTransform();

    //�ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    //�ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
    //��]�擾
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
    //��]�ݒ�  
    const DirectX::XMFLOAT3& GetScale() const { return scale; }
    //�X�P�[���擾
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //���a�擾
    float GetRadius() const { return radius; }

    float GetHeight() const { return height; }

    //�n�ʐڒn
    bool IsGround() const { return isGround; }

    //�Ռ���^����
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    bool ApplyDamage(int damage, float invincibleTime);

    int GetHealth()const { return health; }

    int GetMaxHealth() const { return maxHealth; }

private :
    //�������͍X�V����
    void UpdateVerticalVelocity(float elapsedFrame);

    void UpdateverticalMove(float elapsedTime);
    
    //�������͍X�V����
    void UpdateHorizontalVelocity(float elapsedFrame);
    
    //�����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);

protected:
    //�ړ�����
    /*void Move(float elapsedTime, float vx, float vz, float speed);*/
    void Move(float vx, float vz, float speed);

    //���񏈗�
    void Turn(float elapsedTime, float vx, float vz, float speed);

    void Jump(float speed);

    void UpdateVelocity(float elapsedTime);

    void UpdateInvincibleTimer(float elapsedTime);

    virtual void OnLanding(){}

    virtual void OnDamaged(){}

    virtual void OnDead(){}

    DirectX::XMFLOAT3       position = { 0,0,0 };
    DirectX::XMFLOAT3       angle = { 0,0,0 };
    DirectX::XMFLOAT3       scale = { 1,1,1 };
    DirectX::XMFLOAT4X4       transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1,
    };

    int maxHealth = 1;

    float radius = 0.5f;
    float gravity = -1.0f;
    DirectX::XMFLOAT3 velocity = { 0,0,0 }; //���́E���x�x�N�g��
    bool isGround = false;
    float height = 2.0f;
    int health = 1;
    float invincibleTimer = 1.0f;
    float friction = 0.5f; //���C�W��
    float acceleration = 1.0f;  //�����W��
    float maxMoveSpeed = 5.0f;  
    float moveVecX = 0.0f;
    float moveVecZ = 0.0f;
    float airControl = 0.3f;
    float stepOffset = 1.0f;
    float slopeRate = 1.0f;
};