#include "PlayerHitPoint.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

#include "Math/UpgradeHp.h"

// プレイヤーの最大HP
const int PlayerMaxHitPoint = 250;



void PlayerHitPoint::setUpgradeHp(UpgradeHp* up) {
    upgradehp_ = up;
}

int PlayerHitPoint::getUpgradeHp() const {
    return upgradehp_ ? upgradehp_->get() : 0; // ← nullチェックもバッチリ
}


// デフォルトコンストラクタ
PlayerHitPoint::PlayerHitPoint() :
    hp_{ 50 }, max_hp_{ 50} {
}

// HPの初期化
void PlayerHitPoint::initialize() {
    hp_ = 50;
    max_hp_ = 50;
}

// HP の加算
void PlayerHitPoint::add(int value) {
    hp_ = std::min(hp_ + value, max_hp_);
}

// HP の減算
void PlayerHitPoint::sub(int value) {
    hp_ = std::max(hp_ - value, 0);
}

void PlayerHitPoint::maxup(int value) {
    max_hp_ += value;
    hp_ += value;
}

// HP の取得
int PlayerHitPoint::get() const {
    return hp_;
}

// maxHP の取得
int PlayerHitPoint::getmax() const {
    return max_hp_;
}

// HPの描画
void PlayerHitPoint::draw() const {
    
    GSvector2 Flamepos{ 0,0 };
    // 枠のUIの表示
    gsDrawSprite2D(HPFlame, &Flamepos, NULL, NULL, NULL, NULL, 0);


    // 16, 16 は読み込んだ画像のサイズ
    static const GaugeTexture gauge{ GaugeWhite, GaugeBlack, 16, 16 };
    // ゲージのカラー（半透明）
    static const GScolor gauge_color{ 1.0f, 1.0f, 1.0f, 0.8f };
    // ゲージの背景カラー（半透明）
    static const GScolor background_color{ 1.0f, 1.0f, 1.0f, 0.8f };

    // (120, 640) の位置に、(250, 40)のサイズのゲージを表示
    // ゲージの数値は現在のHP、ゲージ最大値は最大HP。
    gauge.draw(GSvector2{ 120, 640 }, 250, 40, hp_, max_hp_, gauge_color, background_color);
    

    static const NumberTexture number{ Texture_Number, 64, 64 };
    GSvector2 drawPos{-25,590 };
    number.draw(drawPos, hp_, 10);
 




    }

