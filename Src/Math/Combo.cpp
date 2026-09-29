#include "Math/Combo.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// 最大スコア
const int ComboMax = 99999999;

// デフォルトコンストラクタ
Combo::Combo() :
    combo_{ ComboMax }, max_combo_{ ComboMax } {
}

// スコアの初期化
void Combo::initialize() {
    combo_ = 0;
}

// スコアの加算
void Combo::add(int value) {
    combo_ = std::min(combo_ + value, max_combo_);
}

// スコアの減算
void Combo::sub(int value) {
    combo_ = std::max(combo_ - value, 0);
}

// スコアの取得
int Combo::get() const {
    return combo_;
}

// スコアの描画
void Combo::draw() const {
    if (combo_ >= 1) {
        static const NumberTexture number{ Texture_Number, 64, 64 };
        GSvector2 drawPos{ 32 + 1280 / 2, 720 / 2 };
        //number.draw(drawPos, combo_, 3);
    }
}
