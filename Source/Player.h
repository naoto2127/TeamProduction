#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"
#include "ProjectileManager.h"

class StageMoveFloor;

//プレイヤー
class Player : public Character
{
public:
    Player();
    ~Player()override;

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void DrawDebugGUI();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //横移動
    DirectX::XMFLOAT3 GetMoveVec() const;

    const DirectX::XMFLOAT3& GetFront() const { return Front; }

    //ジャンプ機能
    void InputJump();

    void ChangeCharacter();

    //移動入力処理
    bool InputMove(float elapsedTime);

    void ItemVsPlayer();

    void PersonVsPlayer();

    void LaunchCola();

    void CollisionProjectilesVsPerson();

    //待機ステートへ遷移
    void TransitionIdleState();

    //待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    //移動ステートへの遷移
    void TransitionMoveState();

    //移動ステート更新処理
    void UpdateMoveState(float elapsedTime);

protected:
    //着地沙汰時に呼ばれる
    void OnLanding() override;

private:
    enum Animation
    {
        Anim_Idle,
        Anim_Walk,
        Anim_Run,

    };
    enum class State
    {
        Idle,
        Walk,
        Running,
    };

private:
    Model* model = nullptr;  
 

    float moveSpeed = 10.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    float jumpSpeed = 20.0f;

    int jumpCount = 0;

    int jumpLimit = 2;

    int CharacterSlot = 0;

    bool character_type[4] = { false,false };

    ProjectileManager projectileManager;

    StageMoveFloor* instanceOfStageMoveFloor;

    Effect* hitEffect = nullptr;

    State state = State::Idle;
    DirectX::XMFLOAT3 Front = {};
};