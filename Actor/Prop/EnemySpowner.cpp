#include "EnemySpowner.h"
#include "Actor/Player.h"

#include "World/IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"

#include "Collision/AttackCollider.h"
#include "World/World.h"

#include "Actor/Explosion.h"

#include "Actor/Item/DroppedCoin.h"

#include "Actor/Enemy/Enemy2.h"
#include "Actor/Enemy/Enemy3.h"
#include "Actor/Enemy/Enemy4.h"
#include "Actor/Enemy/Enemy5.h"
#include "Actor/Enemy/EnemyG.h"



// 攻撃判定の距離
const float AttackDistance{ 1.6f };
// 移動判定の距離
const float WalkDistance{ 10000.0f };
// 移動スピード
const float WalkSpeed{ 0.025f };

/// 追加！
// 自分の高さ
const float EnemyHeight{ 1.0f };
// 衝突判定用の半径
const float EnemyRadius{ 0.4f };
// 足元のオフセット
const float FootOffset{ 0.1f };
// 重力
const float Gravity{ -0.0f };
///




// コンストラクタ
EnemySpowner::EnemySpowner(IWorld* world, const GSvector3& position) :
    // mesh_{ 1, MotionIdle, true },変更！↓
    mesh_{ Mesh_Enemy, 0, true },

    motion_{ 0 },
    motion_loop_{ true },
    state_timer_{ 0.0f } {
    // ワールドの設定
    world_ = world;
    // タグ名の設定
    tag_ = "EnemySpownerTag";
    // 名前の設定
    name_ = "EnemySpowner";
    // 衝突判定球の設定
    collider_ = BoundingSphere{ 0.3f, GSvector3{0.0f, 0.3f, 0.0f} };


    // 座標の初期化
    transform_.position(position);
    // ワールド変換行列の初期化
    mesh_.transform(transform_.localToWorldMatrix());

}



// 更新
void EnemySpowner::update(float delta_time) {

    // 重力を更新
    velocity_.y += Gravity * delta_time;
    // 重力を加える
    transform_.translate(0.0f, velocity_.y, 0.0f);
    // フィールドとの衝突判定
   // collide_field();

    // モーションを変更
    mesh_.change_motion(motion_, motion_loop_);
    // メッシュを更新
    mesh_.update(delta_time);
    // 行列を設定
    mesh_.transform(transform_.localToWorldMatrix());

    if (world_->isgoal().get() == 2) {
        die();
    }

    if (spownused_ == false) {
        spownused_ = true;
        GSvector3 spownpos = transform_.position();
        spownpos.x += gsRandf(-1.3f, 1.3f);
        spownpos.y += gsRandf(-1.3f, 1.3f);
        int randmax = world_->roomcount().get();
        if (randmax > 20) randmax = 20;
        int enemytype = gsRand(0, randmax);
        if (enemytype >= 0 && enemytype <= 3) {
            world_->add_actor(new Enemy2(world_, spownpos));
        }
        if (enemytype >= 4 && enemytype <= 6) {
            int type = gsRand(1, 10);
            if (type <= 5)
                world_->add_actor(new Enemy2(world_, spownpos));
            if (type >= 6)
            world_->add_actor(new Enemy3(world_, spownpos));
        }
        if (enemytype >= 7 && enemytype <= 9 ) {
            int type = gsRand(1, 10);
            if (type <= 2)
                world_->add_actor(new Enemy2(world_, spownpos));
            if (type >= 3 && type <= 7)
                world_->add_actor(new Enemy3(world_, spownpos));
            if (type >= 8)
                world_->add_actor(new Enemy4(world_, spownpos));
        }
        if (enemytype >= 10 ) {
            int type = gsRand(1, 10);
            if (type <= 1)
                world_->add_actor(new Enemy2(world_, spownpos));
            if (type >= 2 && type <= 4)
                world_->add_actor(new Enemy3(world_, spownpos));
            if (type >= 5 && type <= 7)
                world_->add_actor(new Enemy4(world_, spownpos));
            if (type >= 8 && type <= 10)
                world_->add_actor(new Enemy5(world_, spownpos));
        }
        int gspown = gsRand(0, 50);
        if (gspown == 30)   world_->add_actor(new EnemyG(world_, spownpos));
        die();
    }
}

// 描画
void EnemySpowner::draw() const {



    // メッシュの描画
    mesh_.draw();

    // 衝突判定のデバッグ表示
    ///collider().draw();

    GScolor4 meshcolor = { 1.0f,1.0f,1.0f,1.0f };
    glColor4fv(meshcolor);
}

// 衝突処理
void EnemySpowner::react(Actor& other) {








    // プレーヤーに衝突したら
    if (other.tag() == "PlayerTag" && tag_ == "GoalTag") {
        tag_ = "NullTag";
        //world_->roomcount().add(1);
        // やられた効果音を再生
        gsPlaySE(Se_EnemyDamage);
    }

}



/// ここから↓はフィールド系のクラスの追加！

// フィールドとの衝突判定
void EnemySpowner::collide_field() {
    // 壁との衝突判定（球体との判定）
    GSvector3 center; // 衝突後の球体の中心座標
    if (world_->field()->collide(collider(), &center)) {
        // y座標は変更しない
        center.y = transform_.position().y;
        // 補正後の座標に変更する
        transform_.position(center);
    }
    // 地面との衝突判定（線分との交差判定）
    GSvector3 position = transform_.position();
    Line line;
    line.start = position + collider_.center;
    line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
    GSvector3 intersect;  // 地面との交点
    if (world_->field()->collide(line, &intersect)) {
        // 交差した点からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }
}

// アクターとの衝突処理
void EnemySpowner::collide_actor(Actor& other) {
    // ｙ座標を除く座標を求める
    GSvector3 position = transform_.position();
    position.y = 0.0f;
    GSvector3 target = other.transform().position();
    target.y = 0.0f;
    // 相手との距離
    float distance = GSvector3::distance(position, target);
    // 衝突判定球の半径同士を加えた長さを求める
    float length = collider_.radius + other.collider().radius;
    // 衝突判定球の重なっている長さを求める
    float overlap = length - distance;
    // 重なっている部分の半分の距離だけ離れる移動量を求める
    GSvector3 v = (position - target).getNormalized() * overlap * 0.1f;
    transform_.translate(v, GStransform::Space::World);
    // フィールドとの衝突判定
    collide_field();
}
