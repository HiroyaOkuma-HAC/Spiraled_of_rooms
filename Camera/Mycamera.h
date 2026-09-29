#ifndef MYCAMERA_H_
#define MYCAMERA_H_

#include "Actor/Actor.h"

// 三人称カメラクラス
class MyCamera : public Actor {
public:
    // コンストラクタ
    MyCamera(IWorld* world, const GSvector3& position, const GSvector3& at);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;

private:
    // y軸周りの回転角度
    float yaw_{ 0.0f };
    // x軸周りの回転角度
    float pitch_{ 0.0f };
};

#endif // MYCAMERA_H_