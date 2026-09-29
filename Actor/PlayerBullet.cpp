#include "PlayerBullet.h"
#include "World/IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"
///#include <GSeffect.h>

// 寿命
//const float LifeSpanTime{ 50.0f };
const float LifeSpanTime{ 20.0f };

// コンストラクタ
PlayerBullet::PlayerBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity) {
    // ワールドを設定
    world_ = world;
    // タグ名
    tag_ = "PlayerBulletTag";
    // アクター名
    name_ = "PlayerBullet";
    // 移動量の初期化
    velocity_ = GSvector3{ 0.6f, 0.0f, 0.0f };
    // 衝突判定
    collider_ = BoundingSphere{ 0.7f };
    // 座標の初期化
    transform_.position(position);
    // 向きを設定する
    transform_.rotation(GSquaternion::lookRotation(velocity));
    // 寿命
    lifespan_timer_ = 0.0f;
    // エフェクトを再生（生成）する
   /// effect_handle_ = gsPlayEffect(Effect_DanceFloor, &position);

}

// 更新
void PlayerBullet::update(float delta_time) {
    // 寿命が尽きたら死亡
    if (lifespan_timer_ >= LifeSpanTime) {
        // アクターの死亡処理
        die();
        // エフェクトの停止（削除）
       /// gsStopEffect(effect_handle_);
        return;
    }



    collider().draw();


    // 寿命の更新
    lifespan_timer_ += delta_time;
    // フィールドとの衝突判定
    Line line;
    line.start = transform_.position();
    line.end = line.start + (velocity_ * delta_time);
    GSvector3 intersect;
    if (world_->field()->collide(line, &intersect)) {
        // 交点の座標に補正
        transform_.position(intersect);
        // 衝突したら移動しないようにする
        velocity_ = GSvector3{ 0.0f, 0.0f, 0.0f };
    }
    // 移動する（ワールド座標系基準）
   /// transform_.translate(velocity_ * delta_time, GStransform::Space::World);
 
    /*
    // エフェクトに自身のワールド変換行列を設定
    GSmatrix4 world = transform_.localToWorldMatrix();
    gsSetEffectMatrix(effect_handle_, &world); // ワールド変換行列を設定
    // 寿命によって、だんだん透明にしていく（アルファ値を変化させる）
    GScolor start_color{ 1.0f, 1.0f, 1.0f, 1.0f };
    GScolor end_color{ 1.0f, 1.0f, 1.0f, 0.0f };
    GScolor color = GScolor::lerp(start_color, end_color, lifespan_timer_ / LifeSpanTime);
    gsSetEffectColor(effect_handle_, &color); // カラーを設定
    */
    }

// 衝突リアクション
void PlayerBullet::react(Actor& other) {
}

