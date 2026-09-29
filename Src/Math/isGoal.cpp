#include "isGoal.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// プレイヤーの最大スタミナ
const int isGoalMax = 10;

// デフォルトコンストラクタ
isGoal::isGoal() :
    isgoal_{ isGoalMax }, max_isgoal_{ isGoalMax } {
}

// 初期化
void isGoal::initialize() {
    isgoal_ = 0;
}

// 加算
void isGoal::add(int value) {
    isgoal_ = std::min(isgoal_ + value, max_isgoal_);
}

// 減算
void isGoal::sub(int value) {
    isgoal_ = value;
}

// 取得
int isGoal::get() const {
    return isgoal_;
}

// 描画
void isGoal::draw() const {



    // 16, 16 は読み込んだ画像のサイズ
    //static const GaugeTexture gauge{ GaugeWhite, GaugeBlack, 16, 16 };
    // ゲージのカラー（半透明）
    //static const GScolor gauge_color{ 1.0f, 1.0f, 1.0f, 0.8f };
    // ゲージの背景カラー（半透明）
    //static const GScolor background_color{ 1.0f, 1.0f, 1.0f, 0.8f };

    // (200, 2) の位置に、(100, 16)のサイズのゲージを表示（この位置とサイズは適当です）。
    // ゲージの数値は現在のHP、ゲージ最大値は最大HP。
    //gauge.draw(GSvector2{ 150, 627 }, 220, 7, stamina_, max_stamina_, gauge_color, background_color);



}
