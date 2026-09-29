#ifndef STREAK_LIMIT_H_
#define STREAK_LIMIT_H_

// ストリークの制限時間
class StreakLimit {
public:
    // デフォルトコンストラクタ
    StreakLimit();

    // ストリークの制限時間の初期化
    void initialize();

    // ストリークの制限時間の描画
    void draw() const;

    // ストリークの制限時間の加算
    void add(int value);
    // ストリークの制限時間の減算
    void sub(int value);

    // 現在のストリークの制限時間の取得
    int get() const;

private:
    int streak_limit_; // ストリークの制限時間
    int max_streak_limit_; // 最大ストリークの制限時間
};

#endif
