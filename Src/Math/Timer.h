#ifndef TIMER_H_
#define TIMER_H_

// 制限時間クラス
class Timer {
public:
	// コンストラクタ
	Timer(float time = 0.0f);
	// 初期化
	void initialize(float time);
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;
	// タイムアウトか？
	bool is_timeout() const;

private:
	// タイマ（秒単位）
	float time_{ 0.0f };
};

#endif // TIMER__H_