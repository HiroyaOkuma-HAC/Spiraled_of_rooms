#include "UpgradeHp.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// プレイヤーの最大スタミナ
const int HpUpMax = 30000;

// デフォルトコンストラクタ
UpgradeHp::UpgradeHp() :
    upgradehp_{ 100 }, max_upgradehp_{ HpUpMax } {
}

// 初期化
void UpgradeHp::initialize() {
    upgradehp_ = 100;
}

// 加算
void UpgradeHp::add(int value) {
    upgradehp_ = std::min(upgradehp_ + value, max_upgradehp_);
}

// 減算
void UpgradeHp::sub(int value) {
    upgradehp_ = std::max(upgradehp_ - value, 0);
}

// 取得
int UpgradeHp::get() const {
   return 0;
}

// 描画
void UpgradeHp::draw() const {



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
