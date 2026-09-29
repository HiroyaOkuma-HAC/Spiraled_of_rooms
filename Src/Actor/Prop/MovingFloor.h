#ifndef MOVING_FLOOR_H_
#define MOVING_FLOOR_H_

#include "FieldActor.h"

// 動く床クラス
class MovingFloor : public FieldActor {
public:
    // コンストラクタ
    MovingFloor(IWorld* world, const GSvector3& position, GSuint mesh, GSuint collider,int side, int id);
    // 更新
    virtual void update(float delta_time) override;

    enum {
        LEFT = 0,
        RIGHT,
    };



private:
    // ドアの位置
    int side_ = 0;
    // ドア番号
    int id_ = 0;
    // タイマー
    float doorcount_ = 0.0f;

    bool open_ = true;


};

#endif