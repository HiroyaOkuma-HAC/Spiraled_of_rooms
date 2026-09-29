#ifndef ROOM_COUNT_H_
#define ROOM_COUNT_H_


class RoomCount {
public:
    // デフォルトコンストラクタ
    RoomCount();

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
    int roomcount_; // HP
    int max_roomcount_; // 最大HP
};

#endif
