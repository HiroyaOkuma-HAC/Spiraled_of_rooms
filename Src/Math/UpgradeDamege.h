#ifndef UPGRADE_DAMAGE_H_
#define UPGRADE_DAMAGE_H_

// スタミナ
class UpgradeDamage {
public:
    // デフォルトコンストラクタ
    UpgradeDamage();

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
    int damage_; // スタミナ
    int max_damage_; // 最大スタミナ
};

#endif
