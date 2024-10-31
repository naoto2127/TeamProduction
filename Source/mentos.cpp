#include"mentos.h"

//コンストラクタ
Mentos::Mentos()
{
	model = new Model("Data/Model/mentos/mentos.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.012f;

	//幅、高さ設定
	radius = 0.5f;
	height = 2.0f;
}

Mentos::~Mentos()
{
	delete model;
}

//更新処理
void Mentos::Update(float elapsedTime)
{
	//速力処理更新
	UpdateVelocity(elapsedTime);

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model->UpdateTransform(transform);

	float speed = this->speed * elapsedTime;


	
	
	

}

//描画処理
void Mentos::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void Mentos::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->position = position;
}

//死亡したときに呼ばれる
void Mentos::OnDead()
{
	//自分自身を削除
	Destroy();
}