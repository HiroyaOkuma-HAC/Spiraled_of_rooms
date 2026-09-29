#ifndef PLAYER_HIT_POINT_H_
#define PLAYER_HIT_POINT_H_

class UpgradeHp;

// プレイヤーHP
class PlayerHitPoint {
public:
    // デフォルトコンストラクタ
    PlayerHitPoint();

    void setUpgradeHp(UpgradeHp* up); // ← 外から渡せるようにする！
    int getUpgradeHp() const;


    // HPの初期化
    void initialize();

    // HPの描画
    void draw() const;

    // HP の加算
    void add(int value);
    // HP の減算
    void sub(int value);
    // HP の上限アップ
    void maxup(int value);

    // 現在の HP の取得
    int get() const;
    // 現在の max_hp_ の取得
    int getmax() const;

private:
    int hp_; // HP
    int max_hp_; // 最大HP

    UpgradeHp* upgradehp_ = nullptr; // ← 安全な初期化！

};

#endif
