#include"item.h"
#include"Graphics/Graphics.h"
#include"itemManager.h"

//デバッグプリミティブ描画
void Item::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();


	//衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

//破棄
void Item::Destroy()
{
	//敵マネジャーの削除に自分自身のアドレスを渡す
	ItemManager::Instance().Remove(this);
}