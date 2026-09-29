#ifndef ENEMY_GENERATOR_H_
#define ENEMY_GENERATOR_H_

#include "Actor/Actor.h"

// 敵生成クラス
class EnemyGenerator : public Actor {
public:
	// コンストラクタ
	EnemyGenerator(IWorld* world);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;

private:
	// 生成時間タイマー
	float timer_{ 0.0f };

	// 生成開始タイマー
	float Newtimer_{ 0.0f };
};

#endif