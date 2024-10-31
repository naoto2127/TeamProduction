#include "liquidManager.h"

//コンストラクタ
LiquidManager::LiquidManager()
{
}

//デストラクタ
LiquidManager::~LiquidManager()
{
    Clear();
}

//更新処理
void LiquidManager::Update(float elapsedTime)
{
    //更新処理
    for (Liquid* projectile : projectiles)
        projectile->Update(elapsedTime);

    //破棄処理
    //※projectilesの範囲for文中でerase()すると不具合が発生してしまうため、
    //　更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する
    for (Liquid* projectile : removes)
    {
        //std::vectorから要素を削除する場合はイテレーターで削除しなければならない
        std::vector<Liquid*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);

        if (it != projectiles.end())
        {
            projectiles.erase(it);
        }
        //弾丸破棄処理
        delete projectile;
    }
    //破棄リストをクリア
    removes.clear();
}

//描画処理
void LiquidManager::Render(ID3D11DeviceContext* context,  const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection,
    const DirectX::XMFLOAT3 Pos, const DirectX::XMFLOAT3 Front)
{
    for (Liquid* projectile : projectiles)
        projectile->Render(context,   view,
            projection,
             Pos,Front);
}

//デバッグプリミティブ描画
void LiquidManager::DrawDebugPrimitive()
{
    for (Liquid* projectile : projectiles)
        projectile->DrawDebugPrimitive();
}

//弾丸登録
void LiquidManager::Register(Liquid* projectile)
{
    projectiles.emplace_back(projectile);
}

//弾丸全削除
void LiquidManager::Clear()
{
    for (Liquid* projectile : projectiles)
    {
        delete projectile;//箱だけ消えてる
    }
    projectiles.clear();//箱消す

}

//弾丸削除
void LiquidManager::Remove(Liquid* projectile)
{
    //破棄リストに追加
    removes.insert(projectile);
}