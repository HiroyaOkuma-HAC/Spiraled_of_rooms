#ifndef PLAYER_BULLET_GRAPLE_H_
#define PLAYER_BULLET_GRAPLE_H_

#include "Actor/Actor.h"

// プレーヤーの弾クラス
class PlayerBulletGraple : public Actor {
public:
    // コンストラクタ
    PlayerBulletGraple(IWorld* world, const GSvector3& position, const GSvector3& velocity);
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

};

#endif