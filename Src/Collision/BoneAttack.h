#ifndef BONE_ATTACK_H_
#define BONE_ATTACK_H_

#include "Actor/Actor.h"
#include "AnimatedMesh.h"


// プレーヤーの弾クラス
class BoneAttack : public Actor {
public:
    // コンストラクタ
    BoneAttack(IWorld* world, const GSvector3& position, const GSvector3& velocity);
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

    GSvector3 vec = { 0.0f,0.0f,0.0f };

    // アニメーションメッシュ
    AnimatedMesh	mesh_;
};

#endif