#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

//人
class People :public Character
{
public:
	People() {}
	~People() {}

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	//破棄
	void Destroy();

};
