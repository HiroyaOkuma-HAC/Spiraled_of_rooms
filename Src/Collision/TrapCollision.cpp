#include "TrapCollision.h"
#include "World/IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"
#include <GSeffect.h>
#include "Camera/Mycamera.h"

// 寿命
const float LifeSpanTime{ 20.0f };

// プレーヤーからの相対位置（z座標のみ）
const GSvector3 PlayerOffset{ 0.0f, 0.0f, -5.0f };
// カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f, 1.8f, 0.0f };


// コンストラクタ
TrapCollision::TrapCollision(IWorld* world, const GSvector3& position, const GSvector3& velocity) {
    // ワールドを設定
    world_ = world;
    // タグ名
    tag_ = "TrapCollisionTag";
    // アクター名
    name_ = "TrapCollision";
    // 移動量の初期化
    velocity_ = velocity;


    // 衝突判定
    collider_ = BoundingSphere{ 1.2f };
    // 座標の初期化
    transform_.position(position);
    // 向きを設定する
    transform_.rotation(GSquaternion::lookRotation(-velocity));
    // 寿命
    lifespan_timer_ = 0.0f;
    // エフェクトを再生（生成）する
    //effect_handle_ = gsPlayEffect(Effect_MissileBoost, &position);

  // 注視点の方向を見る
    //transform_.lookAt(at);
   // effect_handle_ = gsPlayEffect(Effect_Clew, &position);
}

// 更新
void TrapCollision::update(float delta_time) {

    // 寿命が尽きたら死亡
    if (lifespan_timer_ >= LifeSpanTime) {
        // アクターの死亡処理
        die();
        // エフェクトの停止（削除）
       // gsStopEffect(effect_handle_);
        return;
    }
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
        die();
        GSvector3 hitpos = transform_.position();

        GScolor color{ 0.0f, 0.0f, 0.0f, 0.0f };
        gsSetEffectColor(effect_handle_, &color); // カラーを設定
    }
    // 移動する（ワールド座標系基準）
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);


    // エフェクトに自身のワールド変換行列を設定
    GSmatrix4 world = transform_.localToWorldMatrix();
    gsSetEffectMatrix(effect_handle_, &world); // ワールド変換行列を設定
    // 寿命によって、だんだん透明にしていく（アルファ値を変化させる）
    GScolor start_color{ 1.0f, 1.0f, 1.0f, 1.0f };
    GScolor end_color{ 1.0f, 1.0f, 1.0f, 1.0f };
    GScolor color = GScolor::lerp(start_color, end_color, lifespan_timer_ / LifeSpanTime);
    gsSetEffectColor(effect_handle_, &color); // カラーを設定
}

// 衝突リアクション
void TrapCollision::react(Actor& other) {
    if (other.tag() == "PlayerTag") {}
    else return;

    // 衝突したら移動しないようにする
    velocity_ = GSvector3{ 0.0f, 0.0f, 0.0f };
    GScolor color{ 0.0f, 0.0f, 0.0f, 0.0f };
    //   gsSetEffectColor(effect_handle_, &color); // カラーを設定


    color = { 1.0f, 1.0f, 1.0f, 1.0f };
    //   gsSetEffectColor(effect_handle_, &color); // カラーを設定


       // 寿命の更新
    lifespan_timer_ += 1.0f;

    // 寿命が尽きたら死亡
    if (velocity_ == GSvector3{ 0.0f, 0.0f, 0.0f }) {
        // アクターの死亡処理
        die();
        // エフェクトの停止（削除）
        gsStopEffect(effect_handle_);
        return;
    }

    // effect_handle_ = gsPlayEffect(Effect_MainShotHit, &position);
     //world_->add_actor(new (MainHitEffect){ world_, transform_.position() });

    die();
    // 無敵状態にする
    //enable_collider_ = false;

}

void TrapCollision::draw() const {
   // collider().draw();
}

