#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"

//ヒット結果
struct HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 };//レイとポリゴンの交点
    DirectX::XMFLOAT3 normal = { 0,0,0 };//衝突したポリゴンの法線ベクトル
    float distance = 0.0f;//レイの始点から交点までの距離
    int materialIndex = -1;//衝突したポリゴンのマテリアル番号
};

//コリジョン
class Collision
{
public:
    /// <summary>
    /// レイとモデルの交差判定
    /// </summary>
    /// <param name="start">レイの始点</param>
    /// <param name="end">レイの終点</param>
    /// <param name="model">衝突先のモデル（ポリゴンの集合体）</param>
    /// <param name="result">結果のヒットリザルト</param>
    /// <returns>結果</returns>
    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
    int a;

    //球と円柱の交差判定
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight
     );

    //円柱と円柱の交差判定
    static bool IntersectCylonderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB
    );
};