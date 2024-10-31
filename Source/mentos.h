#pragma once

#include"Graphics/Model.h"
#include "item.h"

//�X���C��
class Mentos :public Item
{
public:
	Mentos();
	~Mentos()override;

	//�X�V����
	void Update(float elapsedTime)override;

	//�`�揈�u
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//����
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

protected:
	//���S�����Ƃ��ɌĂ΂��
	void OnDead()override;

private:
	Model* model = nullptr;

	float speed = 5.0f;


};
