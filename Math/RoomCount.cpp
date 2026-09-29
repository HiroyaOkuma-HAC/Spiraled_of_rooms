#include "RoomCount.h"
#include "Rendering/GaugeTexture.h"
#include "Assets.h"
#include <algorithm>
#include "Rendering/NumberTexture.h"

// プレイヤーの最大HP
const int MaxRoomCount = 100;

// デフォルトコンストラクタ
RoomCount::RoomCount() :
    roomcount_{ 1 }, max_roomcount_{ MaxRoomCount } {
}

// HPの初期化
void RoomCount::initialize() {
    roomcount_ = 1;
}

// HP の加算
void RoomCount::add(int value) {
    roomcount_ = std::min(roomcount_ + value, max_roomcount_);
}

// HP の減算
void RoomCount::sub(int value) {
    roomcount_ = std::max(roomcount_ - value, 0);
}

// HP の取得
int RoomCount::get() const {
    return roomcount_;
}

// HPの描画
void RoomCount::draw() const {

    GSvector2 Flamepos{ 0,0 };
    // 枠のUIの表示
    gsDrawSprite2D(Texture_Karate, &Flamepos, NULL, NULL, NULL, NULL, 0);



    static const NumberTexture number{ Texture_Number, 64, 64 };
    GSvector2 drawPos{ 0,30 };
    //number.draw(drawPos, roomcount_, 3);




}
