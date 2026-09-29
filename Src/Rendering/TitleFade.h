#ifndef TITLE_FADE_H_
#define TITLE_FADE_H_

#include <gslib.h>


class TitleFade {
public:
    // コンストラクタ
    TitleFade();
    void update(float delta_time) const;
    // 描画
    void draw() const;

private:
    // フォント用のテクスチャ
    GSuint	texture_;
};


#endif //TITLE_FADE_H_