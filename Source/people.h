#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

//�l
class People :public Character
{
public:
	People() {}
	~People() {}

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�`�揈��
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//�f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	//�j��
	void Destroy();

};
