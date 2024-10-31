#pragma once

#include <vector>
#include "liquid.h"
#include <set>

//弾丸マネージャー
class LiquidManager
{
public:
    LiquidManager();
    ~LiquidManager();

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc,  const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection,
        const DirectX::XMFLOAT3 Pos, const DirectX::XMFLOAT3 Front);

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //弾丸登録
    void Register(Liquid* projectile);

    //弾丸全削除
    void Clear();

    //弾丸数取得
    int GetACount() const { return static_cast<int>(projectiles.size()); }

    //弾丸取得
    Liquid* GetA(int index) { return projectiles.at(index); }

    void Remove(Liquid* projectile);

private:
    std::vector<Liquid*>  projectiles;

    std::set<Liquid*> removes;
};