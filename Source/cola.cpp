#include "cola.h"
#include <Collision.h>
#include "StageManager.h"


//コンストラクタ
Cola::Cola(LiquidManager* manager) :Liquid(manager)
{
    //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //表示サイズを調整
    //scale.x = scale.y = scale.z = 0.5f;
    scale.x = scale.y = scale.z = 3.0f;
	cola = new Sprite("Data/Sprite/cola.png");
}

//デストラクタ
Cola::~Cola()
{
    //delete model;
	if (cola != nullptr)
	{
		delete cola;
		cola = nullptr;
	}
}

//更新処理
void Cola::Update(float elapsedTime)
{
    //寿命処理
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        //自分を削除
        Destroy();
    }

    //移動
    float speed = this->speed * elapsedTime;
#if 1
    ////位置+=前方向*速さ
    //position.x += direction.x * speed;
    //position.y += direction.y * speed;
    //position.z += direction.z * speed;
#endif

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    //model->UpdateTransform(transform);

	if (colaNum <= 11)
		colaNum += 0.1f;
}

//描画処理
void Cola::Render(ID3D11DeviceContext* dc,const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection,
    const DirectX::XMFLOAT3 Pos, const DirectX::XMFLOAT3 Front)
{
    //shader->Draw(dc, model);
	RenderCola(
		dc,
		view,
		projection,
		Pos,Front);
}

//発射
void Cola::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}


//コーラ描画
void Cola::RenderCola(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection,
	const DirectX::XMFLOAT3 Pos, const DirectX::XMFLOAT3 Front
	)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//プレイヤーの場所にコーラを表示
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//int enemyCount = enemyManager.GetEnemyCount();

	/*for (int i = 0; i < enemyCount; ++i)
	{*/
	//Enemy* enemy = enemyManager.GetEnemy(i);

	//プレイヤーのワールド座標
	DirectX::XMFLOAT3 worldPosition = Pos;
	//DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
	// 
	worldPosition.x -= 1.3f;
	//yの位置だけ高さ分足す
	//worldPosition.y += 3.5f;

	if (Front.z > 0)
	{
		worldPosition.y += 3.5f;
		colaAngle = 0.0f;

	}
	else if (Front.z < 0)
	{
		worldPosition.y += 1.5f;
		colaAngle = 180.0f;
	}
	else
	{
		worldPosition.y += 3.5f;
		colaAngle = 0.0f;
	}
	//プレイヤーの位置をXMVECTORに変換
	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);
	//DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);

	//ワールド座標(3D)からスクリーン座標(2D)へ変換
	DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(WorldPosition, viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height,
		viewport.MinDepth, viewport.MaxDepth, Projection, View, World);

	//スクリーン座標(3Dから2DにしたXMVECTORをXMFLOAT2に変換)
	DirectX::XMFLOAT2 screenPosition;
	DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

	//ゲージのサイズ
	const float gaugeWidth = 30.0f;
	const float gaugeHeight = 5.0f;

	//ゲージの長さを計算(健康状態比率)（現在健康状態/最大健康状態）
	//float healthRate = static_cast<float>(enemy->GetHealth()) / static_cast<float>(enemy->GetMaxHealth());

	//ゲージ描画
	cola->Render(dc,
		screenPosition.x,//描画の左位置（敵のｘ位置-ゲージ幅の半分）
		screenPosition.y,//描画の上位置（敵のｙ位置-ゲージの高さ）
		300,//Spriteの幅（ゲージの幅*健康状態比率)
		300,//Spriteの高さ（ゲージの高さ）
		640 * static_cast<int>(colaNum), 0,//Textureの切り取り位置XY
		640,//Textureの切り取り幅
		480,//Textureの切り取り高さ
		static_cast<int>(colaAngle),//Spriteの角度
		1, 1, 1, 1//Spriteの色(r,g,b,a)
	);



}
