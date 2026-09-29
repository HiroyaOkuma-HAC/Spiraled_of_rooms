#ifndef UPGRADE_MAGAZINE_H_
#define UPGRADE_MAGAZINE_H_

// スタミナ
class UpgradeMagazine {
public:
    // デフォルトコンストラクタ
    UpgradeMagazine();

    // スタミナの初期化
    void initialize();

    // スタミナの描画
    void draw() const;

    // スタミナの加算
    void add(int value);
    // スタミナの減算
    void sub(int value);

    // 現在のスタミナの取得
    int get() const;

private:
    int magazine_; // スタミナ
    int max_magazine_; // 最大スタミナ
};

#endif
