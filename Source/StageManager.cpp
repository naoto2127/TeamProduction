#include"StageManager.h"

//�X�V����
void StageManager::Update(float elapsedTime)
{
	for (Stage* stage : stages)
	{
		stage->Update(elapsedTime);
	}


	
}

//�`�揈��
void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{

	for (Stage* stage : stages)
	{
		stage->Render(dc, shader);
	}
}

//�X�e�[�W�o�^
void StageManager::Register(Stage* stage)
{
	stages.emplace_back(stage);
}


//�X�e�[�W�S�폜
void StageManager::Clear()
{
	//�S�ẴX�e�[�W�������
	for (Stage* stage : stages)
	{
		delete stage;
	}

	

	//�C�e���[�^�[���N���A����
	stages.clear();
}

//���C�L���X�g
bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	bool result = false;

	//1��ڂ̔���͋����ő�ɂ��Ă����K�v������
	hit.distance = FLT_MAX;

	//�S�ẴX�e�[�W��
	for (Stage*stage:stages)
	{
		//���C�L���X�g
		HitResult tmp;
		if (stage->RayCast(start,end,tmp))
		{
			//�������Ă�����A����̃X�e�[�W�̕����߂����ǂ����`�F�b�N
			if (hit.distance>tmp.distance)
			{
				//����̂ق����߂�������hit�����㏑����result��true
				hit = tmp;
				result = true;
			}
		}
	}
	return result;
}