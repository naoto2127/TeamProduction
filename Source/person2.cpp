#include"person2.h"

//コンストラクタ
Person2::Person2()
{
	model = new Model("Data/Model/chara/person1.mdl");

	//待機ステートへ遷移
	TransitionIdleState();

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.012f;
	angle.y = 160.0f;
	//幅、高さ設定
	radius = 0.5f;
	height = 2.0f;
}

Person2::~Person2()
{
	delete model;
}

//更新処理
void Person2::Update(float elapsedTime)
{
	//速力処理更新
	UpdateVelocity(elapsedTime);

	//ステート毎の処理
	switch (state)
	{
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;
	case State::Death:
		UpdateDeathState(elapsedTime);
		break;
	}

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	//モデル行列更新
	model->UpdateTransform(transform);

	float speed = this->speed * elapsedTime;


	//位置＋＝前方向*速さ

	position.z -= speed;

}

//描画処理
void Person2::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void Person2::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->position = position;
}

void Person2::TransitionIdleState()
{
	state = State::Idle;

	//待機アニメーション再生
	model->PlayAnimation(Anim_Walk, true);
}

void Person2::UpdateIdleState(float elapsedTime)
{

}

void Person2::TrasitionDeathState()
{
	state = State::Death;

	//待機アニメーション再生
	model->PlayAnimation(Anim_Death, false);
}

void Person2::UpdateDeathState(float elapsedTime)
{
	//モデル破壊
	if (!model->IsPlayAnimation())
		Destroy();
}

//死亡したときに呼ばれる
void Person2::OnDead()
{
	//死亡アニメーション呼び出し
	TrasitionDeathState();
}