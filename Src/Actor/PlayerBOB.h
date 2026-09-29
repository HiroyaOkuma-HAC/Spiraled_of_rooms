#ifndef PLAYER_BOB_H_
#define PLAYER_BOB_H_

#include "Actor/Actor.h"
#include "AnimatedMesh.h"

/// プレーヤークラス！
class PlayerBOB : public Actor {
    // プレイヤーの状態
    enum class State {
        Move,           // 移動
        Attack,         // 攻撃
        Attack2,      // ２コンボ攻撃
        Attack3,      // ３コンボ攻撃
        Damage,     // ダメージ中
        Wakeup,     // 起き上がり中
        Graple,
        Run,
        Dead,

        JumpStart,  // ジャンプ開始
        JumpMid,    // ジャンプ中
        JumpEnd,    // ジャンプ終了（着地）
        JumpAttack  // ジャンプ攻撃
    };
public:
    // コンストラクタ
    PlayerBOB(IWorld* world, const GSvector3& position);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;

private:
    // 状態の更新
    void update_state(float delta_time);
    // 状態の変更
    void change_state(State stae, GSuint motion, bool loop = true);
    // 移動
    void move(float delta_time);
    // 攻撃中
    void attack(float delta_time);
    // ２コンボ攻撃中
    void attack2(float delta_time);
    // ３コンボ攻撃中
    void attack3(float delta_time);
    // ダメージ中
    void damage(float delta_time);
    // 起き上がり中
    void wakeup(float delta_time);
    // グラップル
    void graple(float delta_time);
    // 走る
    void run(float delta_time);
    // デッド
    void dead(float delta_time);

    // ジャンプ開始
    void jump_start(float delta_time);
    // ジャンプ中
    void jump_mid(float delta_time);
    // ジャンプ終了（着地）
    void jump_end(float delta_time);
    // ジャンプ攻撃
    void jump_attack(float delta_time);

    /// 追加！
    // フィールドとの衝突処理
    void collide_field();
    // フィールドとの衝突処理 ( 天井)
    void collide_up_field();
    // アクターとの衝突処理
    void collide_actor(Actor& other);

    // 弾の生成
    void generate_bullet();

    // 弾の生成
    void generate_bullet1();

    // グラップル
    void generate_bulletgraple();

    // 武器の描画
    void draw_weapon() const;

    // リロードしてからの経過時間
    float reload_timer_ = { 0.0f };
    // リロード時間
    float reload_time_ = { 0.0f };
    // リロード中か？
    bool reload = false;

    float SCT = 0.0f;

private:
    // アニメーションメッシュ
    AnimatedMesh mesh_;
    // モーション番号
    GSuint motion_;
    // モーションのループ設定
    bool motion_loop_;
    // 状態
    State state_;
    // 状態タイマー
    float state_timer_;

    // ジャンプ攻撃実施済みフラグ
    bool            jump_attack_done_;

    bool walkenable_ = true;
    float walkmanage_ = 0.0f;
    float staminaCT = { 300.0f };

    float color_ = { 1.0f };
    float a_color_ = { 1.0f };
    bool is_dead_ = { false };
    float damage_A{ 0.0f };
};

#endif