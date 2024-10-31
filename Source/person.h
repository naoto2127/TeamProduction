#pragma once

#include"Graphics/Model.h"
#include "people.h"

//�X���C��
class Person :public People
{
public:
	Person();
	~Person()override;

	//�X�V����
	void Update(float elapsedTime)override;

	//�`�揈�u
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//����
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

	//�ҋ@�X�e�[�g�֑J��
	void TransitionIdleState();

	//�ҋ@�X�e�[�g�X�V����
	void UpdateIdleState(float elapsedTime);

	//���S�X�e�[�g�J��
	void TrasitionDeathState();

	//�ҋ@�X�e�[�g�X�V����
	void UpdateDeathState(float elapsedTime);

protected:
	//���S�����Ƃ��ɌĂ΂��
	void OnDead()override;

private:

	enum Animation
	{
		Anim_Walk,
		Anim_Death

	};
	enum class State
	{
		Idle,
		Death
	};

private:
	Model* model = nullptr;

	float speed = 1.0f;

	State state = State::Idle;

};
