#include "StageMain.h"

//static Stage* instance = nullptr;

//インスタンス取得
//Stage& Stage::Instance()
//{
//    return *instance;
//}

//コンストラクタ
StageMain::StageMain()
{
    //instance = this;

    //ステージモデルを読み込み
    model = new Model("Data/Model/stage/stage5.mdl");
}

StageMain::~StageMain()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void StageMain::Update(float elapsedTime)
{
    //今は特に無し
}

//描画処理
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

//レイキャスト
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}