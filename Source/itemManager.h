#pragma once

#include<vector>
#include<set>
#include"item.h"

//アイテムマネージャー
class ItemManager
{
private:
	ItemManager() {}
	~ItemManager() {}
public:
	//唯一のインスタンス取得
	static ItemManager& Instance()
	{
		static ItemManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//アイテム登録
	void Register(Item* item);

	//アイテム完全削除
	void Clear();

	//デバッグプリミティブ描画
	void DrawDebugPrimitive();

	//アイテム数取得
	int GetItemCount() const { return static_cast<int>(itemes.size()); }

	//アイテム取得
	Item* GetItem(int index) { return itemes.at(index); }

	//アイテム削除
	void Remove(Item* item);

	//メントス取得フラグ
	const bool& GetMentos() const { return MentosFlag; }
	//メントス取得個数
	const int& GetMentosNum() const { return MentosNum; }

	void MentosIncrease();


private:
	//アイテム同士の衝突処理
	void CollisionItemVsItemes();

private:


	std::vector<Item*> itemes;
	std::set<Item*>removes;


	bool MentosFlag = false;

	int MentosNum = 0;

	float time = 0.0f;


};

