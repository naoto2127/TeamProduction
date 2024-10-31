#include"ItemManager.h"
#include"Collision.h"

//更新処理
void ItemManager::Update(float elapsedTime)
{
	for (Item* item : itemes)
	{
		item->Update(elapsedTime);
	}


	//破棄処理
	for (Item* item : removes)
	{
		//std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Item*>::iterator it = std::find(itemes.begin(), itemes.end(), item);
		if (it != itemes.end())
		{
			itemes.erase(it);
		}

		//弾丸の破棄処理
		delete item;
	}
	removes.clear();

	if (MentosNum > 0)
	{
		MentosFlag = true;
	}
	else
	{
		MentosFlag = false;
	}

	if (MentosFlag)
	{
		time += elapsedTime;

		if (time >= 10.0)
		{
			MentosNum--;
			time = 0.0f;
		}
	}



	////敵同士の衝突処理
	CollisionItemVsItemes();


}

//描画処理
void ItemManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (Item* item : itemes)
	{
		item->Render(context, shader);
	}
}

//デバッグプリミティブ描画
void ItemManager::DrawDebugPrimitive()
{
	for (Item* item : itemes)
	{
		item->DrawDebugPrimitive();
	}
}

//エネミー登録
void ItemManager::Register(Item* item)
{
	itemes.emplace_back(item);
}

//エネミー全削除
void ItemManager::Clear()
{
	for (Item* item : itemes)
	{
		delete item;
	}

	itemes.clear();
}

void ItemManager::Remove(Item* item)
{
	//破棄リストに追加
	removes.insert(item);
}

void ItemManager::MentosIncrease()
{
	MentosNum++;
}

//エネミー同士の衝突処理
void ItemManager::CollisionItemVsItemes()
{
	//敵の数を取得
	int numEnemies = GetItemCount();
	//敵の数分回す（カウンターi）
	for (int i = 0; i < numEnemies; ++i)
	{
		//敵Aを取得
		Item* itemA = GetItem(i);
		//敵の数分回す(カウンターj※iとjは違う数になるように(同じ敵同士は衝突しないから)）
		for (int j = 0; j < numEnemies; ++j)
		{
			if (i == j)// 同じ敵同士は衝突しないため、同じインデックスの場合はスキップ           
				continue;
			//敵Bを取得
			Item* itemB = GetItem(j);
			//押し出し後の位置
			DirectX::XMFLOAT3 newPositionB;

			////衝突処理
			//if (Collision::IntersectCylonderVsCylinder(
			//	//敵Aの位置
			//	itemA->GetPosition(),
			//	//敵Aの半径
			//	itemA->GetRadius(),
			//	//敵Aの高さ
			//	itemA->GetHeight(),
			//	//敵Bの位置
			//	itemB->GetPosition(),
			//	//敵Bの半径
			//	itemB->GetRadius(),
			//	//敵Bの高さ
			//	itemB->GetHeight(),
			//	//押し出し後の位置
			//	newPositionB))
			//{
			//	//敵Bに押し出し後の位置を設定
			//	itemB->setPosition(newPositionB);
			//}
		}
	}
}