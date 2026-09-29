#ifndef GOAL_H_
#define GOAL_H_

#include "Actor/Actor.h"
#include "AnimatedMesh.h"

// ゴールクラス
class Goal : public Actor {
public:

    // コンストラクタ
    Goal(IWorld* world, const GSvector3& position);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;

private:

    /// 追加！
    // フィールドとの衝突処理
    void collide_field();
    // アクターとの衝突処理
    void collide_actor(Actor& other);


private:
    // アニメーションメッシュ
    AnimatedMesh	mesh_;
    // モーション番号
    GSuint		motion_;
    // モーションのループ指定
    bool             motion_loop_;
    // 状態タイマ
    float		state_timer_;
    // プレーヤー
    Actor* player_;
    // エフェクトハンドル
    GSint effect_handle_;

};

#endif

