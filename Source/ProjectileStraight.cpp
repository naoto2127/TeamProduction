#include"ProjectileStraight.h"

#include"StageManager.h"

//コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager*manager)
	:Projectile(manager)
{

	model = new Model("Data/Model/Sword/Sword.mdl");

	//表示サイズを調整
	scale.x = scale.y = scale.z = 3.0f;
}

//デストラクタ
ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

//更新処理
void ProjectileStraight::Update(float elapsedTime)
{
	//寿命処理
	LifeTimer -= elapsedTime;
	if (LifeTimer<=0)
	{
		//自分を削除
		Destroy();
	}

	//移動
	float speed = this->speed * elapsedTime;

#if false
	//位置＋＝前方向*速さ
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;
#endif
	float mx = direction.x * speed;
	float my = direction.y * speed;
	float mz = direction.z * speed;

	//反射の処理
	{
		//レイの開始位置と終点位置
		DirectX::XMFLOAT3 start = { position.x,position.y,position.z };
		DirectX::XMFLOAT3 end = { position.x + mx,position.y + my,position.z + mz };

		//レイキャストによる壁の判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			//壁までのベクトル
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);//壁の衝突位置
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);//レイの終点位置
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Start,End);//壁の衝突位置までのベクトル

			//壁の法線ベクトル
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//壁の衝突位置までのベクトルを法線に射影
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);
			float dot = DirectX::XMVectorGetX(Dot);//射影の単一の値

			//壁の法線ベクトルをdot　の2倍分スケーリングし(XMVectorScale)
			DirectX::XMVECTOR CollectPosition =DirectX::XMVectorScale(Normal,dot*2.0f);
			//Endの位置からCollectPosition分のベクトルを足す
			DirectX::XMVECTOR ReflectPosition = DirectX::XMVectorAdd(End,CollectPosition);

			//ReflectPosition引くhit.positionで反射ベクトルを計算
			DirectX::XMVECTOR ReflectVec = DirectX::XMVectorSubtract(ReflectPosition,DirectX::XMLoadFloat3(&hit.position));
			//反射ベクトル正規化
			ReflectVec = DirectX::XMVector3Normalize(ReflectVec);

			//floatに変換
			DirectX::XMFLOAT3 reflectVec;
			DirectX::XMStoreFloat3(&reflectVec, ReflectVec);

			//位置のxとzの成分をhit.positionに上書き
			position.x = hit.position.x;
			position.z = hit.position.z;

			///directionのxとzの成分を上書き
			direction.x = reflectVec.x;
			direction.z = reflectVec.z;

		}
		else
		{
			position.x += mx;
			position.z += mz;
		}
	}


	//オブジェクト行列を更新
	UpdateTransform();


	//モデル行列更新処理
	model->UpdateTransform(transform);


}

//描画処理
void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

//発射
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}
