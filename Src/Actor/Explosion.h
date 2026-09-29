#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "Actor/Actor.h"


// 爆発生成クラスの作成
class Explosion : public Actor {
public:
    // コンストラクタ
    Explosion(IWorld* world, const GSvector3& position);
    // 更新
    virtual void update(float delta_time) override;
};

#endif