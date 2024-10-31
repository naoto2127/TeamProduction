#include "Collision.h"


bool Collision::IntersectRayVsModel(
	const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	const Model* model,
	HitResult& result)
{
	// ワールド座標系でのレイの開始点
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
	// ワールド座標系でのレイの終点
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
	// ワールド座標系でのレイのベクトル
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	// ワールド座標系でのレイの長さ
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

	// ワールド空間のレイの長さ
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	// true...当たっている	false...当たっていない
	bool hit = false;
	const ModelResource* resource = model->GetResource();
	// 全てのメッシュとの衝突判定を行う（mesh = fbx(mdl)の中の１つのポリゴンの塊）
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// メッシュノード取得
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

		// レイをワールド空間からローカル空間へ変換
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

		// レイの始点
		DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		// レイの終点
		DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		// レイのベクトル
		DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
		// レイのベクトルを正規化
		DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
		// レイの長さ
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

		// レイの長さ
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		// 三角形（面）との交差判定
		// メッシュがもっているすべての頂点を取得
		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		// メッシュがもっている全てのインデックスを取得
		const std::vector<UINT> indices = mesh.indices;

		int materialIndex = -1;			// 衝突した際のポリゴンの材質
		DirectX::XMVECTOR HitPosition;	// 衝突した際の交点の位置
		DirectX::XMVECTOR HitNormal;	// 衝突した際のポリゴンの法線（向き）
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.indexCount; i += 3)
			{
				UINT index = subset.startIndex + i;

				// 三角形の頂点を抽出
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

				// 三角形の頂点の座標を抽出
				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				// 三角形の三辺のベクトルを算出
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				// 三角形の法線ベクトル（向き）を算出（２辺の外積）
				DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);
				N = DirectX::XMVector3Normalize(N);

				// 内積の結果がプラスならば裏向き（ここで計算終了、つぎのポリゴンへ）
				// レイのベクトルと法線ベクトルの内積を計算
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
				// 内積を単一の値にする
				float d; // = DirectX::XMVectorGetX(Dot);
				DirectX::XMStoreFloat(&d, Dot);
				// プラス以上だったら計算終了
				if (d >= 0) continue;

				// レイと平面の交点を算出
				// レイの始点から三角形の一点へのベクトルを計算
				DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
				// 法線とそのベクトルとの射影を計算
				DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
				float x;
				DirectX::XMStoreFloat(&x, X);
				if (x < .0f || x > neart) continue;

				// 交点までの位置を計算
				DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V, X), S);
				//DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorScale(V, x), S);

				// 交点が三角形の内側にあるか判定
				// １つ目
				DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				// 矢印が逆方向（マイナス）だったら三角形の外側
				if (dot1 < 0.0f) continue;

				// ２つ目
				DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				// 矢印が逆方向（マイナス）だったら三角形の外側
				if (dot2 < 0.0f) continue;

				// ３つ目
				DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				// 矢印が逆方向（マイナス）だったら三角形の外側
				if (dot3 < 0.0f) continue;

				// 最近距離更新
				neart = x;
				// 交点と法線を更新
				HitPosition = P;
				HitNormal = N;
				materialIndex = subset.materialIndex;
			}
		}

		if (materialIndex >= 0)
		{
			// 三角形の交点座標をローカル空間からワールド空間へ変換（戻す）
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
				WorldTransform);

			// ワールド座標上のレイの開始位置から三角形の交点の位置へのベクトル
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			// その長さ
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			// ヒット情報保存
			if (result.distance > distance)
			{
				// 三角形の法線をローカル空間からワールド空間へ変換
				DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal,
					WorldTransform);

				result.distance = distance;
				result.materialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.position, WorldPosition);
				DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
				hit = true;
			}
		}
	}

	return hit;
}
//球体対円柱
bool Collision::IntersectSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius, const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight )
{
	//球の足元が円柱の頭より上なら当たっていない
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)
		return false;
	//球の頭が円柱の足元より下なら当たっていない
	if (cylinderPosition.y > spherePosition.y + sphereRadius)
		return false;

	//XZ平面での範囲チェック
	//X同士を引く
	float vx = cylinderPosition.x - spherePosition.x;

	//z同士を引く
	float vz = cylinderPosition.z - spherePosition.z;
	//XZの長さを計算する
	float distXZ = sqrtf(vx * vx + vz * vz);
	//半径Aと半径Bの長さを計算する
	float range = sphereRadius + cylinderRadius;
	//XZの長さが半径Aと半径Bの長さより大きかったら当たっていない
	if (distXZ > range)
		return false;
	return true;
}

//bool Collision::MentosVsModel(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outPositionB)
//{
//	float Length = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&positionB), DirectX::XMLoadFloat3(&positionA))));
//	float range = radiusA + radiusB;
//	if (Length > range * range)
//		return false;
//	return true;
//}

//円柱と円柱の交差判定
bool Collision::IntersectCylonderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB
)
{
	//Aの足元がBの頭より上なら当たっていない
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}
	//Aの頭がBの足元より下なら当たっていない
	if (positionA.y + heightA < positionB.y)
	{
		return false;

	}
	//XZ平面での範囲チェック
	//X同士を引く
	float vx = positionB.x - positionA.x;

	//Z同士を引く
	float vz = positionB.z - positionA.z;

	//XZの長さを計算する
	float distXZ = sqrtf(vx * vx + vz * vz);

	//半径Aと半径Bの長さを計算する
	float range = radiusA + radiusB;

	//XZの長さが半径Aと半径Bの長さより大きかったら当たってない
	if (distXZ > range)
	{
		return false;
	}
	//AがBを押し出す
	vx /= distXZ;
	vz /= distXZ;

	outPositionB.x = vx * range + positionA.x;
	outPositionB.y = positionB.y;
	outPositionB.z = vz * range + positionA.z;

	return true;

}
