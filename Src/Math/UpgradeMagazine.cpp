#include "UpgradeMagazine.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// プレイヤーの最大スタミナ
const int UpgradeMagazineMax = 500;

// デフォルトコンストラクタ
UpgradeMagazine::UpgradeMagazine() :
    magazine_{ UpgradeMagazineMax }, max_magazine_{ UpgradeMagazineMax } {
}

// スタミナの初期化
void UpgradeMagazine::initialize() {
    magazine_ = 30;
}

// スタミナの加算
void UpgradeMagazine::add(int value) {
    magazine_ = std::min(magazine_ + value, max_magazine_);
}

// スタミナの減算
void UpgradeMagazine::sub(int value) {
    magazine_ = std::max(magazine_ - value, 0);
}

// スタミナの取得
int UpgradeMagazine::get() const {
    return magazine_;
}

// スタミナの描画
void UpgradeMagazine::draw() const {



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
