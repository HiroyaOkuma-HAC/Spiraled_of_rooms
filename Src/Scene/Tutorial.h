#ifndef TUTORIAL_H_
#define TUTORIAL_H_

#include "IScene.h"
#include "World/World.h"
#include "Result.h"
#include "Upgrade.h"

// ゲームプレイシーン
class Tutorial : public IScene {
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

    // ルームを作成
    void create_room();

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

    int blockdata_[41][41][41] = { {{0}} };
    int data_x_ = 0;
    int data_y_ = 0;
    int data_z_ = 0;

    float fadecolor_{ 0.0f };

    float Ma = { 0.6f };
    float Daa = { 0.6f };
    float Sa = { 0.6f };
    float Ha = { 0.6f };
    float Da = { 0.6f };

    int MLevel_ = { 1 };
    int DaLevel_ = { 1 };
    int SLevel_ = { 1 };
    int HLevel_ = { 1 };
    int DLevel_ = { 1 };

    int coincount_ = { 0 };
    int roomcount_ = { 0 };

    int Mmn = { 0 };
    int Man = { 0 };
    int Damn = { 0 };
    int Daan = { 0 };
    int Smn = { 0 };
    int San = { 0 };
    int Hmn = { 0 };
    int Han = { 0 };
    int Dmn = { 0 };
    int Dan = { 0 };

    int hpint_ = { 0 };
    int ammoint_ = { 0 };

    bool endnow_ = { false };

    Upgrade upgrade_;

    bool nextYok = { true };

    int tutorial_tx_ = { 0 };
    float walk_num = { 0.0f };

    // y軸周りの回転角度
    float yaw_{ 0.0f };
    // x軸周りの回転角度
    float pitch_{ 0.0f };
    float lastX{ 0.0f };
    float lastY{ 0.0f };
    float x{ 0.0f };
    float y{ 0.0f };
    int coinnum{ 0 };

    float fadetimer_1t{ 60.0f };
    float fadetimer_2t{ 40.0f };


};

#endif

