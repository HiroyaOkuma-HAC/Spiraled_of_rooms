#include "UpgradeDeffence.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// プレイヤーの最大スタミナ
const int DeffenceUpMax = 30000;

// デフォルトコンストラクタ
UpgradeDeffence::UpgradeDeffence() :
    deffence_{ DeffenceUpMax }, max_deffence_{ DeffenceUpMax } {
}

// 初期化
void UpgradeDeffence::initialize() {
    deffence_ = 1;
}

// 加算
void UpgradeDeffence::add(int value) {
    deffence_ = std::min(deffence_ + value, max_deffence_);
}

// 減算
void UpgradeDeffence::sub(int value) {
    deffence_ = std::max(deffence_ - value, 0);
}

// 取得
int UpgradeDeffence::get() const {
    return deffence_;
}

// 描画
void UpgradeDeffence::draw() const {



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
