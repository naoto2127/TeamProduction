#pragma once

#include"Graphics/Model.h"
#include"Projectile.h"

//íºêiíeä€
class ProjectileStraight :public Projectile
{
public:
	ProjectileStraight(ProjectileManager* manager);
	~ProjectileStraight()override;

	//çXêVèàóù
	void Update(float elapsedTime)override;

	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//î≠éÀ
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
	Model* model = nullptr;
	float speed = 10.0f;

	float LifeTimer = 2.0f;

};
