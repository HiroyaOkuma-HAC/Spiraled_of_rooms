#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "IScene.h"
#include <gslib.h>

// タイトルシーン
class TitleScene : public IScene {
public:
    // 開始
    virtual void start() override;
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 終了しているか？
    virtual bool is_end() const override;
    // 次のシーンを返す
    virtual std::string next() const override;
    // 終了
    virtual void end() override;

private:

    // 終了フラグ
    bool is_end_{ false };

    float fadetimer_ = { 0.0f };
    float fadetimer_2 = { 0.0f };
    float fadetimer_3 = { 0.0f };
    float fadetimer_4 = { 0.0f };
    float fadetimer_5 = { 0.0f };
    float fadetimer_6 = { 0.0f };
    bool Space_ = { false };
    int next_sc_ = { 0 };

    float fadetimer_1a{ 60.0f };
    float fadetimer_2a{ 40.0f };

};

#endif

