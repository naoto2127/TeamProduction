#pragma once

#include "Stage.h"
#include "Player.h"
#include "CameraController.h"
#include "Graphics/Sprite.h"
#include "Scene.h"
#include"gauge.h"
#include"liquidManager.h"
#include "Audio/Audio.h"


//追加
#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;


	void GameEnd();

private:
	//メントス生成
	void CreateMentos();

	//人生成
	void CreatePerson();

	void CreatePerson2();


	

private:
	Player* player = nullptr;
	
	Gauge* gauge = nullptr;

	
	CameraController* cameraController = nullptr;
	Sprite* back = nullptr;
	Sprite* moji = nullptr;
	//Sprite* gauge = nullptr;
	Sprite* pose[4] = {nullptr,nullptr ,nullptr ,nullptr };

	LiquidManager liquidManager;

	
	
	float p_scale[4] = { 1.0f,1.0f,1.0f,1.0f };

	float Mentostime = 0.0f;
	float Persontime = 0.0f;
	float Persontime2 = 0.0f;

	float gameTime = 60.0f;

	

	bool flag = false;

	std::unique_ptr<AudioSource> GameMusic;
	
};
