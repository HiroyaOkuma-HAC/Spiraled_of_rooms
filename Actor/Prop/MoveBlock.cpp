#include "Actor/Prop/MoveBlock.h"
#include "World/IWorld.h"


// コンストラクタ
MoveBlock::MoveBlock(IWorld* world, const GSvector3& position, GSuint mesh, GSuint collder) {
    transform_.position(position);
    mesh_ = mesh;
    mesh_collider_ = collder;


    //transform_.rotate(0.0f, 180.0f, 0.0f);

     // ワールドの設定
    world_ = world;

    GSvector3 move_ = { 7.5f,0.0f,0.0f };

}


// 更新
void MoveBlock::update(float delta_time) {
    timer_ += 1.0f * delta_time;
    GSvector3 move_ = { 7.5f,0.0f,0.0f };

    if (timer_ >= 240.0f && move_LR == false) {
        move_to(transform_.position() + move_, 180.0f).ease(EaseType::EaseOutQuart).overshoot(3.0f).name("test_move");
        move_LR = true;
        timer_ = 0.0f;
    }

    if (timer_ >= 240.0f && move_LR == true){
        move_to(transform_.position() - move_, 180.0f).ease(EaseType::EaseOutQuart).overshoot(3.0f).name("test_move");;
    move_LR = false;
    timer_ = 0.0f;

    }


    if (world_->isgoal().get() == 4) {
        Tween::cancel("test_move");

        die();
    }

}
