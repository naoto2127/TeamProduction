#include "FrontProjectile.h"
#include <Collision.h>
#include "StageManager.h"


//コンストラクタ
FrontProjectile::FrontProjectile(ProjectileManager* manager) :Projectile(manager)
{
 
    scale.x = scale.y = scale.z = 3.0f;
}

//デストラクタ
FrontProjectile::~FrontProjectile()
{
    //delete model;
}

//更新処理
void FrontProjectile::Update(float elapsedTime)
{
    //寿命処理
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        //自分を削除
        Destroy();
    }

    //移動
    float speed = this->speed * elapsedTime;
#if 1
    ////位置+=前方向*速さ
    //position.x += direction.x * speed;
    //position.y += direction.y * speed;
    //position.z += direction.z * speed;
#endif

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    //model->UpdateTransform(transform);
}

//描画処理
void FrontProjectile::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //shader->Draw(dc, model);
}

//発射
void FrontProjectile::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}

