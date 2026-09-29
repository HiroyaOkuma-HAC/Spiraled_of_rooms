#ifndef RESULT_H_
#define RESULT_H_

#include <vector>
#include <string>

// リザルトクラス
class Result {
public:
	// コンストラクタ
	Result();
	// 初期化
	void initialize();
	// スコアの追加
	void add_score(int value);
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;
	// ランキングファイルの保存
	void save(const std::string& file_name) const;
	// ランキングファイルの読み込み
	void load(const std::string& file_name);

private:
	// 背景の描画
	void draw_background() const;
	// プレイヤーの描画
	void draw_player() const;
	// ランキングの描画
	void draw_ranking() const;
	// 評価メッセージの描画
	void draw_grade_message() const;
	// スコアから評価を計算する
	int calculate_grades() const;

private:
	// ランキングデータ
	std::vector<int> ranking_scores_;
	// 最後に追加したスコア
	int final_score_{ 0 };
	// 最後に追加したスコアのランク
	int final_rank_{ 0 };
	// プレイヤー移動用タイマー
	float moving_timer_{ 0.0f };
};

#endif // !RESULT_H_
