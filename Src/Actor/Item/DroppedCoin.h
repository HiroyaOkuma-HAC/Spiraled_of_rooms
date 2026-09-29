#ifndef DROPPED_COIN_H_
#define DROPPED_COIN_H_

#include "Actor/Actor.h"

#include "AnimatedMesh.h"


// フィールドに落ちたコインのクラス
class DroppedCoin : public Actor {
public:
    // コンストラクタ
    DroppedCoin(IWorld* world, const GSvector3& position, int how);
    // 更新
    virtual void update(float delta_time) override;
    // 描画 
    virtual void draw() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;
private:
    // フィールドとの衝突処理
    void collide_field();

    // すいよせ判定
    bool is_move() const;
    // ターゲット方向の角度を求める（符号付き）
    float target_signed_angle() const;
    // ターゲット方向の角度を求める（符号なし）
    float target_angle() const;
    // ターゲットの距離を求める
    float target_distance() const;
    // アニメーションメッシュ
    AnimatedMesh	mesh_;
    // 寿命タイマ
    float lifespan_timer_{ 0.0f };
    // エフェクトハンドル
    GSint effect_handle_;
    // プレーヤー
    Actor* player_;

    bool alreadycollide = false;

    // 獲得量
    int how_ = 0;
};

#endif