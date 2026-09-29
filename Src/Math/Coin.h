#ifndef COIN_H_
#define COIN_H_

// スタミナ
class Coin {
public:
    // デフォルトコンストラクタ
    Coin();

    // コインの初期化
    void initialize();

    // コインの描画
    void draw() const;

    // コインの加算
    void add(int value);
    // スタミナの減算
    void sub(int value);

    // 現在のコインの取得
    int get() const;

private:
    int coin_; // コイン
    int max_coin_; // 最大コイン
};

#endif
