#include "Rendering/GaugeTexture.h"

// コンストラクタ（ゲージ用テクスチャ、背景用テクスチャ、テクスチャ幅、テクスチャ高さ）
GaugeTexture::GaugeTexture(GSuint gauge_texture, GSuint background_texture,
    int texture_width, int texture_height) {
    gauge_texture_ = gauge_texture;
    background_texture_ = background_texture;
    texture_width_ = texture_width;
    texture_height_ = texture_height;
}

// 描画（描画位置、ゲージの幅、ゲージの高さ、ゲージの数値、ゲージの最大値、ゲージの色、ゲージの背景色）
void GaugeTexture::draw(const GSvector2& position,
    int width, int height, int value, int max_value,
    const GScolor& gauge_color, const GScolor& background_color) const {
    // 最大値を超えていたら補正
    if (value > max_value) {
        value = max_value;
    }

    float rate = value / static_cast<float>(max_value); // ゲージの割合を求める
    GSvector2 gauge_scaling; // ゲージの拡大率

    // ゲージの表示
    gauge_scaling.x = (width / static_cast<float>(texture_width_)) * rate; // 横方向の拡大率を求める
    gauge_scaling.y = height / static_cast<float>(texture_height_); // 縦方向の拡大率を求める
    gsDrawSprite2D(gauge_texture_, &position, NULL, NULL, &gauge_color, &gauge_scaling, 0.0f);

    // ゲージの割合が 1.0f 未満の場合
    if (rate < 1.0f) {
        // 背景（ゲージの空部分）の表示
        GSvector2 background_postion; // 背景（ゲージの空部分）の位置
        background_postion.x = position.x + width * rate; // ゲージの右隣の位置に描画
        background_postion.y = position.y;
        GSvector2 background_scaling; // 背景の拡大率
        background_scaling.x = (width / static_cast<float>(texture_width_)) * (1.0f - rate);
        background_scaling.y = gauge_scaling.y;
        gsDrawSprite2D(background_texture_, &background_postion,
            NULL, NULL, &background_color, &background_scaling, 0.0f);
    }
}

