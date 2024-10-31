#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"

//�q�b�g����
struct HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 };//���C�ƃ|���S���̌�_
    DirectX::XMFLOAT3 normal = { 0,0,0 };//�Փ˂����|���S���̖@���x�N�g��
    float distance = 0.0f;//���C�̎n�_�����_�܂ł̋���
    int materialIndex = -1;//�Փ˂����|���S���̃}�e���A���ԍ�
};

//�R���W����
class Collision
{
public:
    /// <summary>
    /// ���C�ƃ��f���̌�������
    /// </summary>
    /// <param name="start">���C�̎n�_</param>
    /// <param name="end">���C�̏I�_</param>
    /// <param name="model">�Փː�̃��f���i�|���S���̏W���́j</param>
    /// <param name="result">���ʂ̃q�b�g���U���g</param>
    /// <returns>����</returns>
    static bool IntersectRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
    int a;

    //���Ɖ~���̌�������
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight
     );

    //�~���Ɖ~���̌�������
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