#pragma once

#include<vector>
#include<set>
#include"people.h"

//�A�C�e���}�l�[�W���[
class PeopleManager
{
private:
	PeopleManager() {}
	~PeopleManager() {}
public:
	//�B��̃C���X�^���X�擾
	static PeopleManager& Instance()
	{
		static PeopleManager instance;
		return instance;
	}

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�A�C�e���o�^
	void Register(People* people);

	//�A�C�e�����S�폜
	void Clear();

	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	//�A�C�e�����擾
	int GetPeopleCount() const { return static_cast<int>(humanes.size()); }

	//�A�C�e���擾
	People* GetPeople(int index) { return  humanes.at(index); }

	//�A�C�e���폜
	void Remove(People* people);


private:
	//�A�C�e�����m�̏Փˏ���
	//void CollisionItemVsItemes();

private:


	std::vector<People*> humanes;
	std::set<People*>removes;


};

