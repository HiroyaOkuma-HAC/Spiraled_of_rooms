#ifndef COMBO_H_
#define COMBO_H_

// コンボ
class Combo {
public:
    // デフォルトコンストラクタ
    Combo();

    // コンボの初期化
    void initialize();

    // コンボの描画
    void draw() const;

    // コンボの加算
    void add(int value);
    // コンボの減算
    void sub(int value);

    // 現在のコンボの取得
    int get() const;

private:
    int combo_; // コンボ
    int max_combo_; // 最大コンボ
};

#endif
