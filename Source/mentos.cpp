#include"mentos.h"

//�R���X�g���N�^
Mentos::Mentos()
{
	model = new Model("Data/Model/mentos/mentos.mdl");

	//���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.012f;

	//���A�����ݒ�
	radius = 0.5f;
	height = 2.0f;
}

Mentos::~Mentos()
{
	delete model;
}

//�X�V����
void Mentos::Update(float elapsedTime)
{
	//���͏����X�V
	UpdateVelocity(elapsedTime);

	//���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���s��X�V
	model->UpdateTransform(transform);

	float speed = this->speed * elapsedTime;


	
	
	

}

//�`�揈��
void Mentos::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void Mentos::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->position = position;
}

//���S�����Ƃ��ɌĂ΂��
void Mentos::OnDead()
{
	//�������g���폜
	Destroy();
}