#pragma once

#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Character.h"
#include"ProjectileManager.h"
#include"Effect.h"

//プレイヤー
class Player :public Character
{
public:
	Player();
	~Player()override;
	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//デバッグ用GUI描画
	void DrawdebugGUI();

	//デバッグプリミティブ描画
	void DrawDebugPrimitive();

	

	//弾丸と敵の衝突処理
	void CollisionProjectilesVsEnemies();

private:
	//アニメーション
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

	//スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec()const;	

	//移動入力処理
	bool InputMove(float elapsedTime);

	//ジャンプ入力処理
	bool InputJump();

	//プレイヤーとエネミーとの衝突処理
	void CollisionPlayerVsEnemies();

	//弾丸入力処理
	void InputProjectile();

	//攻撃入力処理
	bool InputAttack();

	//待機ステートへ遷移
	void TransitionIdleState();

	//待機ステート更新処理
	void UpdateIdleState(float elapsedTime);

	//移動ステートへ遷移
	void TransitionMoveState();

	//移動ステート更新処理
	void UpdateMoveState(float elapsedTime);

	//ジャンプステートへ遷移
	void TransitionJumpState();

	//ジャンプステート更新処理
	void UpdateJumpState(float elapsedTime);

	//着地ステートへ遷移
	void TransitionLandState();

	//着地ステート更新処理
	void UpdateLandState(float elapsedTime);

	//攻撃ステートへ遷移
	void TransitionAttackState();

	//攻撃ステート更新処理
	void UpdateAttackState(float elapsedTime);

protected:
	//着地したときに呼ばれる
	void OnLanding()override;

private:
	//ステート
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
