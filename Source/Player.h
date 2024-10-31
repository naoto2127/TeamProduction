#pragma once

#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Character.h"
#include"ProjectileManager.h"
#include"Effect.h"

//�v���C���[
class Player :public Character
{
public:
	Player();
	~Player()override;
	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�f�o�b�O�pGUI�`��
	void DrawdebugGUI();

	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	

	//�e�ۂƓG�̏Փˏ���
	void CollisionProjectilesVsEnemies();

private:
	//�A�j���[�V����
	enum Animation
	{
		Anim_Attack,
		Anim_Death,
		Anim_Falling,
		Anim_GetHit1,
		Anim_GetHit2,
		Anim_Idle,
		Anim_jump,
		Anim_Jump_Flip,
		Anim_Landing,
		Anim_Revive,
		Anim_Running,
		Anim_Walking
	};

	//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec()const;	

	//�ړ����͏���
	bool InputMove(float elapsedTime);

	//�W�����v���͏���
	bool InputJump();

	//�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
	void CollisionPlayerVsEnemies();

	//�e�ۓ��͏���
	void InputProjectile();

	//�U�����͏���
	bool InputAttack();

	//�ҋ@�X�e�[�g�֑J��
	void TransitionIdleState();

	//�ҋ@�X�e�[�g�X�V����
	void UpdateIdleState(float elapsedTime);

	//�ړ��X�e�[�g�֑J��
	void TransitionMoveState();

	//�ړ��X�e�[�g�X�V����
	void UpdateMoveState(float elapsedTime);

	//�W�����v�X�e�[�g�֑J��
	void TransitionJumpState();

	//�W�����v�X�e�[�g�X�V����
	void UpdateJumpState(float elapsedTime);

	//���n�X�e�[�g�֑J��
	void TransitionLandState();

	//���n�X�e�[�g�X�V����
	void UpdateLandState(float elapsedTime);

	//�U���X�e�[�g�֑J��
	void TransitionAttackState();

	//�U���X�e�[�g�X�V����
	void UpdateAttackState(float elapsedTime);

protected:
	//���n�����Ƃ��ɌĂ΂��
	void OnLanding()override;

private:
	//�X�e�[�g
	enum class State
	{
		Idle,
		Move,
		Jump,
		Land,
		Attack
	};

private:
	Model* model = nullptr;
	float  moveSpeed = 5.0f;
	float  turnSpeed = DirectX::XMConvertToRadians(720);

	float jumpSpeed = 20.0f;
	
	int jumpCount = 0;
	int jumpLimit = 2;

	ProjectileManager projectileManager;

	Effect* hitEffect = nullptr;

	State state = State::Idle;
};
