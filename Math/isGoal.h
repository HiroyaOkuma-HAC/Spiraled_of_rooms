#ifndef IS_GOAL_H_
#define IS_GOAL_H_

// ゴールしているか
class isGoal {
public:
    // デフォルトコンストラクタ
    isGoal();

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
    int isgoal_; // スタミナ
    int max_isgoal_; // 最大スタミナ
};

#endif
