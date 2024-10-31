#include"PeopleManager.h"
#include"Collision.h"

//更新処理
void PeopleManager::Update(float elapsedTime)
{
	for (People* people : humanes)
	{
		people->Update(elapsedTime);
	}


	for (People* people : humanes)
	{
		if (people->GetPosition().z >= 10)
		{
			people->Destroy();
		}
		if (people->GetPosition().z <= -10)
		{
			people->Destroy();
		}
	}

	//破棄処理
	for (People*people  : removes)
	{
		//std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<People*>::iterator it = std::find(humanes.begin(), humanes.end(), people);
		if (it != humanes.end())
		{
			humanes.erase(it);
		}

		//弾丸の破棄処理
		delete people;
	}
	removes.clear();



	////敵同士の衝突処理
	//CollisionPeopleVsPeoplees();


}

//描画処理
void PeopleManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (People* people : humanes)
	{
		people->Render(context, shader);
	}
}

//デバッグプリミティブ描画
void PeopleManager::DrawDebugPrimitive()
{
	for (People* people : humanes)
	{
		people->DrawDebugPrimitive();
	}
}

//エネミー登録
void PeopleManager::Register(People* people)
{
	humanes.emplace_back(people);
}

//エネミー全削除
void PeopleManager::Clear()
{
	for (People* people : humanes)
	{
		delete people;
	}

	humanes.clear();
}

void PeopleManager::Remove(People* people)
{
	//破棄リストに追加
	removes.insert(people);
}

////エネミー同士の衝突処理
//void PeopleManager::CollisionPeopleVsPeoplees()
//{
//	//敵の数を取得
//	int numEnemies = GetPeopleCount();
//	//敵の数分回す（カウンターi）
//	for (int i = 0; i < numEnemies; ++i)
//	{
//		//敵Aを取得
//		People* peopleA = GetPeople(i);
//		//敵の数分回す(カウンターj※iとjは違う数になるように(同じ敵同士は衝突しないから)）
//		for (int j = 0; j < numEnemies; ++j)
//		{
//			if (i == j)// 同じ敵同士は衝突しないため、同じインデックスの場合はスキップ           
//				continue;
//			//敵Bを取得
//			People* peopleB = GetPeople(j);
//			//押し出し後の位置
//			DirectX::XMFLOAT3 newPositionB;
//
//			////衝突処理
//			//if (Collision::IntersectCylonderVsCylinder(
//			//	//敵Aの位置
//			//	peopleA->GetPosition(),
//			//	//敵Aの半径
//			//	peopleA->GetRadius(),
//			//	//敵Aの高さ
//			//	peopleA->GetHeight(),
//			//	//敵Bの位置
//			//	peopleB->GetPosition(),
//			//	//敵Bの半径
//			//	peopleB->GetRadius(),
//			//	//敵Bの高さ
//			//	peopleB->GetHeight(),
//			//	//押し出し後の位置
//			//	newPositionB))
//			//{
//			//	//敵Bに押し出し後の位置を設定
//			//	peopleB->setPosition(newPositionB);
//			//}
//		}
//	}
//}