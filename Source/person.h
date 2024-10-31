#pragma once

#include"Graphics/Model.h"
#include "people.h"

//スライム
class Person :public People
{
public:
	Person();
	~Person()override;

	//更新処理
	void Update(float elapsedTime)override;

	//描画処置
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//発射
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

	//待機ステートへ遷移
	void TransitionIdleState();

	//待機ステート更新処理
	void UpdateIdleState(float elapsedTime);

	//死亡ステート遷移
	void TrasitionDeathState();

	//待機ステート更新処理
	void UpdateDeathState(float elapsedTime);

protected:
	//死亡したときに呼ばれる
	void OnDead()override;

private:

	enum Animation
	{
		Anim_Walk,
		Anim_Death

	};
	enum class State
	{
		Idle,
		Death
	};

private:
	Model* model = nullptr;

	float speed = 1.0f;

	State state = State::Idle;

};
