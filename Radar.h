#ifndef RADAR_H_
#define RADAR_H_

#include "Actor/Actor.h"

// レーダークラス
class Radar : public Actor {
public:
	// コンストラクタ
	Radar(IWorld* world);
	// 描画
	void draw_gui() const override;

private:
	// レーダーの点を描画
	void draw_point(const GSvector3& position, const GScolor& color) const;
};

#endif // RADAR_H_
