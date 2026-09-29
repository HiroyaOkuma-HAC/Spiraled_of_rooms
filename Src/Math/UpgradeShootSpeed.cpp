#include "UpgradeShootSpeed.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// プレイヤーの最大スタミナ
const int ShootSpeedMax = 500;

// デフォルトコンストラクタ
UpgradeShootSpeed::UpgradeShootSpeed() :
    shootspeed_{ ShootSpeedMax }, max_shootspeed_{ ShootSpeedMax } {
}

// 初期化
void UpgradeShootSpeed::initialize() {
    shootspeed_ = 30.0f;
}

// 加算
void UpgradeShootSpeed::add(int value) {
    shootspeed_ = std::min(shootspeed_ + value, max_shootspeed_);
}

// 減算
void UpgradeShootSpeed::sub(int value) {
  //  shootspeed_ = std::max(shootspeed_ - value, 0);
}

// 取得
int UpgradeShootSpeed::get() const {
    return shootspeed_;
}

// 描画
void UpgradeShootSpeed::draw() const {



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
