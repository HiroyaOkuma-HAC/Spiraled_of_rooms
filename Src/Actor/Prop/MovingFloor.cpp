#include "MovingFloor.h"
#include "World/IWorld.h"


// コンストラクタ
MovingFloor::MovingFloor(IWorld* world, const GSvector3& position, GSuint mesh, GSuint collder, int side, int id) {
    transform_.position(position);
    mesh_ = mesh;
    mesh_collider_ = collder;
   
    side_ = side;
    id_ = id;
    if (side_ == 0) { velocity_.z = -0.02f; }
    if (side_ == 1) { velocity_.z = 0.02f; }

    transform_.rotate(0.0f, 180.0f, 0.0f);

    world_ = world;
}

// 更新
void MovingFloor::update(float delta_time) {
    // 移動する
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);
    // 回転する
    //transform_.rotate(0.0f, 0.5f * delta_time, 0.0f);
    // y座標が10以上になったら降下
    if (doorcount_ >= 180.0f &&  open_== true) {
        if (side_ == 0) { velocity_.z = 0.02f; }
        if (side_ == 1) { velocity_.z = -0.02f; }
        doorcount_ = 0;
        open_ = false;
    }
    // y座標が0以下になったら上昇
    else if (doorcount_ >= 180.0f && open_ == false) {
        if (side_ == 0) { velocity_.z = -0.02f; }
        if (side_ == 1) { velocity_.z = 0.02f; }
        doorcount_ = 0;
        open_ = true;
    }
    doorcount_ += delta_time;
 
}