#pragma once

#include "Graphics/Model.h"
#include "liquid.h"
#include <Graphics/Sprite.h>

//直進弾丸
class Cola :public Liquid
{
public:
    //AStraight();
    Cola(LiquidManager* manager);
    ~Cola() override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc,  const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection,
        const DirectX::XMFLOAT3 Pos,
        const DirectX::XMFLOAT3 Front)override;

    //発射
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

    //コーラ描画
    void RenderCola(
        ID3D11DeviceContext* dc,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection,
        const DirectX::XMFLOAT3 Pos,
        const DirectX::XMFLOAT3 Front);

private:
    
    float   speed = 10.0f;
    float   lifeTimer = 2.0f;

    float colaAngle = 0.0f;

    Sprite* cola = nullptr;
    float colaNum = 1.0f;
};