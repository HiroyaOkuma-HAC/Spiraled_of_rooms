#include "Ammo.h"
#include "Rendering/NumberTexture.h"
#include "Assets.h"
#include <gslib.h>
#include <algorithm> // std::min関数のために必要

#include "World/IWorld.h"

// コンストラクタ
Ammo::Ammo(int ammo) :
    ammo_{ ammo } {
}

// スコアの初期化
void Ammo::initialize(int ammo) {
    ammo_ = 30;
}

// スコアの加算
void Ammo::add(int ammo) {
    // スコアの上限値を9999999とする
    ammo_ = std::min(ammo_ + ammo, 9999999);
}

// スコアの描画
void Ammo::draw() const {
    static const NumberTexture number{ Texture_Number, 64, 64 };
    GSvector2 drawPos{ 1280 - 226 - 40 - 160 ,590 };
    number.draw(drawPos, ammo_, 10);
}

// スコアの取得
int Ammo::get() const {
    return ammo_;
}


// スコアのクリア
void Ammo::clear() {
    ammo_ = 0;
}

