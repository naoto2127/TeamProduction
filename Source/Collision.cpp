#include"Collision.h"

//���Ƌ��̌�������
bool Collision::IntersectSphereVsSphre(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB
)
{
	//A��B�̒P�ʃx�N�g�����Z�o
	//XMFLOAT��XMVECTOR�ɕϊ�
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	//Vec=B-A
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB,PositionA);
	//XMVector3LengthSq�֐��Œ������v�Z�@sqrt���Ă��Ȃ�LengthSq�̂ق�������
	DirectX::XMVECTOR LengthSq= DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	//��������(���aA+���aB)
	float range = radiusA + radiusB;
	//sqrt���Ȃ����������a��2�悷��
	if (lengthSq>range*range)
	{
		return false;
	}

	//A��B�������o��
	
	//�^�[�Q�b�g�����ւ̃x�N�g��(Vec)�𐳋K��
	Vec = DirectX::XMVector3Normalize(Vec);

	//���K�������x�N�g����range�X�P�[��(XMVectorScale)
	Vec = DirectX::XMVectorScale(Vec, range);

	//�X�P�[�������x�N�g����PositionA�̈ʒu���瑫��PositionB�͈Ӗ����Ȃ����ł��悢
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);

	//�������l��XMFLOAT3(outPositionB)�ɕϊ�
	DirectX::XMStoreFloat3(&outPositionB,PositionB );
	return true;
}

//�~���Ɖ~���̌�������
bool Collision::IntersectCylonderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB
)
{
	//A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}
	//A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (positionA.y + heightA < positionB.y)
	{
		return false;

	}
	//XZ���ʂł͈̔̓`�F�b�N
	//X���m������
	float vx = positionB.x - positionA.x;

	//Z���m������
	float vz = positionB.z - positionA.z;

	//XZ�̒������v�Z����
	float distXZ = sqrtf(vx*vx+vz*vz);

	//���aA�Ɣ��aB�̒������v�Z����
	float range = radiusA+radiusB;

	//XZ�̒��������aA�Ɣ��aB�̒������傫�������瓖�����ĂȂ�
	if (distXZ>range)
	{
		return false;
	}
	//A��B�������o��
	vx /= distXZ;
	vz /= distXZ;

	outPositionB.x = vx * range + positionA.x;
	outPositionB.y = positionB.y;
	outPositionB.z = vz * range + positionA.z;

	return true;

}

bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius, 
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius, 
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition)
{
	//���̑������~���̓�����Ȃ瓖�����Ă��Ȃ�
	if (spherePosition.y-sphereRadius > cylinderPosition.y + cylinderHeight)
	{
		return false;
	}
	//���̓����~���̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (spherePosition.y + sphereRadius < cylinderPosition.y)
	{
		return false;
	}
	//XZ���ʂł͈̔̓`�F�b�N
	//X���m������
	float vx = cylinderPosition.x - spherePosition.x;

	//Z���m������
	float vz = cylinderPosition.z - spherePosition.z;

	//XZ�̒������v�Z����
	float distXZ = sqrtf(vx * vx + vz * vz);

	//���aA�Ɣ��aB�̒������v�Z����
	float range = sphereRadius + cylinderRadius;

	//XZ�̒��������aA�Ɣ��aB�̒������傫�������瓖�����ĂȂ�
	if (distXZ > range)
	{
		return false;
	}
	//�����~���������o��
	//vx��vz�𐳋K��
	vx /= distXZ;
	vz /= distXZ;

	outCylinderPosition.x = (vx * range) + spherePosition.x;
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = vz * range + spherePosition.z;

	return true;

}


/// <summary>
/// ���C�ƃ��f���̌�������
/// </summary>
/// <param name="start">���C�̎n�_</param>
/// <param name="end">���C�̏I�_</param>
/// <param name="model">�Փː�̃��f��</param>
/// <param name="result">���ʂ̃q�b�g���U���g�\����</param>
/// <returns>true...�Փ˂���</returns>
bool Collision::IntersectRayVsModel(
	const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	const Model* model,
	HitResult& result
)
{
	//���[���h���W�n�ł̃��C�̊J�n�_
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
	//���[���h���W�n�ł̃��C�̏I�_
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
	//���[���h���W�n�ł̃��C�̃x�N�g��
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	//���[���h���W�n�ł̃��C�̒���
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

	//���[���h��Ԃ̃��C�̒���
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	//true...�������Ă���
	bool hit = false;
	const ModelResource* resource = model->GetResource();
	//�S�Ẵ��b�V���Ƃ̏Փ˔�����s��(mesh=fbx(mdl)�̒��̂P�̃|���S���̉�
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		//���b�V���m�[�h�擾
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

		//���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

		//���C�̎n�_
		DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		//���C�̏I�_
		DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		//���C�̃x�N�g��
		DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
		//���C�̃x�N�g���𐳋K��
		DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
		//���C�̒���
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

		//���C�̒���(��ԋ߂����C�j
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		//�O�p�`�i�ʁj�Ƃ̌�������
		//���b�V���������Ă��邷�ׂĂ̒��_���擾
		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		//���b�V���������Ă��邷�ׂẴC���f�b�N�X���擾
		const std::vector<UINT>indices = mesh.indices;

		int materialIndex = -1;         //�Փ˂����ۂ̃|���S���̍ޗ�
		DirectX::XMVECTOR HitPosition;  //�Փ˂����ۂ̌�_�̈ʒu
		DirectX::XMVECTOR HitNormal;    //�Փ˂����ۂ̃|���S���̖@���i�����j
		for (const ModelResource::Subset&subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.indexCount; i += 3)
			{
				UINT index = subset.startIndex + i;

				//�O�p�`�̒��_�𒊏o
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

				//�O�p�`�̒��_�̍��W�𒊏o
				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				//�O�p�`�̎O�Ӄx�N�g�����Z�o
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				//�O�p�`�̖@���x�N�g��(����)���Z�o(BC,AB���Ɣ��]����)
				DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB,BC);
				

				//���ς̌��ʂ��v���X�Ȃ�Η�����(�����Ōv�Z�I���A���̃|���S����)
				//���C�Ɩ@���̓��ς��v�Z
				DirectX::XMVECTOR Dot =DirectX::XMVector3Dot(V,N) ;
				//���ς�P��̒l�ɂ���
				float d;
				DirectX::XMStoreFloat(&d, Dot);
				//�v���X�ȏ�Ȃ�v�Z�I��
				if (d>=0.0f)continue;

				//���C�ƕ��ʂ̌�_���Z�o
				//���C�̎n�_����O�p�`�̈ړ]�ւ̃x�N�g�����v�Z
				DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A,S);
				//�@���Ƃ��̃x�N�g���Ƃ̎ˉe���v�Z
				DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N,SA),Dot);
				float x;
				DirectX::XMStoreFloat(&x, X);
				if (x<.0f || x>neart)continue;//��_�܂ł̋��������܂łɌv�Z�����ŋߋ������
				                              //�傫���Ƃ��̓X�L�b�v
				DirectX::XMVECTOR P =DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V,X),S) ;
				//DirectX::XMVECTOR P =DirectX::XMVectorAdd(DirectX::XMVectorScale(V,X),S) ;

				//��_���O�p�`�̓����ɂ��邩����
				//1��
				DirectX::XMVECTOR PA     = DirectX::XMVectorSubtract(A,P) ;
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA,AB) ;
				DirectX::XMVECTOR Dot1   = DirectX::XMVector3Dot(Cross1,N);
				float dot1;
				DirectX::XMStoreFloat(&dot1,Dot1);
				//��󂪋t�����i�}�C�i�X�j��������O�p�`�̊O��
				if (dot1<0.0f)continue;

				//2��
				DirectX::XMVECTOR PB     = DirectX::XMVectorSubtract(B,P) ;
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB,BC) ;
				DirectX::XMVECTOR Dot2   = DirectX::XMVector3Dot(Cross2,N);
				float dot2;
				DirectX::XMStoreFloat(&dot2,Dot2);
				//��󂪋t�����i�}�C�i�X�j��������O�p�`�̊O��
				if (dot2 < 0.0f)continue;

				//3��
				DirectX::XMVECTOR PC     = DirectX::XMVectorSubtract(C,P) ;
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC,CA) ;
				DirectX::XMVECTOR Dot3   = DirectX::XMVector3Dot(Cross3,N);

				float dot3;
				DirectX::XMStoreFloat(&dot3,Dot3);
				//��󂪋t�����i�}�C�i�X�j��������O�p�`�̊O��
				if (dot3 < 0.0f)continue;

				//�ŋߋ������X�V
				neart = x;

				//��_�Ɩ@�����X�V
				HitPosition = P;
				HitNormal = N;
				materialIndex = subset.materialIndex;


			}
		}
		if (materialIndex >= 0)
		{
			//�O�p�`�̌�_���W�����[�J����Ԃ��烏�[���h��Ԃ֕ύX(�߂�)
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			//�q�b�g���ۑ�
			if (result.distance > distance)
			{
				//�O�p�`�̖@�������[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
				DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

				result.distance = distance;
				result.materialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.position, WorldPosition);
				DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
				hit = true;
			}
		}


	}
	return hit;

}
