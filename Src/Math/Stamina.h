#ifndef STAMINA_H_
#define STAMINA_H_

// スタミナ
class Stamina {
public:
    // デフォルトコンストラクタ
    Stamina();

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
    int stamina_; // スタミナ
    int max_stamina_; // 最大スタミナ
};

#endif
