#include"EnemyManager.h"
#include"Collision.h"

//更新処理
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}


	//破棄処理
	for (Enemy*enemy : removes)
	{
		//std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		//弾丸の破棄処理
		delete enemy;
	}
	removes.clear();
	


	////敵同士の衝突処理
	CollisionEnemyVsEnemies();
	

}

//描画処理
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Render(context, shader);
	}
}

//デバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
	for (Enemy* enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
	enemies.emplace_back(enemy);
}

//エネミー全削除
void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}

	enemies.clear();
}

void EnemyManager::Remove(Enemy* enemy)
{
	//破棄リストに追加
	removes.insert(enemy);
}

//エネミー同士の衝突処理
void EnemyManager::CollisionEnemyVsEnemies()
{
	//敵の数を取得
	int numEnemies = GetEnemyCount();
	//敵の数分回す（カウンターi）
	for (int i = 0; i < numEnemies;++i)
	{
		//敵Aを取得
		Enemy* enemyA = GetEnemy(i);
		//敵の数分回す(カウンターj※iとjは違う数になるように(同じ敵同士は衝突しないから)）
		for (int j = 0; j < numEnemies; ++j)
		{
			if (i == j)// 同じ敵同士は衝突しないため、同じインデックスの場合はスキップ           
				continue;
			//敵Bを取得
			Enemy* enemyB = GetEnemy(j);
			//押し出し後の位置
			DirectX::XMFLOAT3 newPositionB;

			//衝突処理
			if (Collision::IntersectCylonderVsCylinder(
				//敵Aの位置
				enemyA->GetPosition(),
				//敵Aの半径
				enemyA->GetRadius(),
				//敵Aの高さ
				enemyA->GetHeight(),
				//敵Bの位置
				enemyB->GetPosition(),
				//敵Bの半径
				enemyB->GetRadius(),
				//敵Bの高さ
				enemyB->GetHeight(),
				//押し出し後の位置
				newPositionB))
			{
				//敵Bに押し出し後の位置を設定
				enemyB->setPosition(newPositionB);
			}
		}
	}
}