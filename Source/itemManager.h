#pragma once

#include<vector>
#include<set>
#include"item.h"

//�A�C�e���}�l�[�W���[
class ItemManager
{
private:
	ItemManager() {}
	~ItemManager() {}
public:
	//�B��̃C���X�^���X�擾
	static ItemManager& Instance()
	{
		static ItemManager instance;
		return instance;
	}

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�A�C�e���o�^
	void Register(Item* item);

	//�A�C�e�����S�폜
	void Clear();

	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	//�A�C�e�����擾
	int GetItemCount() const { return static_cast<int>(itemes.size()); }

	//�A�C�e���擾
	Item* GetItem(int index) { return itemes.at(index); }

	//�A�C�e���폜
	void Remove(Item* item);

	//�����g�X�擾�t���O
	const bool& GetMentos() const { return MentosFlag; }
	//�����g�X�擾��
	const int& GetMentosNum() const { return MentosNum; }

	void MentosIncrease();


private:
	//�A�C�e�����m�̏Փˏ���
	void CollisionItemVsItemes();

private:


	std::vector<Item*> itemes;
	std::set<Item*>removes;


	bool MentosFlag = false;

	int MentosNum = 0;

	float time = 0.0f;


};

