#pragma once

#include"Graphics/Model.h"
#include "item.h"

//スライム
class Mentos :public Item
{
public:
	Mentos();
	~Mentos()override;

	//更新処理
	void Update(float elapsedTime)override;

	//描画処置
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//発射
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

protected:
	//死亡したときに呼ばれる
	void OnDead()override;

private:
	Model* model = nullptr;

	float speed = 5.0f;


};
