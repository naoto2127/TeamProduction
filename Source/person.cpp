#include"person.h"

//�R���X�g���N�^
Person::Person()
{
	model = new Model("Data/Model/chara/person1.mdl");

	//�ҋ@�X�e�[�g�֑J��
	TransitionIdleState();

	//���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.012f;

	//���A�����ݒ�
	radius = 0.5f;
	height = 2.0f;
}

Person::~Person()
{
	delete model;
}

//�X�V����
void Person::Update(float elapsedTime)
{
	//���͏����X�V
	UpdateVelocity(elapsedTime);

	//�X�e�[�g���̏���
	switch (state)
	{
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;
	case State::Death:
		UpdateDeathState(elapsedTime);
		break;
	}

	//���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	model->UpdateAnimation(elapsedTime);

	//���f���s��X�V
	model->UpdateTransform(transform);

	float speed = this->speed * elapsedTime;


	//�ʒu�{���O����*����

	position.z += speed;

}

//�`�揈��
void Person::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void Person::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->position = position;
}

void Person::TransitionIdleState()
{
	state = State::Idle;

	//�ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Walk, true);
}

void Person::UpdateIdleState(float elapsedTime)
{
	
}

void Person::TrasitionDeathState()
{
	state = State::Death;

	//�ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Death, false);
}

void Person::UpdateDeathState(float elapsedTime)
{
	//���f���j��
	if (!model->IsPlayAnimation())
		Destroy();
}

//���S�����Ƃ��ɌĂ΂��
void Person::OnDead()
{
	//���S�A�j���[�V�����Ăяo��
	TrasitionDeathState();
}