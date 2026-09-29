#ifndef BLOCK_H_
#define BLOCK_H_

#include "FieldActor.h"
#include "World/World.h"

// 動く床クラス
class Block : public FieldActor {
public:
    // コンストラクタ
    Block(IWorld* world, const GSvector3& position, GSuint mesh, GSuint collider);
    // 更新
    virtual void update(float delta_time) override;
private:
    IWorld* world_;
};

#endif // BLOCK_H_