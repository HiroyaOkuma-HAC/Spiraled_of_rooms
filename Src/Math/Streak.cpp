#include "Math/Streak.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/StreakTexture.h"

// 最大ストリーク
const int StreakMax = 10;

// デフォルトコンストラクタ
Streak::Streak() :
    streak_{ StreakMax }, max_streak_{ StreakMax } {
}

// ストリークの初期化
void Streak::initialize() {
    streak_ = 1;
}

// ストリークの加算
void Streak::add(int value) {
    streak_ = std::min(streak_ + value, max_streak_);
}

// ストリークの減算
void Streak::sub(int value) {
    streak_ = std::max(streak_ - value, 0);
}

// ストリークの取得
int Streak::get() const {
    return streak_;
}

// ストリークの描画
void Streak::draw() const {

    static const StreakTexture number{ Texture_Number, 64, 64 };
    GSvector2 drawPos{ 130 + 1280 / 2, -119 + 720 };
   // number.draw(drawPos, streak_,1);
 }
