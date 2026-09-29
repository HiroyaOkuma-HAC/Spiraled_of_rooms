#include "Result.h"
#include "Rendering/ResultTX.h"
#include "Assets.h"
#include <gslib.h>
#include <fstream>

// ランキング数
const int RankingCount{ 5 };
// プレイヤーの移動時間
const float MovingTime{ 30.0f };

// コンストラクタ
Result::Result() {
	// 初期化
	initialize();
}

// 初期化
void Result::initialize() {
	// プレイヤー移動用のタイマー初期化
	moving_timer_ = 0.0f;
	// 最終スコアの初期化
	final_score_ = 0;
	// 最終ランクの初期化
	final_rank_ = 0;
	// ランキングの初期化
	ranking_scores_.resize(RankingCount, 0);
}

// スコアの追加
void Result::add_score(int value) {
	// スコアの設定
	final_score_ = value;
	// 順位を求める
	final_rank_ = 0;
	for (int score : ranking_scores_) {
		// 追加するスコア以上の既存のスコアの数を数える
		if (final_score_ <= score) {
			++final_rank_;
		}
	}
	// ランクインしたか？
	if (final_rank_ < RankingCount) {
		// 新しいスコアの挿入
		ranking_scores_.insert(ranking_scores_.begin() + final_rank_, final_score_);
		// 最下位のスコアを削除
		ranking_scores_.pop_back();
	}
}

// 更新
void Result::update(float delta_time) {
	// 移動時間の更新
	moving_timer_ = MIN(moving_timer_ + delta_time, MovingTime);
}

// 描画
void Result::draw() const {
	// 背景の描画
	//draw_background();
	// 剣道部長の表示
	//draw_player();
	// 評価メッセージの表示
	//draw_grade_message();
	// ランキングの表示
	draw_ranking();
}

// ランキングファイルの保存
void Result::save(const std::string& file_name) const {
	// ファイルのオープン
	std::ofstream file{ file_name };
	// ファイルがオープンできなかったら何もしない
	if (!file) return;
	// スコアの保存
	for (int score : ranking_scores_) {
		file << score << std::endl;
	}
}

// ランキングファイルの読み込み
void Result::load(const std::string& file_name) {
	// ファイルのオープン
	std::ifstream file{ file_name };
	// ファイルがオープンできなかったら何もしない
	if (!file) return;
	// スコアの読み込み
	for (int& score : ranking_scores_) {
		file >> score;
	}
}

// 背景の描画
void Result::draw_background()const {
	// 半透明の青い背景を表示
	static const GScolor bg_color{ 1.0f,1.0f,1.0f,0.5f, };
	static const GSvector2 bg_position{ 20.0f,100.0f };
	gsDrawSprite2D(Texture_BlueBack, &bg_position, NULL, NULL, &bg_color, NULL, 0.0f);
}

// プレイヤーの描画
void Result::draw_player() const {
	// 剣道部長の画像の座標を線形補間で動かす
	//static const GSvector2 kendo_position_start{ -1024.0f, 100.0f };
	//static const GSvector2 kendo_position_end{ 370.0f, 100.0f };
	//GSvector2 kendo_position = GSvector2::lerp(kendo_position_start, kendo_position_end, moving_timer_ / MovingTime);

	// プレイヤーの画像の描画
	//gsDrawSprite2D(Texture_Kendo, &kendo_position, NULL, NULL, NULL, NULL, 0.0f);
}

// ランキングの描画
void Result::draw_ranking() const {
	const static GSvector2 position_start1{ 0,0 };
	gsDrawSprite2D(Texture_result, &position_start1, NULL, NULL, NULL, NULL, 0.0f);
	// 「順位」の文字を表示
	//static const GSrect			jyuni_rect{ 0,192,128,256 };				// 切り出し
	//static const GSvector2	jyuni_position{ 40.0f, 360.0f };					// 座標
	//static const GScolor		jyuni_color{ 1.0f, 0.0f, 0.0f, 1.0f };	// 色

	//gsDrawSprite2D(Texture_Text, &jyuni_position, &jyuni_rect, NULL, &jyuni_color, NULL, 0.0f);
	// 上位５位の順位（主席など）と得点を描画
	for (int i = 0; i < (int)ranking_scores_.size(); ++i) {
		GScolor rank_in_color{ 1.0f,1.0f,1.0f,1.0f }; // 白いろ
		// ランクインしたら、その順位の文字を赤色にする
		if (i == final_rank_) {
			rank_in_color = GScolor{ 1.0f,0.0f,0.0f,1.0f }; // 赤いろ
		}
		// 「主席」「次席」「３い」「４い」「５い」のランクを描画
		const GSrect rank_rect{ 128.0f, 60.0f * i, 256.0f, 60.0f * i + 60.0f };
		const GSvector2 rank_position{ 180.0f,360.0f + 60.0f * i };
		//gsDrawSprite2D(Texture_Text, &rank_position, &rank_rect, NULL, &rank_in_color, NULL, 0.0f);
		// ランキングデータ内の得点を描画
		static const ResultTX number{ Texture_Number, 64, 64 };
		number.draw(GSvector2{ 320.0f, 160.0f + 85.0f * i }, ranking_scores_[i], 8, '0', rank_in_color);
	}
}

// 評価メッセージの描画
void Result::draw_grade_message() const {
	// スコアから評価を計算する
	int grades = calculate_grades();
	// 評価のタイトルを表示
	GSrect grades1_rect{ 0.0f,64.0f * grades, 1024, 64.0f * grades + 64.0f };
	static const GSvector2 grades1_position{ 40.0f,120.0f };
	static const GScolor grades1_color{ 1.0f,0.0f,0.0f,1.0f };
	//gsDrawSprite2D(Texture_Result1, &grades1_position, &grades1_rect, NULL, &grades1_color, NULL, 0.0f);
	// 評価の文面を表示
	GSrect grades2_rect{ 0.0f,200.0f * grades, 1024, 200.0f * grades + 180.0f };
	static const GSvector2 grades2_position{ 40.0f, 190.0f };
	//gsDrawSprite2D(Texture_Result2, &grades2_position, &grades2_rect, NULL, NULL, NULL, 0.0f);
}

// スコアから評価を計算する
int Result::calculate_grades() const {
	if (final_score_ >= 60) return 0;
	if (final_score_ >= 50) return 1;
	if (final_score_ >= 40) return 2;
	if (final_score_ >= 30) return 3;
	return 4;
}