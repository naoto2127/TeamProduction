#pragma once

#include "Graphics/Shader.h"

//前方方向
class LiquidManager;

//弾丸
class Liquid
{
public:
    //A() {}
    Liquid(LiquidManager* manager);
    virtual ~Liquid() {}

    //更新処理
    virtual void Update(float elapsedTime) = 0;

    //描画処理
    virtual void Render(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection,
        const DirectX::XMFLOAT3 Pos, const DirectX::XMFLOAT3 Front) = 0;

    //デバッグプリミティブ描画
    virtual void DrawDebugPrimitive();

    //位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    //方向取得
    const DirectX::XMFLOAT3& GetDirecton() const { return direction; }

    //スケール取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    float GetRadius() const { return radius; }

    //破棄
    void Destroy();

protected:
    void UpdateTransform();

protected:
    DirectX::XMFLOAT3   position = { 0,0,0 };
    DirectX::XMFLOAT3   direction = { 0,0,1 };
    DirectX::XMFLOAT3   scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

    LiquidManager* manager = nullptr;

    float radius = 0.5f;
};