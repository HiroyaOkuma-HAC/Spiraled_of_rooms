#ifndef UPGRADE_DEFFENCE_H_
#define UPGRADE_DEFFENCE_H_

// スタミナ
class UpgradeDeffence {
public:
    // デフォルトコンストラクタ
    UpgradeDeffence();

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
    int deffence_; // 
    int max_deffence_; // 最大
};

#endif
