#ifndef GAME_PLAY_SCENE_H_
#define GAME_PLAY_SCENE_H_

#include "IScene.h"
#include "World/World.h"
#include "Result.h"
#include "Upgrade.h"

// ゲームプレイシーン
class GamePlayScene : public IScene {
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
    // ゲーム中状態の更新
    void update_playing(float delta_time);
    // リザルト状態の更新
    void update_result(float delta_time);
    // アップグレード状態の更新
    void update_upgrade(float delta_time);

private:
    // ゲームプレイシーンの状態
    enum class State {
        Playing,    // ゲームプレイ中
        Result,      // リザルト中
        Upgrade,
    };
    // 状態
    State   state_{ State::Playing };
    // ワールドクラス
    World   world_;
    // リザルト
    Result  result_;
    // リザルト用タイマ
    float   result_timer_{ 0.0f };
    // 終了フラグ
    bool    is_end_{ false };

    Upgrade upgrade_;
};

#endif

