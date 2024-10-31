#include "StageManager.h"
#include "Collision.h"

//更新処理
void StageManager::Update(float elapsedTime)
{
    for (Stage* stage : stages)
    {
        stage->Update(elapsedTime);
    }
}

//描画処理
void StageManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Stage* stage : stages)
    {
        stage->Render(context, shader);
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
    for (Stage* stage : stages)
    {
        delete stage;
    }
    stages.clear();
}

bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    bool result = false;

    //一回目の判定は距離最大にしておく必要がある
    hit.distance = FLT_MAX;

    //全てのステージとレイキャスト
    for (Stage* stage : stages)
    {
        //レイキャスト
        HitResult tmp;
        if (stage->RayCast(start,end,tmp))
        {
            //当たっていたら、今回のステージが近いかどうかチェック
            if (hit.distance > tmp.distance)
            {
                //今回の方が近かったらhit情報を上書きしresultにtrue
                hit = tmp;
                result = true;
            }
        }
    }
    return result;
}