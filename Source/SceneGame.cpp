#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "CameraController.h"
#include "EffectManager.h"
#include "Input/Input.h"
#include "StageManager.h"
#include "StageMain.h"

#include "SceneManager.h"
#include "SceneLoading.h"
#include "GameOver.h"
#include "mentos.h"
#include "itemManager.h"
#include"person.h"
#include"peopleManager.h"
#include <cola.h>
#include"person2.h"



extern int count;


// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);
	
	//プレイヤー初期化
	player = new Player();
	moji = new Sprite("Data/fonts/font4.png");
	gauge = new Gauge();
	

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
		);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
	count = 0;

	//カメラコントローラー初期化
	cameraController = new CameraController();	

	//アイテム初期化
    //アイテムマネージャーのインスタンスを用意しておく
	ItemManager& itemManager = ItemManager::Instance();


	PeopleManager& peopleManager = PeopleManager::Instance();
	

	GameMusic = Audio::Instance().LoadAudioSource("Data/music/game.wav");

	GameMusic->Play(true);
}

// 終了化
void SceneGame::Finalize()
{

	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//ステージ終了化
	StageManager::Instance().Clear();

	//アイテム終了化
	ItemManager::Instance().Clear();

	PeopleManager::Instance().Clear();

	liquidManager.Clear();

	//プレイヤー終了化
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}
	if (moji != nullptr)
	{
		delete moji;
		moji = nullptr;
	}

	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	GameMusic->Stop();
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->Settarget(target);
	cameraController->Update(elapsedTime);

	//ステージ更新処理
	StageManager::Instance().Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	liquidManager.Update(elapsedTime);


	gauge->Update(elapsedTime);

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	ItemManager::Instance().Update(elapsedTime);

	PeopleManager::Instance().Update(elapsedTime);

	GameEnd();

	Mentostime+=elapsedTime;
	Persontime+=elapsedTime;
	Persontime2+=elapsedTime;

	gameTime -= elapsedTime;

	if (Mentostime>=30.0f)
	{
		CreateMentos();
		Mentostime = 0.0f;
	}

	if (Persontime >= 5.0f)
	{
		CreatePerson();
		Persontime = 0.0f;

	}

	if (Persontime2 >= 8.0f)
	{
		CreatePerson2();
		Persontime2 = 0.0f;

	}

	




}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);



	



	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラパラメータ
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

		ItemManager::Instance().Render(dc, shader);

		PeopleManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	moji->textout(dc, std::to_string(static_cast<int>(gameTime)),
		600, 10,
		40, 40,
		1, 1, 1, 1);

	moji->textout(dc, "score:",
		0, 10,
		30, 30,
		1, 1, 1, 1);
	moji->textout(dc, std::to_string(count),
		200, 10,
		30, 30,
		1, 1, 1, 1);

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}
	
	//2Dを透過させる

	dc->OMSetDepthStencilState(Graphics::Instance().GetDepthStencilState(3), 1);
	dc->OMSetBlendState(Graphics::Instance().GetBlendState(1), nullptr, 0xFFFFFFFF);
	gauge->Render(dc);
	
	Mouse& mouse = Input::Instance().GetMouse();
	//直進弾丸発射
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT &&static_cast<int>( gauge->gaugeNum)!=0)
	{
		Cola* cola = new Cola(&liquidManager);
		player->LaunchCola();
		if (gauge->gaugeNum > 0)
		{
			gauge->gaugeNum--;
		}
		
		//RenderCola(dc, rc.view, rc.projection);
	}
	if (gauge->gaugeNum >= 0)
	{
		liquidManager.Render(dc, rc.view, rc.projection, player->GetPosition(),player->GetFront());
	}
	
		
	
}

void SceneGame::GameEnd()
{
	if (static_cast<int>(gameTime)<=0)
		SceneManager::Instance().ChangeScene(new SceneLoading(new GameOver()));
}

void SceneGame::CreateMentos()
{
	//ランダムにアイテムを生成する
	float x = (float)rand() / RAND_MAX * 14.0f - 7.0f;

	DirectX::XMFLOAT3 NewPosition = { x,30.0f,0.0f };
	
	//メントスを配置（メントスを生成）
	Mentos* mentos = new Mentos();
	//メントスの位置を設定
	mentos->SetPosition(NewPosition);
	//アイテム管理に登録
	ItemManager::Instance().Register(mentos);
}

void SceneGame::CreatePerson()
{
	//ランダムにアイテムを生成する
	float x = (float)rand() / RAND_MAX * 14.0f - 7.0f;

	DirectX::XMFLOAT3 NewPosition = { x,30.0f,-9.5f };

	//メントスを配置（メントスを生成）
	Person* person = new Person();
	//メントスの位置を設定
	person->SetPosition(NewPosition);
	//アイテム管理に登録
	PeopleManager::Instance().Register(person);
}

void SceneGame::CreatePerson2()
{
	//ランダムにアイテムを生成する
	//float x = (float)rand() / RAND_MAX * 3.0f + 0.0f;
	float x = (float)rand() / RAND_MAX * 14.0f - 7.0f;

	DirectX::XMFLOAT3 NewPosition = { x,30.0f,9.5f };

	//メントスを配置（メントスを生成）
	Person2* person2 = new Person2();
	//メントスの位置を設定
	person2->SetPosition(NewPosition);
	//アイテム管理に登録
	PeopleManager::Instance().Register(person2);
}


