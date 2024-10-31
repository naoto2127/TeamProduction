#include"item.h"
#include"Graphics/Graphics.h"
#include"itemManager.h"

//�f�o�b�O�v���~�e�B�u�`��
void Item::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();


	//�Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

//�j��
void Item::Destroy()
{
	//�G�}�l�W���[�̍폜�Ɏ������g�̃A�h���X��n��
	ItemManager::Instance().Remove(this);
}