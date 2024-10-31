#include"StageMoveFloor.h"

//�R���X�g���N�^
StageMoveFloor::StageMoveFloor()
{
	scale.x = scale.z = 3.0f;
	scale.y = 0.5f;

	//�X�e�[�W���f����ǂݍ���
	model = new Model("Data/Model/Cube/Cube.mdl");


}

StageMoveFloor::~StageMoveFloor()
{
	//�X�e�[�W���f����j��
	delete model;
}

//�X�V����
void StageMoveFloor::Update(float elapsedTime)
{

	//�O��̏���ۑ�
	oldTransform = transform;
	oldAngle = angle;

	//�X�^�[�g����S�[���܂ł̋������Z�o����
	DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
	DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
	float length;
	DirectX::XMStoreFloat(&length, Length);

	//�X�^�[�g����S�[���܂ł̊Ԃ����ĕb�ԂŐi�ފ���(0.0~1.0)���Z�o����
	float speed = moveSpeed * elapsedTime;
	float speedRate = speed / length;
	moveRate += speedRate;

	//�S�[���̓��B�A�܂��̓X�^�[�g�ɖ߂����ꍇ�A�ړ������𔽓]������
	if (moveRate <= 0.0f || moveRate >= 1.0f)
	{
		moveSpeed = -moveSpeed;
	}

	//���`�⊮�ňʒu���Z�o����
	DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
	DirectX::XMStoreFloat3(&position, Position);

	//��]
	angle.x += torque.x * elapsedTime;
	angle.y += torque.y * elapsedTime;
	angle.z += torque.z * elapsedTime;


	//��]�s��
	UpdateTransfotm();

	//���f���s��X�V
	//model->UpdateTransform(transform);
	//���C�L���X�g�p�ɂ��ł���ԍs��̂��邽�ߒP�ʍs���n��
	const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	model->UpdateTransform(transformIdentity);


}

//�`�揈��
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//�\���p�̂��߃��[���h�s��ɍX�V����
	model->UpdateTransform(transform);


	shader->Draw(dc, model);
}

//���C�L���X�g
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	//return Collision::IntersectRayVsModel(start, end, model, hit);


	//�O��̃��[���h�s��Ƌt�s������߂�
	DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&oldTransform);
	DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);


	//�O��̃��[�J�����(oldTransform�����[�J����ԂɂȂ�)�ł̃��C�ɕϊ�
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);//���C�̎n�_
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);//���C�̏I�_
	//���C�̎n�_�����[���h��Ԃ���oldTransform�̋�Ԃɕϊ�
	DirectX::XMVECTOR LocalStart = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
	//���C�̏I�_�����[���h��Ԃ���oldTransform�̋�Ԃɕϊ�
	DirectX::XMVECTOR LocalEnd = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);

	//���[�J����Ԃł̃��C�Ƃ̌�_�����߂�
	DirectX::XMFLOAT3 localStart, localEnd;
	DirectX::XMStoreFloat3(&localStart, LocalStart);
	DirectX::XMStoreFloat3(&localEnd, LocalEnd);
    
	HitResult localhit;
	if (Collision::IntersectRayVsModel(localStart, localEnd, model, localhit))
	{
		//�O��̃��[�J����Ԃ��獡��̃��[���h��Ԃ֕ϊ�
		//�O�񂩂獡��ɂ����ĕϊ����ꂽ���e������Ă���I�u�W�F�N�g�ɔ��f�����
		//WorldTransform������̈ړ����̍s����㏑��
		WorldTransform = DirectX::XMLoadFloat4x4(&transform);
		//�q�b�g�����ړ����̈ʒu(localHit.position)���擾
		DirectX::XMVECTOR LocalPosition = DirectX::XMLoadFloat3(&localhit.position);
		//�q�b�g�����ʒu�����[�J����ԁi�ړ����j���烏�[���h��Ԃ֕ϊ�
		DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(LocalPosition, WorldTransform);
		//�q�b�g�����ړ����̌����i�@���x�N�g���j���擾
		DirectX::XMVECTOR LocalNormal = DirectX::XMLoadFloat3(&localhit.normal);
		//�q�b�g�������������[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
		DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(LocalNormal, WorldTransform);
		//���[���h��ԏ�Ń��C�̊J�n�ʒu����q�b�g�ʒu�܂ł̋����x�N�g��
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
		//���̒���
		DirectX::XMVECTOR Dist = DirectX::XMVector3Length(Vec);
		//���[���h�̃q�b�g�����ʒu��hit.position��
		DirectX::XMStoreFloat3(&hit.position, WorldPosition);
		//���[���h�̃q�b�g����������hit.normal��
		DirectX::XMStoreFloat3(&hit.normal, WorldNormal);
		//������hit.distance��
		DirectX::XMStoreFloat(&hit.distance, Dist);

		//�q�b�g�����}�e���A���C���f�b�N�X��hit.materialIndex��
		hit.materialIndex = localhit.materialIndex;

		//��]�������Z�o
		hit.rotation.x = angle.x - oldAngle.x;
		hit.rotation.y = angle.y - oldAngle.y;
		hit.rotation.z = angle.z - oldAngle.z;

		return true;
	}
	return false;
}

//�s��X�V����
void StageMoveFloor::UpdateTransfotm()
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
}