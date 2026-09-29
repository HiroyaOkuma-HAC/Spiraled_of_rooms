#ifndef RAY_H_
#define RAY_H_

#include <gslib.h>

// レイクラス
class Ray {
public:
	// デフォルトコンストラクタ
	Ray() = default;
	// コンストラクタ
	Ray(const GSvector3& position, const GSvector3& direction) :
		position{ position }, direction{ direction } {}

public:
	// 始点
	GSvector3 position{ 0.0f,0.0f,0.0f };
	// 終点
	GSvector3 direction{ 0.0f,0.0f,0.0f };
};

#endif