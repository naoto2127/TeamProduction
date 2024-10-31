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


// ������
void SceneGame::Initialize()
{
	//�X�e�[�W������
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(0, 1, 3));
	stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
	stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
	stageManager.Register(stageMoveFloor);

	//�v���C���[�̏�����
	player = new Player();

	//�J���������ݒ�
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

	//�J�����A�R���g���[��������
	cameraController = new CameraController();

	//�G�l�~�[������
	// �G�l�~�[�}�l�[�W���[�̃C���X�^���X��p�ӂ��Ă���
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 2; ++i)
	{
		//�X���C���𐶐�����
		EnemySlime* slime = new EnemySlime();
		//�G�l�~�[�X���C���̈ʒu��ݒ�(0,0,5)
		slime->setPosition(DirectX::XMFLOAT3(i*2.0f,0,5));
		//���������G�l�~�[�X���C�����G�l�~�[�}�l�[�W���[�ɓo�^����
		enemyManager.Register(slime);
	}

	//�Q�[�W�X�v���C�g
	gauge = new Sprite();
	
}

// �I����
void SceneGame::Finalize()
{
	//�Q�[�W�X�v���C�g�I����
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

	//�G�l�~�[�I����
	EnemyManager::Instance().Clear();

	//�X�e�[�W�I����
	StageManager::Instance().Clear();

	//�v���C���[�I����
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//�J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//�X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);

	//�v���C���[�X�V����
	player->Update(elapsedTime);

	//�G�l�~�[�X�V����
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//enemyManager.Update(elapsedTime);
	EnemyManager::Instance().Update(elapsedTime);

	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);

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

		//�G�l�~�[�`��
		/*EnemyManager& enemyManager = EnemyManager::Instance();
		enemyManager.Render(dc, shader);*/
		EnemyManager::Instance().Render(dc,shader);
		
		shader->End(dc);

	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
		player->DrawDebugPrimitive();

		//�G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		EnemyManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2D�f�o�b�OGUI�`��
	{
		//�v���C���[�f�o�b�O�`��
		player->DrawdebugGUI();
	}
}

//�G�l�~�[HP�Q�[�W�`��
void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//�S�Ă̓G�̓����HP�Q�[�W��\��
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		
		//�G�l�~�[�̓���̃��[���h���W
		DirectX::XMFLOAT3 worldPosition =enemy->GetPosition() ;
		//y�̈ʒu��������������
		worldPosition.y += enemy->GetHeight();

		//�G�̈ʒu��XMVECTOR�ɕϊ�
		DirectX::XMVECTOR WorldPosition =DirectX::XMLoadFloat3(&worldPosition) ;

		//���[���h���W(3D)����X�N���[�����W(2D)�֕ϊ�
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(WorldPosition,viewport.TopLeftX,viewport.TopLeftY,viewport.Width,viewport.Height,
			viewport.MinDepth,viewport.MaxDepth,Projection, View,World);

		//�X�N���[�����W(3D����2D�ɂ���XMVECTOR��XMFLOAT2�ɕϊ�)
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

		//�Q�[�W�̃T�C�Y
		const float gaugeWidth = 30.0f;
		const float gaugeHeight = 5.0f;

		//�Q�[�W�̒������v�Z(���N��Ԕ䗦)�i���݌��N���/�ő匒�N��ԁj
		float healthRate = static_cast<float>(enemy->GetHealth()) / static_cast<float>(enemy->GetMaxHealth());

		//�Q�[�W�`��
		gauge->Render(dc,
			screenPosition.x-gaugeWidth*0.5f,//�`��̍��ʒu�i�G�̂��ʒu-�Q�[�W���̔����j
			screenPosition.y-gaugeHeight,//�`��̏�ʒu�i�G�̂��ʒu-�Q�[�W�̍����j
			gaugeWidth*healthRate,//Sprite�̕��i�Q�[�W�̕�*���N��Ԕ䗦)
			gaugeHeight,//Sprite�̍����i�Q�[�W�̍����j
			0,0,//Texture�̐؂���ʒuXY
			static_cast<float>(gauge->GetTextureWidth()),//Texture�̐؂��蕝
			static_cast<float>(gauge->GetTextureHeight()),//Texture�̐؂��荂��
			0.0f,//Sprite�̊p�x
			1.0f,0.0f,0.0f,1.0f//Sprite�̐F(r,g,b,a)
			);
	}

	//�G�l�~�[�z�u����
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//�}�E�X�J�[�\�����W���擾
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());

		DirectX::XMVECTOR ScreenPosition, WorldPosition;
		//���C�̎n�_���v�Z
		screenPosition.z = 0.0f;//�r���[�|�[�g�̐[�x��0.0��ݒ�
		//screenPosition��XMVECTOR�ɕϊ�
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);

		//�n�_���QD����RD �ɕϊ�
		WorldPosition = DirectX::XMVector3Unproject(ScreenPosition,
			viewport.TopLeftX,viewport.TopLeftY,
			viewport.Width,viewport.Height,
			viewport.MinDepth,viewport.MaxDepth,
			Projection,View,World
		);

		//���[���h��ԏ�ł̃��C�̎n�_(WorldPosition��XMFLOAT3�ɕϊ�)
		DirectX::XMFLOAT3 rayStart;
		DirectX::XMStoreFloat3(&rayStart, WorldPosition);

		//���C�̏I�_���Z�o		
		screenPosition.z = 1.0f;//�r���[�|�[�g�̐[�x��1.0�ɐݒ�
							  
		//screenPosition��XMVECTOR�ɕϊ�
		ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);
		
		//�I�_��2D����3D�ɕϊ�
		WorldPosition = DirectX::XMVector3Unproject(
			ScreenPosition,
			viewport.TopLeftX, viewport.TopLeftY,
			viewport.Width, viewport.Height,
			viewport.MinDepth, viewport.MaxDepth,
			Projection, View, World
		);

		//���[���h��ԏ�ł̃��C�̏I�_(WorldPosition��XMFLOAT3�ɕϊ�)
		DirectX::XMFLOAT3 rayEnd;
		DirectX::XMStoreFloat3(&rayEnd, WorldPosition);

		//���C�L���X�g�i��ō�������C�ƃX�e�[�W�Ƃ̏Փ�)
		HitResult hit;
		if (StageManager::Instance().RayCast(rayStart,rayEnd,hit))
		{
			//�G��z�u�i�G�𐶐��j
			EnemySlime* slime = new EnemySlime();
			//�G�̈ʒu�����C���q�b�g�����ʒu�ɐݒ�
			slime->setPosition(hit.position);
			//�G�Ǘ��ɓo�^
			EnemyManager::Instance().Register(slime);
		}
	}
}
