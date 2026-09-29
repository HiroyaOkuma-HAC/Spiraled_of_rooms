#include "Coin.h"
//#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// プレイヤーの最大スタミナ
const int coinMax = 999999;

// デフォルトコンストラクタ
Coin::Coin() :
    coin_{ coinMax }, max_coin_{ coinMax } {
}

// スタミナの初期化
void Coin::initialize() {
    coin_ = 0;
}

// スタミナの加算
void Coin::add(int value) {
    coin_ = std::min(coin_ + value, max_coin_);
}

// スタミナの減算
void Coin::sub(int value) {
   coin_ = std::max(coin_ - value, 0);
}

// スタミナの取得
int Coin::get() const {
    return coin_;
}

// スタミナの描画
void Coin::draw() const {

    static const NumberTexture number{ Texture_Number, 64, 64 };
    GSvector2 drawPos{ 32 + 1280 / 2, 720 / 2 };
    //number.draw(drawPos, coin_, 6);

    /*
    // 16, 16 は読み込んだ画像のサイズ
    static const GaugeTexture gauge{ GaugeWhite, GaugeBlack, 16, 16 };
    // ゲージのカラー（半透明）
    static const GScolor gauge_color{ 1.0f, 1.0f, 1.0f, 0.8f };
    // ゲージの背景カラー（半透明）
    static const GScolor background_color{ 1.0f, 1.0f, 1.0f, 0.8f };

    // (200, 2) の位置に、(100, 16)のサイズのゲージを表示（この位置とサイズは適当です）。
    // ゲージの数値は現在のHP、ゲージ最大値は最大HP。
    gauge.draw(GSvector2{ 150, 627 }, 220, 7, stamina_, max_stamina_, gauge_color, background_color);
    */


}
