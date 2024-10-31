#pragma once

#include <DirectXMath.h>

//キャラクター
class Character
{
public:
    Character() {}
    virtual ~Character() {}

    //行動更新処理
    void UpdateTransform();

    //位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    //位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
    //回転取得
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
    //回転設定  
    const DirectX::XMFLOAT3& GetScale() const { return scale; }
    //スケール取得
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //半径取得
    float GetRadius() const { return radius; }

    float GetHeight() const { return height; }

    //地面接地
    bool IsGround() const { return isGround; }

    //衝撃を与える
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    bool ApplyDamage(int damage, float invincibleTime);

    int GetHealth()const { return health; }

    int GetMaxHealth() const { return maxHealth; }

private :
    //垂直速力更新処理
    void UpdateVerticalVelocity(float elapsedFrame);

    void UpdateverticalMove(float elapsedTime);
    
    //水平速力更新処理
    void UpdateHorizontalVelocity(float elapsedFrame);
    
    //水平移動更新処理
    void UpdateHorizontalMove(float elapsedTime);

protected:
    //移動処理
    /*void Move(float elapsedTime, float vx, float vz, float speed);*/
    void Move(float vx, float vz, float speed);

    //旋回処理
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
    DirectX::XMFLOAT3 velocity = { 0,0,0 }; //速力・速度ベクトル
    bool isGround = false;
    float height = 2.0f;
    int health = 1;
    float invincibleTimer = 1.0f;
    float friction = 0.5f; //摩擦係数
    float acceleration = 1.0f;  //加速係数
    float maxMoveSpeed = 5.0f;  
    float moveVecX = 0.0f;
    float moveVecZ = 0.0f;
    float airControl = 0.3f;
    float stepOffset = 1.0f;
    float slopeRate = 1.0f;
};