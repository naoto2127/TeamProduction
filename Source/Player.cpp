#include<imgui.h>
#include"Player.h"
#include"Input/Input.h"
#include"Camera.h"
#include"Graphics/graphics.h"
#include"EnemyManager.h"
#include"Collision.h"
#include"ProjectileStraight.h"
#include "ProjectileHoming.h"


//�R���X�g���N�^
Player::Player()
{
	//model = new Model("Data/model/Mr.Incredible/Mr.Incredible.mdl");
	model = new Model("Data/Model/Jammo/Jammo.mdl");
	//model->PlayAnimation(0);

	//���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;

	//�q�b�g�G�t�F�N�g�ǂݍ���
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//�ҋ@�X�e�[�g�֑J��
	TransitionIdleState();

}

//�f�X�g���N�^
Player::~Player()
{
	delete hitEffect;

	delete model;
}

//�X�V����
void Player::Update(float elapsedTime)
{
	

	//�X�e�[�g���̏���
	switch (state)
	{
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;
	case State::Move:
		UpdateMoveState(elapsedTime);
		break;
	case State::Jump:
		UpdateJumpState(elapsedTime);
		break;
	case State::Land:
		UpdateLandState(elapsedTime);
		break;
	case State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	}

	//���͏����X�V
	UpdateVelocity(elapsedTime);

	//�e�ۍX�V����
	projectileManager.Update(elapsedTime);

	//�v���C���[�ƓG�Ƃ̏Փˏ���
	CollisionPlayerVsEnemies();

	//�e�ۂƓG�̏Փˏ���
	CollisionProjectilesVsEnemies();

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���A�j���[�V��������
	model->UpdateAnimation(elapsedTime);

	//���f���s��X�V����
	model->UpdateTransform(transform);
}


//�ړ����͏���
bool Player::InputMove(float elapsedTime)
{
	//�i�s�x�N�g���擾
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	//�ړ�����
	Move(moveVec.x, moveVec.z, moveSpeed);

	//���񏈗�
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	//�i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
	return (moveVec.x != 0.0f || moveVec.z != 0.0f);
}

//�`�揈��
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);

	//�e�ە`�揈��
	projectileManager.Render(dc,shader);
}

//�f�o�b�O�pGUI�`��
void Player::DrawdebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		//�g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//�ʒu
			ImGui::InputFloat3("Position", &position.x);
			//��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y= DirectX::XMConvertToDegrees(angle.y);
			a.z= DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//�X�P�[��
			ImGui::InputFloat3("Scale", &scale.x);

		}
	}
	ImGui::End();
}

//�f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//�Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	//�e�ۃf�o�b�O�v���~�e�B�u�`��
	projectileManager.DrawDebugPrimitive();
}

//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec()const
{
	//���͏����擾
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//�J���������ƃX�e�B�b�N���͒l�ɂ���Đi�s�������v�Z����
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//�ړ��x�N�g����ZX���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

	//�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		//�P�ʃx�N�g����
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	//�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength= sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		//�P�ʃx�N�g����
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	//�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
	//�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
	//�i�s�x�N�g�����v�Z����
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX*ax)+(cameraFrontX*ay);
	vec.z = (cameraRightZ*ax)+(cameraFrontZ*ay);
	//Y�������ɂ͈ړ����Ȃ�
	vec.y = 0.0f;
	return vec;

}



//�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//�S�Ă̓G�Ƒ�������ŏՓˏ���
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//�Փˏ���
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectCylonderVsCylinder(
			//�v���C���[�̈ʒu
			position,
            //�v���C���[�̔��a
			radius,
			//�v���C���[�̍���
			height,
			//�G�̈ʒu
			enemy->GetPosition(),
			//�G�̔��a
			enemy->GetRadius(),
			//�G�̍���
			enemy->GetHeight(),
			//�����o����̈ʒu
			outPosition
			))
		{
			//�G�̐^��t�߂ɓ������������`�F�b�N
			//�v���C���[�̈ʒu��XMVECTOR�ɕϊ�
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			//�G�̈ʒu��XMVECTOR�ɕϊ�
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			//�v���C���[�����ւ̃x�N�g�����擾��
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			//���̃x�N�g���𐳋K��
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			//�ォ�瓥��Â����ꍇ�͏��W�����v����
			if (normal.y > 0.8f)
			{
				//���W�����v����
				Jump(jumpSpeed * 0.5f);

				//1�_���[�W��^����
				enemy->ApplyDamage(1,0.5f);
			}
			else
			{
				//�G�̈ʒu�ɉ����o����̈ʒu�ݒ�
				enemy->setPosition(outPosition);
			}
			////�G�̐^��t�߂ɓ������������`�F�b�N
			//if (0.5f>=(enemy->GetHeight()+enemy->Getposition().y- position.y))
			//{
			//	//�^��t�߂������珬�W�����v�i�����̗͂ŃW�����v�j
			//	Jump(jumpSpeed * 0.5f);
			//}
			
			  

			
		}
	}

}

//���n�������ɌĂ΂��
void Player::OnLanding()
{
	//�W�����v�񐔃��Z�b�g
	jumpCount = 0;

	//�������̑��x�����ȏ�Ȃ璅�n�X�e�[�g��
	if (velocity.y != 0.0f)
	{
		TransitionLandState();
	}
}


//�W�����v���͏���
bool Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		//���݂̃W�����v�񐔂��ő�̃W�����v�񐔂�菭�Ȃ�������
		if (jumpCount < jumpLimit)
		{
			//���݂̃W�����v�񐔂�1���₷
			jumpCount++;

			//�W�����v
			Jump(jumpSpeed);

			//�W�����v����
			return true;
		}

		
	}
	return false;
}

//�e�ۓ��͏���
void Player::InputProjectile()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//���i�e�۔���
	if (gamePad.GetButtonDown() & GamePad::BTN_X)
	{
		//�O����
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);//transform._31
		dir.y = 0.0f;
		dir.z = cosf(angle.y);//transform._33
		//���ˈʒu�i�v���C���[��������j
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y+height/2.0f;
		pos.z = position.z;
		//����
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
		

	}
	//�ǔ��e�۔���
	if (gamePad.GetButtonDown() & GamePad::BTN_Y)
	{
		//�O����
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0.0f;
		dir.z = cosf(angle.y);

		//���ˈʒu(�v���C���[�̍�������)
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y+height*0.5f;
		pos.z = position.z;

		//�^�[�Q�b�g�i�f�t�H���g�ł̓v���C���[�̑O��)
		DirectX::XMFLOAT3 target;
		target.x = position.x + dir.x * 1000.0f;
		target.y = position.y + dir.y * 1000.0f;
		target.z = position.z + dir.z * 1000.0f;

		//��ԋ߂��̓G���^�[�Q�b�g�ɂ���
		//��ԉ����ꏊ��ݒ肵�Ă���
		float dist = FLT_MAX;

		//�G�}�l�W���[���擾
		EnemyManager& enemyManager = EnemyManager::Instance();

		//�G�̐����擾
		int enemyCount = enemyManager.GetEnemyCount();

		//��ԋ߂��G��G�̐������`�F�b�N����
		for (int i=0;i<enemyCount;++i)
		{
			//�G�Ƃ̋�������
			//1�Ԗڂ̓G���擾
			Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
			//�v���C���[�i����)�̈ʒu��XMVECTOR�ɕϊ�
			DirectX::XMVECTOR playerPosVec = DirectX::XMLoadFloat3(&position);
			//�G�̈ʒu��XMVECTOR�ɕϊ�
			DirectX::XMVECTOR enemyPosVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
			//�G�����ւ̃x�N�g�����v�Z
			DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(enemyPosVec, playerPosVec);
			//���̃x�N�g���̒������v�Z��
			DirectX::XMVECTOR D = DirectX::XMVector3Length(directionVec);
			//�P��̒l�ɂ���
			float d;
			DirectX::XMStoreFloat(&d, D);

			//���܂Ōv�Z���������̒��ň�ԏ����������Ȃ�
			if (d < dist)
			{
				dist = d;

				//target�̈ʒu���X�V(�G�̈ʒu��ݒ�)
				target = enemy->GetPosition();

				//target��Y�������̍����ɂ��Ă���
				target.y += enemy->GetHeight() * 0.5f;
			}
		}


		//����
		ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
		projectile->Launch(dir, pos, target);

	}
}
bool Player::InputAttack()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		return true;
	}
	return false;
}
//�ҋ@�X�e�[�g�֑J��
void Player::TransitionIdleState()
{
	state = State::Idle;

	//�ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Idle, true);
}

//�ҋ@�X�e�[�g�X�V
void Player::UpdateIdleState(float elapsedTime)
{
	

	//�ړ����͏���
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
		
	}

	//�W�����v���͏���
	if (InputJump())
	{
		TransitionJumpState();
	}

	//�U�����͏���
	if (InputAttack())
	{
		TransitionAttackState();
	}

	//�W�����v���͏���
	InputJump();

	//�e�ۓ��͏���
	InputProjectile();
}

//�ҋ@�X�e�[�g�֑J��
void Player::TransitionMoveState()
{
	state = State::Move;

	//����A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Running, true);
}

//�ړ��X�e�[�g�X�V����
void Player::UpdateMoveState(float elapsedTime)
{
	//�ړ����͏���
	if (!InputMove(elapsedTime))
	{
		TransitionIdleState();
	}
	//�W�����v���͏���
	if (InputJump())
	{
		TransitionJumpState();
	}
	//�U�����͏���
	if (InputAttack())
	{
		TransitionAttackState();
	}

	//�W�����v���͏���
	InputJump();

	//�e�ۓ��͏���
	InputProjectile();
}

//�W�����v�X�e�[�g�֑J��
void Player::TransitionJumpState()
{
	state = State::Jump;

	//�W�����v�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_jump, false);
}

//�W�����v�X�e�[�g�X�V����
void Player::UpdateJumpState(float elapsedTime)
{
	//�ړ����͏���
	if (!InputMove(elapsedTime))
	{
		return;
	}
	//�W�����v���͏���
	if (InputJump())
	{
		TransitionJumpState();
	}


	//�e�ۓ��͏���
	InputProjectile();
}

//���n�X�e�[�g�֑J��
void Player::TransitionLandState()
{
	state = State::Land;

	//���n�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Landing, false);
}

//���n�X�e�[�g�X�V����
void Player::UpdateLandState(float elapsedTime)
{
	//���n��,�ړ����͂������ꍇ�ɑ��̏�Ԃւ̑J�ڂ��s��
    if (!model->IsPlayAnimation())
    {
    	TransitionIdleState();
    }
    //�e�ۓ��͏���
    InputProjectile();
}

//�U���X�e�[�g�֑J��
void Player::TransitionAttackState()
{
	state = State::Attack;

	//�U���A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Attack, false);
}

//�U���X�e�[�g�X�V����
void Player::UpdateAttackState(float elapsedTime)
{
	//�����A�j���[�V�����������Ă��Ȃ���Αҋ@�֑J��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}

//�e�ۂƓG�Ƃ̏Փˏ���
void Player::CollisionProjectilesVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//�S�Ă̒e�ۂƑS�Ă̓G�𑍓�����ŏՓˏ���
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* projectile = projectileManager.GetProjectile(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);

			//�Փˏ���
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsCylinder(
				//�e�ۂ̈ʒu
				projectile->GetPosition(),
				//�e�ۂ̔��a
				projectile->GetRadius(),
				//�G�̈ʒu
				enemy->GetPosition(),
				//�G�̔��a
				enemy->GetRadius(),
				//�G�̍���
				enemy->GetHeight(),			
				//�����o����̈ʒu
				outPosition
			))
			{
				//�_���[�W��^����
				if (enemy->ApplyDamage(1, 0.5f))
				{
					//	������΂�
					{
						DirectX::XMFLOAT3 impulse;
						const float power = 10.0f;//������΂���

						///�G�̈ʒu���擾
						const DirectX::XMFLOAT3& e = enemy->GetPosition();

						//�e�̈ʒu���擾
						const DirectX::XMFLOAT3& p = projectile->GetPosition();

						//�e����G�̈ʒu�ւ̃x�N�g�����擾
						float vx = e.x-p.x;
						float vz = e.z-p.z;

						//���̃x�N�g���𐳋K��(�������v�Z���A�����Ŋ���)
						float lengthXZ =sqrtf(vx*vx+vz*vz);
						vx /= lengthXZ;
						vz /= lengthXZ;

						//�Ռ��̒l��ݒ�(xz�͐��K�������x�N�g����power���X�P�[�����O)
						impulse.x = vx*power;
						impulse.y = power * 0.5f;//y�͂�����ƕ�������
						impulse.z = vz * power;

						enemy->AddImpulse(impulse);
					}

					//�q�b�g�G�t�F�N�g�Đ�
					{
						DirectX::XMFLOAT3 e = enemy->GetPosition();
						e.y += enemy->GetHeight() * 0.5f;
						hitEffect->Play(e);
					}

					//�e�۔j��
					projectile->Destroy();
				}




			}
		}
	}
	

}

