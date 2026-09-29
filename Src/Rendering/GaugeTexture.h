#ifndef GAUGE_TEXTURE_H_
#define GAUGE_TEXTURE_H_

#include <gslib.h>

// ゲージテクスチャ
class GaugeTexture {
public:
    // コンストラクタ（ゲージ用テクスチャ、背景用テクスチャ、テクスチャ幅、テクスチャ高さ）
    GaugeTexture(GSuint gauge_texture, GSuint background_texture,
        int texture_width, int texture_height);

    // 描画（描画位置、ゲージの幅、ゲージの高さ、ゲージの数値、ゲージの最大値、ゲージの色）
    void draw(const GSvector2& position,
        int width, int height, int value, int max_value,
        const GScolor& gauge_color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f },
        const GScolor& background_color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) const;

private:
    // ゲージのテクスチャ
    GSuint gauge_texture_;
    // 背景のテクスチャ
    GSuint background_texture_;
    // テクスチャの幅
    int texture_width_;
    // テクスチャの高さ
    int texture_height_;
};

#endif
