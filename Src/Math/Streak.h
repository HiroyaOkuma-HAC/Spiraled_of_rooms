#ifndef STREAK_H_
#define STREAK_H_

// ストリーク
class Streak {
public:
    // デフォルトコンストラクタ
    Streak();

    // ストリークの初期化
    void initialize();

    // ストリークの描画
    void draw() const;

    // ストリークの加算
    void add(int value);
    // ストリークの減算
    void sub(int value);

    // 現在のストリークの取得
    int get() const;

private:
    int streak_; // ストリーク
    int max_streak_; // 最大ストリーク
};

#endif
