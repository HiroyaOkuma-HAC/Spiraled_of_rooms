#ifndef SCORE_H_
#define SCORE_H_

// スコア
class Score {
public:
    // デフォルトコンストラクタ
    Score();

    // スコアの初期化
    void initialize();

    // スコアの描画
    void draw() const;

    // スコアの加算
    void add(int value);
    // スコアの減算
    void sub(int value);

    // 現在のスコアの取得
    int get() const;

private:
    int score_; // スコア
    int max_score_; // 最大スコア
};

#endif
