#pragma once

#include<vector>
#include<set>
#include"people.h"

//アイテムマネージャー
class PeopleManager
{
private:
	PeopleManager() {}
	~PeopleManager() {}
public:
	//唯一のインスタンス取得
	static PeopleManager& Instance()
	{
		static PeopleManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//アイテム登録
	void Register(People* people);

	//アイテム完全削除
	void Clear();

	//デバッグプリミティブ描画
	void DrawDebugPrimitive();

	//アイテム数取得
	int GetPeopleCount() const { return static_cast<int>(humanes.size()); }

	//アイテム取得
	People* GetPeople(int index) { return  humanes.at(index); }

	//アイテム削除
	void Remove(People* people);


private:
	//アイテム同士の衝突処理
	//void CollisionItemVsItemes();

private:


	std::vector<People*> humanes;
	std::set<People*>removes;


};

