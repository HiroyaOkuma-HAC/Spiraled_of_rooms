#ifndef UPGRADE_SHOOT_SPEED_H_
#define UPGRADE_SHOOT_SPEED_H_

// スタミナ
class UpgradeShootSpeed {
public:
    // デフォルトコンストラクタ
    UpgradeShootSpeed();

    // 初期化
    void initialize();

    // 描画
    void draw() const;

    // 加算
    void add(int value);
    // 減算
    void sub(int value);

    // 取得
    int get() const;

private:
    float shootspeed_; // スタミナ
    float max_shootspeed_; // 最大スタミナ
};

#endif
