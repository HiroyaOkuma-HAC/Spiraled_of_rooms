#include "Collision/RemootAttackCollider.h"
#include "World/IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"
#include <GSeffect.h>
#include "Camera/Mycamera.h"

#include "Actor/Player.h"
#include "Actor/Enemy/Enemy5.h"

#include <cmath>

#include <cmath>



// 寿命
const float LifeSpanTime{ 180.0f };

// プレーヤーからの相対位置（z座標のみ）
const GSvector3 PlayerOffset{ 0.0f, 0.0f, -5.0f };
// カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f, 1.8f, 0.0f };


// コンストラクタ
RemootAttackCollider::RemootAttackCollider(IWorld* world, const GSvector3& position, const GSvector3& velocity) {
    // ワールドを設定
    world_ = world;
    // タグ名
    tag_ = "RemootAttackTag";
    // アクター名
    name_ = "RemootAttack";
    // 移動量の初期化
    //velocity_ = velocity;

    /// 視点によって上、下にも撃てるようにする
    // プレイヤーを検索
   // Actor* player = world_->find_actor("Player");
    //if (player == nullptr) return;
    // プレーヤーを検索する
    player_ = world_->find_actor("Player");
    // 視点の位置
    //GSvector3 eye = PlayerOffset * player->transform().localToWorldMatrix();
    // 中視点の位置
  //  GSvector3 at = eye + player->transform().forward();

   // velocity_.y = world_->camera()->transform().forward().y * 1.4f; /// 1.4f 1.1f
    ///
        // 座標の初期化
    transform_.position(position);
    // 向きを設定する
    transform_.rotation(GSquaternion::lookRotation(-velocity));

    GSvector3 playerPos = player_->transform().position();
    playerPos.y += 1.0f;
    GSvector3 enemyPos = position;

    GSvector3 diff = playerPos - enemyPos;
    GSvector3 shootDir; // 結果を受け取る用の変数

    gsVector3Normalize(&shootDir, &diff); // ← 引数は2つ！ & を忘れずに！

    float bulletSpeed = 0.2f;
     velocity_ = shootDir * bulletSpeed; // bulletSpeed は float で設定！

    collider_ = BoundingSphere{ 0.4f };

    // 寿命
    lifespan_timer_ = 0.0f;
    // エフェクトを再生（生成）する
    effect_handle_ = gsPlayEffect(Effect_MissileBoost, &position);

    // 注視点の方向を見る
  //  transform_.lookAt(at);
}

// 更新
void RemootAttackCollider::update(float delta_time) {

    lifespan_timer_ += delta_time;

    // 寿命が尽きたら死亡
    if (lifespan_timer_ >= LifeSpanTime) {
        // アクターの死亡処理

        // エフェクトの停止（削除）
        gsStopEffect(effect_handle_);
        die();
        return;
    }
    // 寿命の更新
   // lifespan_timer_ += delta_time;
    // フィールドとの衝突判定
    Line line;
    line.start = transform_.position();
    line.end = line.start + (velocity_ * delta_time);
    GSvector3 intersect;
    if (world_->field()->collide(line, &intersect)) {
        // 交点の座標に補正
        transform_.position(intersect);
        // 衝突したら移動しないようにする
       // velocity_ = GSvector3{ 0.0f, 0.0f, 0.0f };
        // エフェクトの停止（削除）
        gsStopEffect(effect_handle_);
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
    GScolor end_color{ 1.0f, 1.0f, 1.0f, 0.0f };
    GScolor color = GScolor::lerp(start_color, end_color, lifespan_timer_ / LifeSpanTime);
    gsSetEffectColor(effect_handle_, &color); // カラーを設定
}

// 衝突リアクション
void RemootAttackCollider::react(Actor& other) {

    if (other.tag() == "PlayerTag") {}
    else return;

    // 衝突したら移動しないようにする
   // velocity_ = GSvector3{ 0.0f, 0.0f, 0.0f };
    GScolor color{ 0.0f, 0.0f, 0.0f, 0.0f };
    gsSetEffectColor(effect_handle_, &color); // カラーを設定


    color = { 1.0f, 1.0f, 1.0f, 1.0f };
    gsSetEffectColor(effect_handle_, &color); // カラーを設定


    // 寿命の更新
    lifespan_timer_ += 1.0f;

    // 寿命が尽きたら死亡
    if (velocity_ == GSvector3{ 0.0f, 0.0f, 0.0f }) {
        // エフェクトの停止（削除）
        gsStopEffect(effect_handle_);
        // アクターの死亡処理
        die();

        return;
    }


    //world_->add_actor(new (MainHitEffect){ world_, transform_.position() });

   // 無敵状態にする
   //enable_collider_ = false;

}

void RemootAttackCollider::draw() const {
      collider().draw();
}

