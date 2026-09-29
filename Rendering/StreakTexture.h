#ifndef STREAK_TEXTURE_H_
#define STREAK_TEXTURE_H_

#include <gslib.h>
#include <string>

// ナンバテクスチャ
class StreakTexture {
public:
    // コンストラクタ
    StreakTexture(GSuint texture, int width, int height);
    // 描画(右詰め)
    void draw(const GSvector2& position, int num, int digit, char fill = ' ') const;
    // 描画(左詰め)
    void draw(const GSvector2& position, int num) const;
    // 描画
    void draw(const GSvector2& position, const std::string& num) const;

private:
    // フォント用のテクスチャ
    GSuint	texture_;
    // 文字の幅
    int	width_;
    // 文字の高さ
    int	height_;
};

#endif

