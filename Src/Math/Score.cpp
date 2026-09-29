#include "Math/Score.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// 最大スコア
const int ScoreMax = 99999999;

// デフォルトコンストラクタ
Score::Score() :
    score_{ ScoreMax }, max_score_{ ScoreMax } {
}

// スコアの初期化
void Score::initialize() {
    score_ = 0;
}

// スコアの加算
void Score::add(int value) {
    score_ = std::min(score_ + value, max_score_);
}

// スコアの減算
void Score::sub(int value) {
    score_ = std::max(score_ - value, 0);
}

// スコアの取得
int Score::get() const {
    return score_;
}

// スコアの描画
void Score::draw() const {
    static const NumberTexture number{ Texture_Number, 64, 64 };
    GSvector2 drawPos{ -95 + 1280 / 2 , 640 };
    number.draw(drawPos, score_, 8, 8);
}
