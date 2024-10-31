#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include"Camera.h"
#include"EnemyManager.h"
#include"EnemySlime.h"
#include"EffectManager.h"
#include"Input/Input.h"
#include"StageManager.h"
#include"StageMain.h"
#include"StageMoveFloor.h"


// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(0, 1, 3));
	stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
	stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
	stageManager.Register(stageMoveFloor);

	//プレイヤーの初期化
	player = new Player();

	//カメラ初期設定
	Graphics& grahics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		grahics.GetScreenWidth() / grahics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	//カメラアコントロール初期化
	cameraController = new CameraController();

	//エネミー初期化
	// エネミーマネージャーのインスタンスを用意しておく
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 2; ++i)
	{
		//スライムを生成する
		EnemySlime* slime = new EnemySlime();
		//エネミースライムの位置を設定(0,0,5)
		slime->setPosition(DirectX::XMFLOAT3(i*2.0f,0,5));
		//生成したエネミースライムをエネミーマネージャーに登録する
		enemyManager.Register(slime);
	}

	//ゲージスプライト
	gauge = new Sprite();
	
}

// 終了化
void SceneGame::Finalize()
{
	//ゲージスプライト終了化
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	//エネミー終了化
	EnemyManager::Instance().Clear();

	//ステージ終了化
	StageManager::Instance().Clear();

	//プレイヤー終了化
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//ステージ更新処理
	StageManager::Instance().Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	//エネミー更新処理
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//enemyManager.Update(elapsedTime);
	EnemyManager::Instance().Update(elapsedTime);

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0〜1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		StageManager::Instance().Render(dc, shader);
		//プレイヤー描画
		player->Render(dc, shader);

		//エネミー描画
		/*EnemyManager& enemyManager = EnemyManager::Instance();
		enemyManager.Render(dc, shader);*/
		EnemyManager::Instance().Render(dc,shader);
		
		shader->End(dc);

	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();

		//エネミーデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2DデバッグGUI描画
	{
		//プレイヤーデバッグ描画
		player->DrawdebugGUI();
	}
}

//エネミーHPゲージ描画
void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//全ての敵の頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		
		//エネミーの頭上のワールド座標
		DirectX::XMFLOAT3 worldPosition =enemy->GetPosition() ;
		//yの位置だけ高さ分足す
		worldPosition.y += enemy->GetHeight();

		//敵の位置をXMVECTORに変換
		DirectX::XMVECTOR WorldPosition =DirectX::XMLoadFloat3(&worldPosition) ;

		//ワールド座標(3D)からスクリーン座標(2D)へ変換
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(WorldPosition,viewport.TopLeftX,viewport.TopLeftY,viewport.Width,viewport.Height,
			viewport.MinDepth,viewport.MaxDepth,Projection, View,World);

		//スクリーン座標(3Dから2DにしたXMVECTORをXMFLOAT2に変換)
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

		//ゲージのサイズ
		const float gaugeWidth = 30.0f;
		const float gaugeHeight = 5.0f;

		//ゲージの長さを計算(健康状態比率)（現在健康状態/最大健康状態）
		float healthRate = static_cast<float>(enemy->GetHealth()) / static_cast<float>(enemy->GetMaxHealth());

		//ゲージ描画
		gauge->Render(dc,
			screenPosition.x-gaugeWidth*0.5f,//描画の左位置（敵のｘ位置-ゲージ幅の半分）
			screenPosition.y-gaugeHeight,//描画の上位置（敵のｙ位置-ゲージの高さ）
			gaugeWidth*healthRate,//Spriteの幅（ゲージの幅*健康状態比率)
			gaugeHeight,//Spriteの高さ（ゲージの高さ）
			0,0,//Textureの切り取り位置XY
			static_cast<float>(gauge->GetTextureWidth()),//Textureの切り取り幅
			static_cast<float>(gauge->GetTextureHeight()),//Textureの切り取り高さ
			0.0f,//Spriteの角度
			1.0f,0.0f,0.0f,1.0f//Spriteの色(r,g,b,a)
			);
	}

	//エネミー配置処理
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//マウスカーソル座標を取得
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());

		DirectX::XMVECTOR ScreenPosition, WorldPosition;
		//レイの始点を計算
		screenPosition.z = 0.0f;//ビューポートの深度の0.0を設定
		//screenPositionをXMVECTORに変換
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);

		//始点を２Dから３D に変換
		WorldPosition = DirectX::XMVector3Unproject(ScreenPosition,
			viewport.TopLeftX,viewport.TopLeftY,
			viewport.Width,viewport.Height,
			viewport.MinDepth,viewport.MaxDepth,
			Projection,View,World
		);

		//ワールド空間上でのレイの始点(WorldPositionをXMFLOAT3に変換)
		DirectX::XMFLOAT3 rayStart;
		DirectX::XMStoreFloat3(&rayStart, WorldPosition);

		//レイの終点を算出		
		screenPosition.z = 1.0f;//ビューポートの深度を1.0に設定
							  
		//screenPositionをXMVECTORに変換
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		
		//終点を2Dから3Dに変換
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX, viewport.TopLeftY,
			viewport.Width, viewport.Height,
			viewport.MinDepth, viewport.MaxDepth,
			Projection, View, World
		);

		//ワールド空間上でのレイの終点(WorldPositionをXMFLOAT3に変換)
		DirectX::XMFLOAT3 rayEnd;
		DirectX::XMStoreFloat3(&rayEnd, WorldPosition);

		//レイキャスト（上で作ったレイとステージとの衝突)
		HitResult hit;
		if (StageManager::Instance().RayCast(rayStart,rayEnd,hit))
		{
			//敵を配置（敵を生成）
			EnemySlime* slime = new EnemySlime();
			//敵の位置をレイがヒットした位置に設定
			slime->setPosition(hit.position);
			//敵管理に登録
			EnemyManager::Instance().Register(slime);
		}
	}
}
