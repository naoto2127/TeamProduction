#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Collision.h"
#include "ItemManager.h"
#include "peopleManager.h"
#include "FrontProjectile.h"
#include"Projectile.h"
#include "ProjectileManager.h"
#include "person.h"

bool poseFlg[4] = {};
int count;

//�R���X�g���N�^
Player::Player()
{
    model = new Model("Data/Model/chara/player.mdl");
  

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.015f;

    position.z = 10.0f;
}

//�f�X�g���N�^
Player::~Player()
{
    delete model;

    delete instanceOfStageMoveFloor;

    delete hitEffect;
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
    case State::Running:
        UpdateMoveState(elapsedTime);
        break;
    }

    //�O����
    Front.x = sinf(angle.y);//transform._31
    Front.y = 0.0f;
    Front.z = cosf(angle.y);//transform._33

    //�I�u�W�F�N�g�s����X�V�i�s��X�V�����j
    UpdateTransform();

    ////�W�����v����
    //InputJump();

    //���x�����v�Z
    UpdateVelocity(elapsedTime);

    ItemVsPlayer();

    PersonVsPlayer();

    projectileManager.Update(elapsedTime);

    //LaunchCola();

    CollisionProjectilesVsPerson();

    model->UpdateAnimation(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);
  
}

//�`�揈��
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
    projectileManager.Render(dc, shader);

}

//�f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
    
}

//���n�����Ƃ��̌Ă΂��
void Player::OnLanding()
{
    //���݂̃W�����v�񐔂����Z�b�g
    jumpCount = 0;
}

//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //�ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

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
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
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
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
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

void Player::ItemVsPlayer()
{
    ItemManager& itemManager = ItemManager::Instance();

    int MentosCount = itemManager.GetItemCount();

    for (int i = 0; i < MentosCount; ++i)
    {
        Item* item = itemManager.GetItem(i);

        //�Փˏ���
        if (Collision::IntersectSphereVsCylinder(
            item->GetPosition(),
            item->GetRadius(),
            position,
            radius,
            height
        ))
        {
            itemManager.MentosIncrease();
          
            //�A�C�e���j��
            item->Destroy();
            
        }
    }
}

void Player::PersonVsPlayer()
{
    PeopleManager& peopleManager = PeopleManager::Instance();

    int MentosCount = peopleManager.GetPeopleCount();

    for (int i = 0; i < MentosCount; ++i)
    {
        People* people = peopleManager.GetPeople(i);
        DirectX::XMFLOAT3 outPosition;
        //�Փˏ���
        if (Collision::IntersectCylonderVsCylinder(
            position,
            radius,
            height,
            people->GetPosition(),
            people->GetRadius(),
            people->GetHeight(),
            outPosition
        ))
        {
            people->SetPosition(outPosition);
 
        }
    }
}

void Player::LaunchCola()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //���i�e�۔���
   
        //�O����
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);//transform._31
        dir.y = 0.0f;
        dir.z = cosf(angle.y);//transform._33
        //���ˈʒu�i�v���C���[��������j
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height / 2.0f;
        pos.z = position.z;
        //����
        FrontProjectile* projectile = new FrontProjectile(&projectileManager);
        projectile->Launch(dir, pos);


    
}

//�f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();


    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    projectileManager.DrawDebugPrimitive();
}


void Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        //���݂̃W�����v�񐔂��ő�̃W�����v�񐔂��D���Ȃ�������
        if (jumpCount < jumpLimit)
        {
            //���݂̃W�����v�񐔂�1���₷
            jumpCount++;
            //�W�����v
            Jump(jumpSpeed);
        }
    }
}

//�L�����̐؂�ւ�
void Player::ChangeCharacter()
{
    static bool prevLeftButtonDown = false;
    static bool prevRightButtonDown = false;

    Mouse& mouse = Input::Instance().GetMouse();

    if (mouse.GetButton() & Mouse::BTN_LEFT)
    {
        if (!prevLeftButtonDown)  // �O��t���[���ō��{�^����������Ă��Ȃ�������
        {
            CharacterSlot += 1;
            if (CharacterSlot > 3)
            {
                CharacterSlot = 0;
            }
        }
        prevLeftButtonDown = true;
    }
    else
    {
        prevLeftButtonDown = false;
    }

    if (mouse.GetButton() & Mouse::BTN_RIGHT)
    {
        if (!prevRightButtonDown)  // �O��t���[���ŉE�{�^����������Ă��Ȃ�������
        {
            CharacterSlot = 0;
        }
        prevRightButtonDown = true;
    }
    else
    {
        prevRightButtonDown = false;
    }
}

//�R�[���Ɛl�Ƃ̏Փˏ���
void Player::CollisionProjectilesVsPerson()
{
    PeopleManager& peopleManager = PeopleManager::Instance();
   
    //�S�Ă̒e�ۂƑS�Ă̓G�𑍓�����ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();
    int peopleCount = peopleManager.GetPeopleCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < peopleCount; ++j)
        {
            People* people = peopleManager.GetPeople(j);

            //�Փˏ���
            
            if (Collision::IntersectSphereVsCylinder(
                //�e�ۂ̈ʒu
                projectile->GetPosition(),
                //�e�ۂ̔��a
                projectile->GetRadius(),
                //�G�̈ʒu
                people->GetPosition(),
                //�G�̔��a
                people->GetRadius(),
                //�G�̍���
                people->GetHeight()
               
                
            ))
            {
                //�l��|��郂�[�V�������Ă�
                if (people->ApplyDamage(1, 0.5f))
                {

                    //	������΂�
                    
                       DirectX::XMFLOAT3 impulse;
                           const float power = 10.0f;//������΂���

                           ///�G�̈ʒu���擾
                           const DirectX::XMFLOAT3& e = people->GetPosition();

                           //�e�̈ʒu���擾
                           const DirectX::XMFLOAT3& p = projectile->GetPosition();

                       //�e����G�̈ʒu�ւ̃x�N�g�����擾
                       float vx = e.x - p.x;
                       float vz = e.z - p.z;

                       //���̃x�N�g���𐳋K��(�������v�Z���A�����Ŋ���)
                       float lengthXZ = sqrtf(vx * vx + vz * vz);
                       vx /= lengthXZ;
                       vz /= lengthXZ;

                       //�Ռ��̒l��ݒ�(xz�͐��K�������x�N�g����power���X�P�[�����O)
                       impulse.x = vx * power;
                       impulse.y = power * 0.5f;//y�͂�����ƕ�������
                       impulse.z = vz * power;

                       people->AddImpulse(impulse);
                       count++;
                    //�e�۔j��
                       projectile->Destroy();
                }

            }
        }
    }
}

void Player::TransitionIdleState()
{
    state = State::Idle;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
}

void Player::UpdateIdleState(float elapsedTime)
{
    //�ړ����͏���
    if (InputMove(elapsedTime))
        TransitionMoveState();
}

void Player::TransitionMoveState()
{
    state = State::Running;

    //����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Run, true);
}

void Player::UpdateMoveState(float elapsedTime)
{
    //�ړ����͏���
    if (!InputMove(elapsedTime))
        TransitionIdleState();
}
