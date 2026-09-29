#ifndef CAMERA_FPS_H_
#define CAMERA_FPS_H_

#include "Actor/Actor.h"

// 1人称カメラクラス
class CameraFPS : public Actor {
public:
    // コンストラクタ
    CameraFPS(IWorld* world);
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

#endif