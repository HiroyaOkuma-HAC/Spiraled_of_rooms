#ifndef NUMBER_TEXTURE_H_
#define NUMBER_TEXTURE_H_

#include <gslib.h>
#include <string>

// ナンバテクスチャ
class NumberTexture {
public:
    // コンストラクタ
    NumberTexture(GSuint texture, int width, int height);
    // 描画(右詰め)
    void draw(const GSvector2& position, int num, int digit, char fill = ' ') const;

    void rankdraw(const GSvector2& position, int num, int digit, char fill = '0',
        const GScolor& color = GScolor{ 1.0f,1.0f,1.0f,1.0f }) const;

    // 描画(左詰め)
    void draw(const GSvector2& position, int num) const;
    // 描画
    void draw(const GSvector2& position, const std::string& num) const;

    void numberdraw(const GSvector2& position, const std::string& num,
        const GScolor& color = GScolor{ 1.0f,1.0f,1.0f,1.0f }) const;

private:
    // フォント用のテクスチャ
    GSuint	texture_;
    // 文字の幅
    int	width_;
    // 文字の高さ
    int	height_;
};

#endif

