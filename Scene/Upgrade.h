#ifndef UPGRADE_H_
#define UPGRADE_H_

#include <vector>
#include <string>

// リザルトクラス
class Upgrade {
public:
	// コンストラクタ
	Upgrade();
	// 初期化
	void initialize();
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;

private:
	// 背景の描画
	void draw_background() const;

};

#endif 
