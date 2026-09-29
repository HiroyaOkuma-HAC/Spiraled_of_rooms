#ifndef MOVE_BLOCK_H_
#define MOVE_BLOCK_H_

#include "FieldActor.h"
#include "World/World.h"

// 動く床クラス
class MoveBlock : public FieldActor {
public:
    // コンストラクタ
    MoveBlock(IWorld* world, const GSvector3& position, GSuint mesh, GSuint collider);
    // 更新
    virtual void update(float delta_time) override;
private:
    IWorld* world_;

    float timer_ = { 0.0f };
    bool move_LR = { false };
};

#endif // BLOCK_H_