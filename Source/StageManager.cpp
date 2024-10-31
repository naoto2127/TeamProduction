#include"StageManager.h"

//更新処理
void StageManager::Update(float elapsedTime)
{
	for (Stage* stage : stages)
	{
		stage->Update(elapsedTime);
	}


	
}

//描画処理
void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{

	for (Stage* stage : stages)
	{
		stage->Render(dc, shader);
	}
}

//ステージ登録
void StageManager::Register(Stage* stage)
{
	stages.emplace_back(stage);
}


//ステージ全削除
void StageManager::Clear()
{
	//全てのステージを解放し
	for (Stage* stage : stages)
	{
		delete stage;
	}

	

	//イテレーターをクリアする
	stages.clear();
}

//レイキャスト
bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	bool result = false;

	//1回目の判定は距離最大にしておく必要がある
	hit.distance = FLT_MAX;

	//全てのステージと
	for (Stage*stage:stages)
	{
		//レイキャスト
		HitResult tmp;
		if (stage->RayCast(start,end,tmp))
		{
			//当たっていたら、今回のステージの方が近いかどうかチェック
			if (hit.distance>tmp.distance)
			{
				//今回のほうが近かったらhit情報を上書きしresultにtrue
				hit = tmp;
				result = true;
			}
		}
	}
	return result;
}