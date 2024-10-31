#include"StageMoveFloor.h"

//コンストラクタ
StageMoveFloor::StageMoveFloor()
{
	scale.x = scale.z = 3.0f;
	scale.y = 0.5f;

	//ステージモデルを読み込み
	model = new Model("Data/Model/Cube/Cube.mdl");


}

StageMoveFloor::~StageMoveFloor()
{
	//ステージモデルを破棄
	delete model;
}

//更新処理
void StageMoveFloor::Update(float elapsedTime)
{

	//前回の情報を保存
	oldTransform = transform;
	oldAngle = angle;

	//スタートからゴールまでの距離を算出する
	DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
	DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
	float length;
	DirectX::XMStoreFloat(&length, Length);

	//スタートからゴールまでの間をいて秒間で進む割合(0.0~1.0)を算出する
	float speed = moveSpeed * elapsedTime;
	float speedRate = speed / length;
	moveRate += speedRate;

	//ゴールの到達、またはスタートに戻った場合、移動方向を反転させる
	if (moveRate <= 0.0f || moveRate >= 1.0f)
	{
		moveSpeed = -moveSpeed;
	}

	//線形補完で位置を算出する
	DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
	DirectX::XMStoreFloat3(&position, Position);

	//回転
	angle.x += torque.x * elapsedTime;
	angle.y += torque.y * elapsedTime;
	angle.z += torque.z * elapsedTime;


	//回転行列
	UpdateTransfotm();

	//モデル行列更新
	//model->UpdateTransform(transform);
	//レイキャスト用にもです空間行列のするため単位行列を渡す
	const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	model->UpdateTransform(transformIdentity);


}

//描画処理
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//表示用のためワールド行列に更新する
	model->UpdateTransform(transform);


	shader->Draw(dc, model);
}

//レイキャスト
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	//return Collision::IntersectRayVsModel(start, end, model, hit);


	//前回のワールド行列と逆行列を求める
	DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&oldTransform);
	DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);


	//前回のローカル空間(oldTransformがローカル空間になる)でのレイに変換
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);//レイの始点
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);//レイの終点
	//レイの始点をワールド空間からoldTransformの空間に変換
	DirectX::XMVECTOR LocalStart = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
	//レイの終点をワールド空間からoldTransformの空間に変換
	DirectX::XMVECTOR LocalEnd = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);

	//ローカル空間でのレイとの交点を求める
	DirectX::XMFLOAT3 localStart, localEnd;
	DirectX::XMStoreFloat3(&localStart, LocalStart);
	DirectX::XMStoreFloat3(&localEnd, LocalEnd);
    
	HitResult localhit;
	if (Collision::IntersectRayVsModel(localStart, localEnd, model, localhit))
	{
		//前回のローカル空間から今回のワールド空間へ変換
		//前回から今回にかけて変換された内容が乗っているオブジェクトに反映される
		//WorldTransformを今回の移動床の行列を上書き
		WorldTransform = DirectX::XMLoadFloat4x4(&transform);
		//ヒットした移動床の位置(localHit.position)を取得
		DirectX::XMVECTOR LocalPosition = DirectX::XMLoadFloat3(&localhit.position);
		//ヒットした位置をローカル空間（移動床）からワールド空間へ変換
		DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(LocalPosition, WorldTransform);
		//ヒットした移動床の向き（法線ベクトル）を取得
		DirectX::XMVECTOR LocalNormal = DirectX::XMLoadFloat3(&localhit.normal);
		//ヒットした向きをローカル空間からワールド空間へ変換
		DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(LocalNormal, WorldTransform);
		//ワールド空間上でレイの開始位置からヒット位置までの距離ベクトル
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
		//その長さ
		DirectX::XMVECTOR Dist = DirectX::XMVector3Length(Vec);
		//ワールドのヒットした位置をhit.positionへ
		DirectX::XMStoreFloat3(&hit.position, WorldPosition);
		//ワールドのヒットした向きをhit.normalへ
		DirectX::XMStoreFloat3(&hit.normal, WorldNormal);
		//長さをhit.distanceへ
		DirectX::XMStoreFloat(&hit.distance, Dist);

		//ヒットしたマテリアルインデックスをhit.materialIndexへ
		hit.materialIndex = localhit.materialIndex;

		//回転差分を算出
		hit.rotation.x = angle.x - oldAngle.x;
		hit.rotation.y = angle.y - oldAngle.y;
		hit.rotation.z = angle.z - oldAngle.z;

		return true;
	}
	return false;
}

//行列更新処理
void StageMoveFloor::UpdateTransfotm()
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
}