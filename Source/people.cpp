#include"people.h"
#include"Graphics/Graphics.h"
#include"peopleManager.h"

//�f�o�b�O�v���~�e�B�u�`��
void People::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();


	//�Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

//�j��
void People::Destroy()
{
	//�G�}�l�W���[�̍폜�Ɏ������g�̃A�h���X��n��
	PeopleManager::Instance().Remove(this);
}