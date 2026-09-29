#include "Actor/Prop/TrapBlock.h"
#include "World/IWorld.h"
#include <Assets.h>
#include "Line.h"
#include "Field.h"

#include "Collision/TrapCollision.h"

enum 
{
    Default,
    Ready,
    Stage1,
    Stage2,
};

// 足元のオフセット
const float FootOffset{ 1.5f };

// コンストラクタ
TrapBlock::TrapBlock(IWorld* world, const GSvector3& position, GSuint mesh, GSuint collder) {
    transform_.position(position);
    mesh_ = mesh;
    mesh_collider_ = collder;


    //transform_.rotate(0.0f, 180.0f, 0.0f);

     // ワールドの設定
    world_ = world;

    tag_ = "TrapTag_anenable";

   // collider_ = BoundingSphere{ 0.5f, GSvector3{0.0f, 0.3f, 0.0f} };


}


// 更新
void TrapBlock::update(float delta_time) {
    if (move_y == false) 
    {
        float result = fmod(transform_.position().y, 3.0f);
        if (result >= 1.0f) 
        {
            die();
        }
    }
    GSvector3 damagepos = transform_.position();
    damagepos.y += 1.8f;
    GSvector3 velocity = { 0.0f,0.0f,0.0f };
    collider().draw();
    timer_ += 2.0f * delta_time;
    collide_field();
    if (move_y == true) {
        transform_.position(GSvector3{transform_.position().x,transform_.position().y -0.1f,transform_.position().z});
    }

    if (timer_ >= 0.0f && timer_ <= 240.0f) {
        mesh_ = { Mesh_Trap_Default };
        tag_ = "TrapTag_anenable";
    }
    else  if (timer_ > 240.0f && timer_ <= 360.0f) {
        mesh_ = { Mesh_Trap_Ready };
        tag_ = "TrapTag_anenable";
    }
    else  if (timer_ > 360.0f && timer_ <= 450.0f) {
        mesh_ = { Mesh_Trap_Default };
        tag_ = "TrapTag_anenable";
    }
    else  if (timer_ > 450.0f && timer_ <= 455.0f) {
        mesh_ = { Mesh_Trap_Ready };
        tag_ = "TrapTag_anenable";
    }
    else  if (timer_ > 455.0f && timer_ <= 460.0f) {
        mesh_ = { Mesh_Trap_Stage1 };
        tag_ = "TrapTag_anenable";
    }
    else  if (timer_ > 460.0f && timer_ <= 650.0f) {
        mesh_ = { Mesh_Trap_Stage2 };
        tag_ = "TrapTag_enable";
        if (enable == false) {
            world_->add_actor(new TrapCollision{ world_, damagepos, velocity });
            damagepos.x -= 4.6f/2;
            damagepos.z -= 4.6f/2;
            world_->add_actor(new TrapCollision{ world_, damagepos, velocity });
            damagepos.x += 9.2f/2;
            world_->add_actor(new TrapCollision{ world_, damagepos, velocity });
            damagepos.z += 9.2f/2;
            world_->add_actor(new TrapCollision{ world_, damagepos, velocity });
            damagepos.x -= 9.2f/2;
            world_->add_actor(new TrapCollision{ world_, damagepos, velocity });
            enable = true;
        }
    }
    else  if (timer_ > 650.0f && timer_ <= 655.0f) {
        mesh_ = { Mesh_Trap_Stage1 };
        tag_ = "TrapTag_anenable";
        enable = false;
    }
    else  if (timer_ > 655.0f && timer_ <= 660.0f) {
        mesh_ = { Mesh_Trap_Ready };
        tag_ = "TrapTag_anenable";
    }
    else  if (timer_ > 660.0f ) {
        mesh_ = { Mesh_Trap_Default };
        tag_ = "TrapTag_anenable";
        timer_ = 0.0f;
    }




    if (world_->isgoal().get() == 4) {

        die();
    }

}

void TrapBlock::collide_field() {
    // 壁との衝突判定（球体との判定）
    GSvector3 center; // 衝突後の球体の中心座標
    if (world_->field()->collide(collider(), &center)) {
        // y座標は変更しない
        center.y = transform_.position().y;
        // 補正後の座標に変更する
        transform_.position(center);
        if (move_y == true) move_y = false;
    }
    // 地面との衝突判定（線分との交差判定）
    GSvector3 position = transform_.position();
    Line line;
    line.start = position + collider_.center;
    line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
    GSvector3 intersect;            // 地面との交点
    // 衝突したフィールド用アクター
    Actor* field_actor{ nullptr };
    // 親をリセットしておく
    transform_.parent(nullptr);
    if (world_->field()->collide(line, &intersect, nullptr, &field_actor)) {
        // 交差した点からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
        // フィールド用のアクタークラスと衝突したか？
        if (field_actor != nullptr) {
            // 衝突したフィールド用のアクターを親のトランスフォームクラスとして設定
            transform_.parent(&field_actor->transform());
            if (move_y == true)
            transform_.position(GSvector3{ transform_.position().x,transform_.position().y - 1.5f,transform_.position().z });
        }
        if (move_y == true) move_y = false;
    }
}

void TrapBlock::react(Actor& other) {
    if (other.tag() == "GoalTag") {
        die();
    }
    }