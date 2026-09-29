#include "UpgradeDamege.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// プレイヤーの最大スタミナ
const int DamageupMax = 50000;

// デフォルトコンストラクタ
UpgradeDamage::UpgradeDamage() :
    damage_{ DamageupMax }, max_damage_{ DamageupMax } {
}

// 初期化
void UpgradeDamage::initialize() {
    damage_ = 30;
}

// 加算
void UpgradeDamage::add(int value) {
    damage_ = std::min(damage_ + value, max_damage_);
}

// 減算
void UpgradeDamage::sub(int value) {
    damage_ = std::max(damage_ - value, 0);
}

// 取得
int UpgradeDamage::get() const {
    return damage_;
}

// 描画
void UpgradeDamage::draw() const {



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
