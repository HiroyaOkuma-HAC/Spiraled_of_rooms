#include "Math/StreakLimit.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// 最大ストリーク制限時間
const int StreakLimitMax = 600.0f;

// デフォルトコンストラクタ
StreakLimit::StreakLimit() :
    streak_limit_{ StreakLimitMax }, max_streak_limit_{ StreakLimitMax } {
}

// ストリーク制限時間の初期化
void StreakLimit::initialize() {
    streak_limit_ = StreakLimitMax;
}

// ストリーク制限時間の加算
void StreakLimit::add(int value) {
    streak_limit_ = std::min(streak_limit_ + value, max_streak_limit_);
}

// ストリーク制限時間の減算
void StreakLimit::sub(int value) {
    streak_limit_ = std::max(streak_limit_ - value, 0);
}

// ストリーク制限時間の取得
int StreakLimit::get() const {
    return streak_limit_;
}

// ストリーク制限時間の描画
void StreakLimit::draw() const {
    // 16, 16 は読み込んだ画像のサイズ
    static const GaugeTexture gauge{ GaugeWhite, GaugeBlack, 16, 16 };
    // ゲージのカラー（半透明）
    static const GScolor gauge_color{ 1.0f, 1.0f, 1.0f, 0.8f };
    // ゲージの背景カラー（半透明）
    static const GScolor background_color{ 1.0f, 1.0f, 1.0f, 0.8f };

    gauge.draw(GSvector2{ 460, 670 }, 363, 10, streak_limit_, max_streak_limit_, gauge_color, background_color);
}
