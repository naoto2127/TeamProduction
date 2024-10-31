#include"PeopleManager.h"
#include"Collision.h"

//�X�V����
void PeopleManager::Update(float elapsedTime)
{
	for (People* people : humanes)
	{
		people->Update(elapsedTime);
	}


	for (People* people : humanes)
	{
		if (people->GetPosition().z >= 10)
		{
			people->Destroy();
		}
		if (people->GetPosition().z <= -10)
		{
			people->Destroy();
		}
	}

	//�j������
	for (People*people  : removes)
	{
		//std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<People*>::iterator it = std::find(humanes.begin(), humanes.end(), people);
		if (it != humanes.end())
		{
			humanes.erase(it);
		}

		//�e�ۂ̔j������
		delete people;
	}
	removes.clear();



	////�G���m�̏Փˏ���
	//CollisionPeopleVsPeoplees();


}

//�`�揈��
void PeopleManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (People* people : humanes)
	{
		people->Render(context, shader);
	}
}

//�f�o�b�O�v���~�e�B�u�`��
void PeopleManager::DrawDebugPrimitive()
{
	for (People* people : humanes)
	{
		people->DrawDebugPrimitive();
	}
}

//�G�l�~�[�o�^
void PeopleManager::Register(People* people)
{
	humanes.emplace_back(people);
}

//�G�l�~�[�S�폜
void PeopleManager::Clear()
{
	for (People* people : humanes)
	{
		delete people;
	}

	humanes.clear();
}

void PeopleManager::Remove(People* people)
{
	//�j�����X�g�ɒǉ�
	removes.insert(people);
}

////�G�l�~�[���m�̏Փˏ���
//void PeopleManager::CollisionPeopleVsPeoplees()
//{
//	//�G�̐����擾
//	int numEnemies = GetPeopleCount();
//	//�G�̐����񂷁i�J�E���^�[i�j
//	for (int i = 0; i < numEnemies; ++i)
//	{
//		//�GA���擾
//		People* peopleA = GetPeople(i);
//		//�G�̐�����(�J�E���^�[j��i��j�͈Ⴄ���ɂȂ�悤��(�����G���m�͏Փ˂��Ȃ�����)�j
//		for (int j = 0; j < numEnemies; ++j)
//		{
//			if (i == j)// �����G���m�͏Փ˂��Ȃ����߁A�����C���f�b�N�X�̏ꍇ�̓X�L�b�v           
//				continue;
//			//�GB���擾
//			People* peopleB = GetPeople(j);
//			//�����o����̈ʒu
//			DirectX::XMFLOAT3 newPositionB;
//
//			////�Փˏ���
//			//if (Collision::IntersectCylonderVsCylinder(
//			//	//�GA�̈ʒu
//			//	peopleA->GetPosition(),
//			//	//�GA�̔��a
//			//	peopleA->GetRadius(),
//			//	//�GA�̍���
//			//	peopleA->GetHeight(),
//			//	//�GB�̈ʒu
//			//	peopleB->GetPosition(),
//			//	//�GB�̔��a
//			//	peopleB->GetRadius(),
//			//	//�GB�̍���
//			//	peopleB->GetHeight(),
//			//	//�����o����̈ʒu
//			//	newPositionB))
//			//{
//			//	//�GB�ɉ����o����̈ʒu��ݒ�
//			//	peopleB->setPosition(newPositionB);
//			//}
//		}
//	}
//}