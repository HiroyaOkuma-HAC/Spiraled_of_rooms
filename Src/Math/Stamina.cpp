#include "Stamina.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// プレイヤーの最大スタミナ
const int StaminaMax = 500;

// デフォルトコンストラクタ
Stamina::Stamina() :
    stamina_{ StaminaMax }, max_stamina_{ StaminaMax } {
}

// スタミナの初期化
void Stamina::initialize() {
    stamina_ = StaminaMax;
}

// スタミナの加算
void Stamina::add(int value) {
    stamina_ = std::min(stamina_ + value, max_stamina_);
}

// スタミナの減算
void Stamina::sub(int value) {
    stamina_ = std::max(stamina_ - value, 0);
}

// スタミナの取得
int Stamina::get() const {
    return stamina_;
}

// スタミナの描画
void Stamina::draw() const {

    

    // 16, 16 は読み込んだ画像のサイズ
    static const GaugeTexture gauge{ GaugeWhite, GaugeBlack, 16, 16 };
    // ゲージのカラー（半透明）
    static const GScolor gauge_color{ 1.0f, 1.0f, 1.0f, 0.8f };
    // ゲージの背景カラー（半透明）
    static const GScolor background_color{ 1.0f, 1.0f, 1.0f, 0.8f };

    // (200, 2) の位置に、(100, 16)のサイズのゲージを表示（この位置とサイズは適当です）。
    // ゲージの数値は現在のHP、ゲージ最大値は最大HP。
    gauge.draw(GSvector2{ 150, 627 }, 220, 7, stamina_, max_stamina_, gauge_color, background_color);



}
