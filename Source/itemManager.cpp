#include"ItemManager.h"
#include"Collision.h"

//�X�V����
void ItemManager::Update(float elapsedTime)
{
	for (Item* item : itemes)
	{
		item->Update(elapsedTime);
	}


	//�j������
	for (Item* item : removes)
	{
		//std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Item*>::iterator it = std::find(itemes.begin(), itemes.end(), item);
		if (it != itemes.end())
		{
			itemes.erase(it);
		}

		//�e�ۂ̔j������
		delete item;
	}
	removes.clear();

	if (MentosNum > 0)
	{
		MentosFlag = true;
	}
	else
	{
		MentosFlag = false;
	}

	if (MentosFlag)
	{
		time += elapsedTime;

		if (time >= 10.0)
		{
			MentosNum--;
			time = 0.0f;
		}
	}



	////�G���m�̏Փˏ���
	CollisionItemVsItemes();


}

//�`�揈��
void ItemManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (Item* item : itemes)
	{
		item->Render(context, shader);
	}
}

//�f�o�b�O�v���~�e�B�u�`��
void ItemManager::DrawDebugPrimitive()
{
	for (Item* item : itemes)
	{
		item->DrawDebugPrimitive();
	}
}

//�G�l�~�[�o�^
void ItemManager::Register(Item* item)
{
	itemes.emplace_back(item);
}

//�G�l�~�[�S�폜
void ItemManager::Clear()
{
	for (Item* item : itemes)
	{
		delete item;
	}

	itemes.clear();
}

void ItemManager::Remove(Item* item)
{
	//�j�����X�g�ɒǉ�
	removes.insert(item);
}

void ItemManager::MentosIncrease()
{
	MentosNum++;
}

//�G�l�~�[���m�̏Փˏ���
void ItemManager::CollisionItemVsItemes()
{
	//�G�̐����擾
	int numEnemies = GetItemCount();
	//�G�̐����񂷁i�J�E���^�[i�j
	for (int i = 0; i < numEnemies; ++i)
	{
		//�GA���擾
		Item* itemA = GetItem(i);
		//�G�̐�����(�J�E���^�[j��i��j�͈Ⴄ���ɂȂ�悤��(�����G���m�͏Փ˂��Ȃ�����)�j
		for (int j = 0; j < numEnemies; ++j)
		{
			if (i == j)// �����G���m�͏Փ˂��Ȃ����߁A�����C���f�b�N�X�̏ꍇ�̓X�L�b�v           
				continue;
			//�GB���擾
			Item* itemB = GetItem(j);
			//�����o����̈ʒu
			DirectX::XMFLOAT3 newPositionB;

			////�Փˏ���
			//if (Collision::IntersectCylonderVsCylinder(
			//	//�GA�̈ʒu
			//	itemA->GetPosition(),
			//	//�GA�̔��a
			//	itemA->GetRadius(),
			//	//�GA�̍���
			//	itemA->GetHeight(),
			//	//�GB�̈ʒu
			//	itemB->GetPosition(),
			//	//�GB�̔��a
			//	itemB->GetRadius(),
			//	//�GB�̍���
			//	itemB->GetHeight(),
			//	//�����o����̈ʒu
			//	newPositionB))
			//{
			//	//�GB�ɉ����o����̈ʒu��ݒ�
			//	itemB->setPosition(newPositionB);
			//}
		}
	}
}