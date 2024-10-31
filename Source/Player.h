#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"
#include "ProjectileManager.h"

class StageMoveFloor;

//�v���C���[
class Player : public Character
{
public:
    Player();
    ~Player()override;

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void DrawDebugGUI();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //���ړ�
    DirectX::XMFLOAT3 GetMoveVec() const;

    const DirectX::XMFLOAT3& GetFront() const { return Front; }

    //�W�����v�@�\
    void InputJump();

    void ChangeCharacter();

    //�ړ����͏���
    bool InputMove(float elapsedTime);

    void ItemVsPlayer();

    void PersonVsPlayer();

    void LaunchCola();

    void CollisionProjectilesVsPerson();

    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    //�ړ��X�e�[�g�ւ̑J��
    void TransitionMoveState();

    //�ړ��X�e�[�g�X�V����
    void UpdateMoveState(float elapsedTime);

protected:
    //���n�������ɌĂ΂��
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