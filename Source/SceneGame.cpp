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


// ������
void SceneGame::Initialize()
{
	//�X�e�[�W������
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);
	
	//�v���C���[������
	player = new Player();
	moji = new Sprite("Data/fonts/font4.png");
	gauge = new Gauge();
	

	//�J���������ݒ�
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

	//�J�����R���g���[���[������
	cameraController = new CameraController();	

	//�A�C�e��������
    //�A�C�e���}�l�[�W���[�̃C���X�^���X��p�ӂ��Ă���
	ItemManager& itemManager = ItemManager::Instance();


	PeopleManager& peopleManager = PeopleManager::Instance();
	

	GameMusic = Audio::Instance().LoadAudioSource("Data/music/game.wav");

	GameMusic->Play(true);
}

// �I����
void SceneGame::Finalize()
{

	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//�X�e�[�W�I����
	StageManager::Instance().Clear();

	//�A�C�e���I����
	ItemManager::Instance().Clear();

	PeopleManager::Instance().Clear();

	liquidManager.Clear();

	//�v���C���[�I����
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

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->Settarget(target);
	cameraController->Update(elapsedTime);

	//�X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);

	//�v���C���[�X�V����
	player->Update(elapsedTime);

	liquidManager.Update(elapsedTime);


	gauge->Update(elapsedTime);

	//�G�t�F�N�g�X�V����
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

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);



	



	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J�����p�����[�^
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);

		//�v���C���[�`��
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

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}
	
	//2D�𓧉߂�����

	dc->OMSetDepthStencilState(Graphics::Instance().GetDepthStencilState(3), 1);
	dc->OMSetBlendState(Graphics::Instance().GetBlendState(1), nullptr, 0xFFFFFFFF);
	gauge->Render(dc);
	
	Mouse& mouse = Input::Instance().GetMouse();
	//���i�e�۔���
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
	//�����_���ɃA�C�e���𐶐�����
	float x = (float)rand() / RAND_MAX * 14.0f - 7.0f;

	DirectX::XMFLOAT3 NewPosition = { x,30.0f,0.0f };
	
	//�����g�X��z�u�i�����g�X�𐶐��j
	Mentos* mentos = new Mentos();
	//�����g�X�̈ʒu��ݒ�
	mentos->SetPosition(NewPosition);
	//�A�C�e���Ǘ��ɓo�^
	ItemManager::Instance().Register(mentos);
}

void SceneGame::CreatePerson()
{
	//�����_���ɃA�C�e���𐶐�����
	float x = (float)rand() / RAND_MAX * 14.0f - 7.0f;

	DirectX::XMFLOAT3 NewPosition = { x,30.0f,-9.5f };

	//�����g�X��z�u�i�����g�X�𐶐��j
	Person* person = new Person();
	//�����g�X�̈ʒu��ݒ�
	person->SetPosition(NewPosition);
	//�A�C�e���Ǘ��ɓo�^
	PeopleManager::Instance().Register(person);
}

void SceneGame::CreatePerson2()
{
	//�����_���ɃA�C�e���𐶐�����
	//float x = (float)rand() / RAND_MAX * 3.0f + 0.0f;
	float x = (float)rand() / RAND_MAX * 14.0f - 7.0f;

	DirectX::XMFLOAT3 NewPosition = { x,30.0f,9.5f };

	//�����g�X��z�u�i�����g�X�𐶐��j
	Person2* person2 = new Person2();
	//�����g�X�̈ʒu��ݒ�
	person2->SetPosition(NewPosition);
	//�A�C�e���Ǘ��ɓo�^
	PeopleManager::Instance().Register(person2);
}


