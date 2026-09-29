#include "Actor/Prop/Block.h"
#include "World/IWorld.h"


// コンストラクタ
Block::Block(IWorld* world, const GSvector3& position, GSuint mesh, GSuint collder) {
    transform_.position(position);
    mesh_ = mesh;
    mesh_collider_ = collder;


    //transform_.rotate(0.0f, 180.0f, 0.0f);

     // ワールドの設定
    world_ = world;
}


// 更新
void Block::update(float delta_time) {
    if (world_->isgoal().get() == 4) {
        die();
    }

}
