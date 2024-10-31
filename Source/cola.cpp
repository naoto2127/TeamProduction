#include "cola.h"
#include <Collision.h>
#include "StageManager.h"


//�R���X�g���N�^
Cola::Cola(LiquidManager* manager) :Liquid(manager)
{
    //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //�\���T�C�Y�𒲐�
    //scale.x = scale.y = scale.z = 0.5f;
    scale.x = scale.y = scale.z = 3.0f;
	cola = new Sprite("Data/Sprite/cola.png");
}

//�f�X�g���N�^
Cola::~Cola()
{
    //delete model;
	if (cola != nullptr)
	{
		delete cola;
		cola = nullptr;
	}
}

//�X�V����
void Cola::Update(float elapsedTime)
{
    //��������
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        //�������폜
        Destroy();
    }

    //�ړ�
    float speed = this->speed * elapsedTime;
#if 1
    ////�ʒu+=�O����*����
    //position.x += direction.x * speed;
    //position.y += direction.y * speed;
    //position.z += direction.z * speed;
#endif

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    //model->UpdateTransform(transform);

	if (colaNum <= 11)
		colaNum += 0.1f;
}

//�`�揈��
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

//����
void Cola::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}


//�R�[���`��
void Cola::RenderCola(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection,
	const DirectX::XMFLOAT3 Pos, const DirectX::XMFLOAT3 Front
	)
{
	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//�v���C���[�̏ꏊ�ɃR�[����\��
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//int enemyCount = enemyManager.GetEnemyCount();

	/*for (int i = 0; i < enemyCount; ++i)
	{*/
	//Enemy* enemy = enemyManager.GetEnemy(i);

	//�v���C���[�̃��[���h���W
	DirectX::XMFLOAT3 worldPosition = Pos;
	//DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
	// 
	worldPosition.x -= 1.3f;
	//y�̈ʒu��������������
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
	//�v���C���[�̈ʒu��XMVECTOR�ɕϊ�
	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);
	//DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);

	//���[���h���W(3D)����X�N���[�����W(2D)�֕ϊ�
	DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(WorldPosition, viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height,
		viewport.MinDepth, viewport.MaxDepth, Projection, View, World);

	//�X�N���[�����W(3D����2D�ɂ���XMVECTOR��XMFLOAT2�ɕϊ�)
	DirectX::XMFLOAT2 screenPosition;
	DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

	//�Q�[�W�̃T�C�Y
	const float gaugeWidth = 30.0f;
	const float gaugeHeight = 5.0f;

	//�Q�[�W�̒������v�Z(���N��Ԕ䗦)�i���݌��N���/�ő匒�N��ԁj
	//float healthRate = static_cast<float>(enemy->GetHealth()) / static_cast<float>(enemy->GetMaxHealth());

	//�Q�[�W�`��
	cola->Render(dc,
		screenPosition.x,//�`��̍��ʒu�i�G�̂��ʒu-�Q�[�W���̔����j
		screenPosition.y,//�`��̏�ʒu�i�G�̂��ʒu-�Q�[�W�̍����j
		300,//Sprite�̕��i�Q�[�W�̕�*���N��Ԕ䗦)
		300,//Sprite�̍����i�Q�[�W�̍����j
		640 * static_cast<int>(colaNum), 0,//Texture�̐؂���ʒuXY
		640,//Texture�̐؂��蕝
		480,//Texture�̐؂��荂��
		static_cast<int>(colaAngle),//Sprite�̊p�x
		1, 1, 1, 1//Sprite�̐F(r,g,b,a)
	);



}
