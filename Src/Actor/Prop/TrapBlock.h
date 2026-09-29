#ifndef TRAP_BLOCK_H_
#define TRAP_BLOCK_H_

#include "FieldActor.h"
#include "World/World.h"

// 動く床クラス
class TrapBlock : public FieldActor {
public:
    // コンストラクタ
    TrapBlock(IWorld* world, const GSvector3& position, GSuint mesh, GSuint collider);
    // 更新
    virtual void update(float delta_time) override;
    // フィールドとの衝突処理
    void collide_field();
    // 衝突リアクション
    virtual void react(Actor& other) override;
private:
    IWorld* world_;

    float timer_ = { 0.0f };
    bool move_y = { true };
    bool enable = { false };
};

#endif // BLOCK_H_