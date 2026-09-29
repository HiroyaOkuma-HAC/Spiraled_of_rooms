#ifndef UPGRADE_HP_H_
#define UPGRADE_HP_H_

// スタミナ
class UpgradeHp {
public:
    // デフォルトコンストラクタ
    UpgradeHp();

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
    int upgradehp_; // 
    int max_upgradehp_; // 最大
};

#endif
