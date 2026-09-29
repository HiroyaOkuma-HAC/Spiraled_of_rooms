#include "EnemyGenerator.h"
#include "World/IWorld.h"
#include "Field.h"
#include "Enemy.h"
#include "GOLDKARATE.h"

#include "Enemy2.h"
#include "Enemy3.h"

// コンストラクタ
EnemyGenerator::EnemyGenerator(IWorld* world) {
	world_ = world;
	tag_ = "Generator";
	name_ = "EnemyGenerator";
	timer_ = 0.0f;
	enable_collider_ = false;
}

// 更新
void EnemyGenerator::update(float delta_time) {

    if (timer_ <= 0) {
        // 出現座標をランダムに決定する
        int XZRand = gsRand(0, 1);
        if (XZRand == 0) {
            int ZRand = gsRand(0, 1);
            float Z = 0.0f;
            if (ZRand == 0) Z = -20.0f;
            else if (ZRand == 1) Z = 20.0f;
            GSvector3 position{ gsRandf(-20.0f,20.0f),0.0f,Z};
            world_->add_actor(new Enemy{ world_, position });
            world_->add_actor(new (Enemy3)(world_, position));
        }
        else if (XZRand == 1) {
            int XRand = gsRand(0, 1);
            float X = 0.0f;
            if (XRand == 0) X = -20.0f;
            else if (XRand == 1) X = 20.0f;
            GSvector3 position{ X,0.0f,gsRandf(-20.0f,20.0f) };
            world_->add_actor(new Enemy{ world_, position });
            world_->add_actor(new (Enemy3)(world_, position));
        }

        timer_ = 30.0f;
    }
    int enemy_count_ = world_->count_actor_with_tag("EnemyTag");
    if (enemy_count_ < 40) {
        timer_ -= delta_time;
    }
    
}

// 描画
void EnemyGenerator::draw() const {
    int enemy_count_ = world_->count_actor_with_tag("EnemyTag");

    // デバッグ表示
  //  gsDrawText("てきちゃんのかず = %d", enemy_count_);
}