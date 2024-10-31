#include<imgui.h>
#include"Player.h"
#include"Input/Input.h"
#include"Camera.h"
#include"Graphics/graphics.h"
#include"EnemyManager.h"
#include"Collision.h"
#include"ProjectileStraight.h"
#include "ProjectileHoming.h"


//コンストラクタ
Player::Player()
{
	//model = new Model("Data/model/Mr.Incredible/Mr.Incredible.mdl");
	model = new Model("Data/Model/Jammo/Jammo.mdl");
	//model->PlayAnimation(0);

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	//ヒットエフェクト読み込み
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//待機ステートへ遷移
	TransitionIdleState();

}

//デストラクタ
Player::~Player()
{
	delete hitEffect;

	delete model;
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

	//速力処理更新
	UpdateVelocity(elapsedTime);

	//弾丸更新処理
	projectileManager.Update(elapsedTime);

	//プレイヤーと敵との衝突処理
	CollisionPlayerVsEnemies();

	//弾丸と敵の衝突処理
	CollisionProjectilesVsEnemies();

	//オブジェクト行列を更新
	UpdateTransform();

	//モデルアニメーション処理
	model->UpdateAnimation(elapsedTime);

	//モデル行列更新処理
	model->UpdateTransform(transform);
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

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);

	//弾丸描画処理
	projectileManager.Render(dc,shader);
}

//デバッグ用GUI描画
void Player::DrawdebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		//トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//位置
			ImGui::InputFloat3("Position", &position.x);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y= DirectX::XMConvertToDegrees(angle.y);
			a.z= DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//スケール
			ImGui::InputFloat3("Scale", &scale.x);

		}
	}
	ImGui::End();
}

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	//弾丸デバッグプリミティブ描画
	projectileManager.DrawDebugPrimitive();
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec()const
{
	//入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//カメラ方向とスティック入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//移動ベクトルはZX平面に水平なベクトルになるようにする

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
	float cameraFrontLength= sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
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
	vec.x = (cameraRightX*ax)+(cameraFrontX*ay);
	vec.z = (cameraRightZ*ax)+(cameraFrontZ*ay);
	//Y軸方向には移動しない
	vec.y = 0.0f;
	return vec;

}



//プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectCylonderVsCylinder(
			//プレイヤーの位置
			position,
            //プレイヤーの半径
			radius,
			//プレイヤーの高さ
			height,
			//敵の位置
			enemy->GetPosition(),
			//敵の半径
			enemy->GetRadius(),
			//敵の高さ
			enemy->GetHeight(),
			//押し出し後の位置
			outPosition
			))
		{
			//敵の真上付近に当たったかをチェック
			//プレイヤーの位置をXMVECTORに変換
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			//敵の位置をXMVECTORに変換
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			//プレイヤー方向へのベクトルを取得し
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			//そのベクトルを正規化
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			//上から踏んづけた場合は小ジャンプする
			if (normal.y > 0.8f)
			{
				//小ジャンプする
				Jump(jumpSpeed * 0.5f);

				//1ダメージを与える
				enemy->ApplyDamage(1,0.5f);
			}
			else
			{
				//敵の位置に押し出し後の位置設定
				enemy->setPosition(outPosition);
			}
			////敵の真上付近に当たったかをチェック
			//if (0.5f>=(enemy->GetHeight()+enemy->Getposition().y- position.y))
			//{
			//	//真上付近だったら小ジャンプ（半分の力でジャンプ）
			//	Jump(jumpSpeed * 0.5f);
			//}
			
			  

			
		}
	}

}

//着地した時に呼ばれる
void Player::OnLanding()
{
	//ジャンプ回数リセット
	jumpCount = 0;

	//下方向の速度が一定以上なら着地ステートへ
	if (velocity.y != 0.0f)
	{
		TransitionLandState();
	}
}


//ジャンプ入力処理
bool Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		//現在のジャンプ回数が最大のジャンプ回数より少なかったら
		if (jumpCount < jumpLimit)
		{
			//現在のジャンプ回数を1増やす
			jumpCount++;

			//ジャンプ
			Jump(jumpSpeed);

			//ジャンプした
			return true;
		}

		
	}
	return false;
}

//弾丸入力処理
void Player::InputProjectile()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//直進弾丸発射
	if (gamePad.GetButtonDown() & GamePad::BTN_X)
	{
		//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);//transform._31
		dir.y = 0.0f;
		dir.z = cosf(angle.y);//transform._33
		//発射位置（プレイヤー腰あたり）
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y+height/2.0f;
		pos.z = position.z;
		//発射
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
		

	}
	//追尾弾丸発射
	if (gamePad.GetButtonDown() & GamePad::BTN_Y)
	{
		//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0.0f;
		dir.z = cosf(angle.y);

		//発射位置(プレイヤーの腰あたり)
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y+height*0.5f;
		pos.z = position.z;

		//ターゲット（デフォルトではプレイヤーの前方)
		DirectX::XMFLOAT3 target;
		target.x = position.x + dir.x * 1000.0f;
		target.y = position.y + dir.y * 1000.0f;
		target.z = position.z + dir.z * 1000.0f;

		//一番近くの敵をターゲットにする
		//一番遠い場所を設定しておく
		float dist = FLT_MAX;

		//敵マネジャーを取得
		EnemyManager& enemyManager = EnemyManager::Instance();

		//敵の数を取得
		int enemyCount = enemyManager.GetEnemyCount();

		//一番近い敵を敵の数だけチェックする
		for (int i=0;i<enemyCount;++i)
		{
			//敵との距離判定
			//1番目の敵を取得
			Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
			//プレイヤー（自分)の位置をXMVECTORに変換
			DirectX::XMVECTOR playerPosVec = DirectX::XMLoadFloat3(&position);
			//敵の位置をXMVECTORに変換
			DirectX::XMVECTOR enemyPosVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
			//敵方向へのベクトルを計算
			DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(enemyPosVec, playerPosVec);
			//そのベクトルの長さを計算し
			DirectX::XMVECTOR D = DirectX::XMVector3Length(directionVec);
			//単一の値にする
			float d;
			DirectX::XMStoreFloat(&d, D);

			//今まで計算した距離の中で一番小さい距離なら
			if (d < dist)
			{
				dist = d;

				//targetの位置を更新(敵の位置を設定)
				target = enemy->GetPosition();

				//targetのYだけ腰の高さにしておく
				target.y += enemy->GetHeight() * 0.5f;
			}
		}


		//発射
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
//待機ステートへ遷移
void Player::TransitionIdleState()
{
	state = State::Idle;

	//待機アニメーション再生
	model->PlayAnimation(Anim_Idle, true);
}

//待機ステート更新
void Player::UpdateIdleState(float elapsedTime)
{
	

	//移動入力処理
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
		
	}

	//ジャンプ入力処理
	if (InputJump())
	{
		TransitionJumpState();
	}

	//攻撃入力処理
	if (InputAttack())
	{
		TransitionAttackState();
	}

	//ジャンプ入力処理
	InputJump();

	//弾丸入力処理
	InputProjectile();
}

//待機ステートへ遷移
void Player::TransitionMoveState()
{
	state = State::Move;

	//走りアニメーション再生
	model->PlayAnimation(Anim_Running, true);
}

//移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{
	//移動入力処理
	if (!InputMove(elapsedTime))
	{
		TransitionIdleState();
	}
	//ジャンプ入力処理
	if (InputJump())
	{
		TransitionJumpState();
	}
	//攻撃入力処理
	if (InputAttack())
	{
		TransitionAttackState();
	}

	//ジャンプ入力処理
	InputJump();

	//弾丸入力処理
	InputProjectile();
}

//ジャンプステートへ遷移
void Player::TransitionJumpState()
{
	state = State::Jump;

	//ジャンプアニメーション再生
	model->PlayAnimation(Anim_jump, false);
}

//ジャンプステート更新処理
void Player::UpdateJumpState(float elapsedTime)
{
	//移動入力処理
	if (!InputMove(elapsedTime))
	{
		return;
	}
	//ジャンプ入力処理
	if (InputJump())
	{
		TransitionJumpState();
	}


	//弾丸入力処理
	InputProjectile();
}

//着地ステートへ遷移
void Player::TransitionLandState()
{
	state = State::Land;

	//着地アニメーション再生
	model->PlayAnimation(Anim_Landing, false);
}

//着地ステート更新処理
void Player::UpdateLandState(float elapsedTime)
{
	//着地中,移動入力が合う場合に他の状態への遷移を行う
    if (!model->IsPlayAnimation())
    {
    	TransitionIdleState();
    }
    //弾丸入力処理
    InputProjectile();
}

//攻撃ステートへ遷移
void Player::TransitionAttackState()
{
	state = State::Attack;

	//攻撃アニメーション再生
	model->PlayAnimation(Anim_Attack, false);
}

//攻撃ステート更新処理
void Player::UpdateAttackState(float elapsedTime)
{
	//何もアニメーションが入っていなければ待機へ遷移
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}

//弾丸と敵との衝突処理
void Player::CollisionProjectilesVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//全ての弾丸と全ての敵を総当たりで衝突処理
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* projectile = projectileManager.GetProjectile(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);

			//衝突処理
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsCylinder(
				//弾丸の位置
				projectile->GetPosition(),
				//弾丸の半径
				projectile->GetRadius(),
				//敵の位置
				enemy->GetPosition(),
				//敵の半径
				enemy->GetRadius(),
				//敵の高さ
				enemy->GetHeight(),			
				//押し出し後の位置
				outPosition
			))
			{
				//ダメージを与える
				if (enemy->ApplyDamage(1, 0.5f))
				{
					//	吹き飛ばす
					{
						DirectX::XMFLOAT3 impulse;
						const float power = 10.0f;//吹き飛ばす力

						///敵の位置を取得
						const DirectX::XMFLOAT3& e = enemy->GetPosition();

						//弾の位置を取得
						const DirectX::XMFLOAT3& p = projectile->GetPosition();

						//弾から敵の位置へのベクトルを取得
						float vx = e.x-p.x;
						float vz = e.z-p.z;

						//そのベクトルを正規化(長さを計算し、長さで割る)
						float lengthXZ =sqrtf(vx*vx+vz*vz);
						vx /= lengthXZ;
						vz /= lengthXZ;

						//衝撃の値を設定(xzは正規化したベクトルをpower分スケーリング)
						impulse.x = vx*power;
						impulse.y = power * 0.5f;//yはちょっと浮かせる
						impulse.z = vz * power;

						enemy->AddImpulse(impulse);
					}

					//ヒットエフェクト再生
					{
						DirectX::XMFLOAT3 e = enemy->GetPosition();
						e.y += enemy->GetHeight() * 0.5f;
						hitEffect->Play(e);
					}

					//弾丸破棄
					projectile->Destroy();
				}




			}
		}
	}
	

}

