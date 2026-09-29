#include "Rendering/TitleFade.h"
#include <sstream>
#include <iomanip>
#include <Assets.h>

// コンストラクタ
TitleFade::TitleFade() {
}

// 更新
void TitleFade::update(float delta_time) const {
}

// 描画
void TitleFade::draw() const {
    const static GSvector2 position_title{ 1280.0f / 2, 720.0f / 2 };
    gsDrawSprite2D(Texture_TitleFade, &position_title, NULL, NULL, NULL, NULL, 0.0f);
}

