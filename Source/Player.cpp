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

//コンストラクタ
Player::Player()
{
    model = new Model("Data/Model/chara/player.mdl");
  

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.015f;

    position.z = 10.0f;
}

//デストラクタ
Player::~Player()
{
    delete model;

    delete instanceOfStageMoveFloor;

    delete hitEffect;
}

//更新処理
void Player::Update(float elapsedTime)
{
  

    //ステート毎の処理
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Running:
        UpdateMoveState(elapsedTime);
        break;
    }

    //前方向
    Front.x = sinf(angle.y);//transform._31
    Front.y = 0.0f;
    Front.z = cosf(angle.y);//transform._33

    //オブジェクト行列を更新（行列更新処理）
    UpdateTransform();

    ////ジャンプ処理
    //InputJump();

    //速度処理計算
    UpdateVelocity(elapsedTime);

    ItemVsPlayer();

    PersonVsPlayer();

    projectileManager.Update(elapsedTime);

    //LaunchCola();

    CollisionProjectilesVsPerson();

    model->UpdateAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);
  
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
    projectileManager.Render(dc, shader);

}

//デバッグ用GUI描画
void Player::DrawDebugGUI()
{
    
}

//着地したときの呼ばれる
void Player::OnLanding()
{
    //現在のジャンプ回数をリセット
    jumpCount = 0;
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //移動ベクトルはXZ平面に水平なベクトルになるようにする

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        //単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //スティックの水平入力値をカメラ右方向に反映し、
    //スティックの垂直入力値をカメラ前方向に反映し、
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}

//移動入力処理
bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    //進行ベクトルがゼロベクトルでない場合は入力された
    return (moveVec.x != 0.0f || moveVec.z != 0.0f);
}

void Player::ItemVsPlayer()
{
    ItemManager& itemManager = ItemManager::Instance();

    int MentosCount = itemManager.GetItemCount();

    for (int i = 0; i < MentosCount; ++i)
    {
        Item* item = itemManager.GetItem(i);

        //衝突処理
        if (Collision::IntersectSphereVsCylinder(
            item->GetPosition(),
            item->GetRadius(),
            position,
            radius,
            height
        ))
        {
            itemManager.MentosIncrease();
          
            //アイテム破壊
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
        //衝突処理
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

    //直進弾丸発射
   
        //前方向
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);//transform._31
        dir.y = 0.0f;
        dir.z = cosf(angle.y);//transform._33
        //発射位置（プレイヤー腰あたり）
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height / 2.0f;
        pos.z = position.z;
        //発射
        FrontProjectile* projectile = new FrontProjectile(&projectileManager);
        projectile->Launch(dir, pos);


    
}

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();


    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    projectileManager.DrawDebugPrimitive();
}


void Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        //現在のジャンプ回数が最大のジャンプ回数より好かなかったら
        if (jumpCount < jumpLimit)
        {
            //現在のジャンプ回数を1増やす
            jumpCount++;
            //ジャンプ
            Jump(jumpSpeed);
        }
    }
}

//キャラの切り替え
void Player::ChangeCharacter()
{
    static bool prevLeftButtonDown = false;
    static bool prevRightButtonDown = false;

    Mouse& mouse = Input::Instance().GetMouse();

    if (mouse.GetButton() & Mouse::BTN_LEFT)
    {
        if (!prevLeftButtonDown)  // 前回フレームで左ボタンが押されていなかったら
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
        if (!prevRightButtonDown)  // 前回フレームで右ボタンが押されていなかったら
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

//コーラと人との衝突処理
void Player::CollisionProjectilesVsPerson()
{
    PeopleManager& peopleManager = PeopleManager::Instance();
   
    //全ての弾丸と全ての敵を総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    int peopleCount = peopleManager.GetPeopleCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < peopleCount; ++j)
        {
            People* people = peopleManager.GetPeople(j);

            //衝突処理
            
            if (Collision::IntersectSphereVsCylinder(
                //弾丸の位置
                projectile->GetPosition(),
                //弾丸の半径
                projectile->GetRadius(),
                //敵の位置
                people->GetPosition(),
                //敵の半径
                people->GetRadius(),
                //敵の高さ
                people->GetHeight()
               
                
            ))
            {
                //人を倒れるモーションを呼ぶ
                if (people->ApplyDamage(1, 0.5f))
                {

                    //	吹き飛ばす
                    
                       DirectX::XMFLOAT3 impulse;
                           const float power = 10.0f;//吹き飛ばす力

                           ///敵の位置を取得
                           const DirectX::XMFLOAT3& e = people->GetPosition();

                           //弾の位置を取得
                           const DirectX::XMFLOAT3& p = projectile->GetPosition();

                       //弾から敵の位置へのベクトルを取得
                       float vx = e.x - p.x;
                       float vz = e.z - p.z;

                       //そのベクトルを正規化(長さを計算し、長さで割る)
                       float lengthXZ = sqrtf(vx * vx + vz * vz);
                       vx /= lengthXZ;
                       vz /= lengthXZ;

                       //衝撃の値を設定(xzは正規化したベクトルをpower分スケーリング)
                       impulse.x = vx * power;
                       impulse.y = power * 0.5f;//yはちょっと浮かせる
                       impulse.z = vz * power;

                       people->AddImpulse(impulse);
                       count++;
                    //弾丸破棄
                       projectile->Destroy();
                }

            }
        }
    }
}

void Player::TransitionIdleState()
{
    state = State::Idle;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}

void Player::UpdateIdleState(float elapsedTime)
{
    //移動入力処理
    if (InputMove(elapsedTime))
        TransitionMoveState();
}

void Player::TransitionMoveState()
{
    state = State::Running;

    //走りアニメーション再生
    model->PlayAnimation(Anim_Run, true);
}

void Player::UpdateMoveState(float elapsedTime)
{
    //移動入力処理
    if (!InputMove(elapsedTime))
        TransitionIdleState();
}
