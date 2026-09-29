#include "Actor/Item/DroppedHeal.h"
#include "World/IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"
#include <GSeffect.h>
#include "Camera/Mycamera.h"
#include "Math/Coin.h"
#include "Math/PlayerHitpoint.h"
#include "Math/Score.h"

// 寿命
const float LifeSpanTime{ 1800.0f };

// すいよせ判定の距離
const float MoveDistance{ 7.6f };
// 振り向く角度
const float TurnAngle{ 2.5f };

// コンストラクタ
DroppedHeal::DroppedHeal(IWorld* world, const GSvector3& position, int how) :
    mesh_{ Mesh_Portion, 0, true } {
    // ワールドを設定
    world_ = world;
    // タグ名
    tag_ = "DroppedHealTag";
    // アクター名
    name_ = "DroppedHeal";

    how_ = how;



    // プレイヤーを検索
    Actor* player = world_->find_actor("Player");
    if (player == nullptr) return;


    // 衝突判定
    collider_ = BoundingSphere{ 0.2f };
    // 座標の初期化
    transform_.position(position);
    // 寿命
    lifespan_timer_ = 0.0f;
    // エフェクトを再生（生成）する
    //effect_handle_ = gsPlayEffect(Effect_MissileBoost, &position);

    velocity_.y = 0.2f;
    velocity_.x = gsRandf(-0.05, 0.05);
    velocity_.z = gsRandf(-0.05, 0.05);
    // ワールド変換行列の初期化
    mesh_.transform(transform_.localToWorldMatrix());

}

// 更新
void DroppedHeal::update(float delta_time) {

    // プレーヤーを検索する
    player_ = world_->find_actor("Player");

    if (is_move() && alreadycollide == true) {
        // ターゲット方向の角度を求める
        float angle = target_signed_angle();
        // 向きを変える
        transform_.rotate(0.0f, angle, 0.0f);

        // 前進する（ローカル座標基準）
        transform_.translate(0.0f, 0.0f, 0.2f * delta_time);
        if (transform_.position().y < player_->transform().position().y + 0.2f) transform_.translate(0.0f, 0.3f, 0.0f);
        if (transform_.position().y > player_->transform().position().y) transform_.translate(0.0f, -0.3f, 0.0f);
    }

    else {
        transform_.rotate(0.0f, delta_time * 10, 0.0f);
    }
    // メッシュを更新
    mesh_.update(delta_time);
    // 行列を設定
    mesh_.transform(transform_.localToWorldMatrix());

    if (alreadycollide == false) {
        velocity_.y -= 0.01f;
    }

    // 寿命が尽きたら死亡
    if (lifespan_timer_ >= LifeSpanTime) {
        // アクターの死亡処理
        die();
        return;
    }
    // 寿命の更新
    lifespan_timer_ += delta_time;
    // フィールドとの衝突判定
    Line line;
    line.start = transform_.position();
    line.end = line.start + (velocity_ * delta_time);
    GSvector3 intersect;
    if (world_->field()->collide(line, &intersect) && alreadycollide == false) {
        // 交点の座標に補正
        transform_.position(intersect);
        velocity_ = GSvector3{ 0.0f, 0.0f, 0.0f };
        alreadycollide = true;
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

// 描画
void DroppedHeal::draw() const {
    // メッシュの描画
    mesh_.draw();

   // collider().draw();
}

// 衝突リアクション
void DroppedHeal::react(Actor& other) {
    if (other.tag() == "PlayerTag") {
        world_->score().add(how_ * 10);
        world_->player_hp().add(how_ / 3);
        gsPlaySE(Se_heal);
        die();
        return;
    }




    // 寿命の更新
    lifespan_timer_ += 1.0f;

    /*
    // 寿命が尽きたら死亡
    if (velocity_ == GSvector3{ 0.0f, 0.0f, 0.0f }) {
        // アクターの死亡処理
        die();
        // エフェクトの停止（削除）
        gsStopEffect(effect_handle_);
        return;
    }
    */

    // effect_handle_ = gsPlayEffect(Effect_MainShotHit, &position);
     //world_->add_actor(new (MainHitEffect){ world_, transform_.position() });

    // 無敵状態にする
    //enable_collider_ = false;

}

// ターゲットとの距離を求める
float DroppedHeal::target_distance() const {

    // ターゲットがいなければ最大距離を返す
    if (player_ == nullptr) return FLT_MAX; // float型の最大値

    // ターゲットとの距離を計算する
    return GSvector3::distance(player_->transform().position(), transform_.position());
}

bool DroppedHeal::is_move() const {
    // すいよせ距離内か？
    return (target_distance() <= MoveDistance);
}

// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号付き）
float DroppedHeal::target_signed_angle() const {

    // ターゲットがいなければ0を返す
    if (player_ == nullptr) return 0.0f;

    // ターゲット方向のベクトルを求める
    GSvector3 to_target = player_->transform().position() - transform_.position();
    // 前向き方向のベクトルを取得
    GSvector3 forward = transform_.forward();
    // ベクトルのy成分を無効にする
    forward.y = 0.0f;
    to_target.y = 0.0f;
    // 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める
    return GSvector3::signedAngle(forward, to_target);
}

// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号なし）
float DroppedHeal::target_angle() const {
    return std::abs(target_signed_angle());
}

// フィールドとの衝突判定
void DroppedHeal::collide_field() {
    // 壁との衝突判定（球体との判定）
    GSvector3 center; // 衝突後の球体の中心座標
    if (world_->field()->collide(collider(), &center)) {
        // y座標は変更しない
        center.y = transform_.position().y;
        // 補正後の座標に変更する
        transform_.position(center);
    }
    // 地面との衝突判定（線分との交差判定）
    GSvector3 position = transform_.position();
    Line line;
    line.start = position + collider_.center;
    line.end = position + GSvector3{ 0.0f, -0.1, 0.0f };
    GSvector3 intersect;  // 地面との交点
    if (world_->field()->collide(line, &intersect)) {
        // 交差した点からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }
}
