#ifndef EXPLOSION_PART_FIRE_H_
#define EXPLOSION_PART_FIRE_H_

#include "Actor/Actor.h"

// 爆発火花クラス
class ExplosionPartFire : public Actor {
public:
	// コンストラクタ
	ExplosionPartFire(IWorld* world, const GSvector3& position, const GSvector3& velocity);
	// 更新
	virtual void update(float delta_time) override;
	// 半透明オブジェクトの描画
	virtual void draw_transparent() const override;
private:
	// 消滅タイマー
	float timer_{ 0.0f };
};

#endif