#ifndef REMOOT_ATTACK_COLLIDER_H_
#define REMOOT_ATTACK_COLLIDER_H_

#include "Actor/Actor.h"

// プレーヤーの弾クラス
class RemootAttackCollider : public Actor {
public:
    // コンストラクタ
    RemootAttackCollider(IWorld* world, const GSvector3& position, const GSvector3& velocity);
    // 更新
    virtual void update(float delta_time) override;
    // 描画 (デバッグ用！)
    virtual void draw() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;
private:
    // 寿命タイマ
    float lifespan_timer_{ 0.0f };
    // エフェクトハンドル
    GSint effect_handle_;

    // プレーヤー
    Actor* player_;
    // 敵
    Actor* enemy5_;

};

#endif