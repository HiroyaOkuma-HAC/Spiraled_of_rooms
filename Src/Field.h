#ifndef FIELD_H_
#define FIELD_H_

#include <gslib.h>
#include "FieldActorManager.h"

class Ray;
class Line;
class BoundingSphere;

// フィールドクラス
class Field {
public:
    // コンストラクタ
    Field(GSuint octree, GSuint collider, GSuint skybox);
    // デストラクタ
    ~Field();
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;
    // フィールド用アクターの描画
    void draw_actors() const;

    // レイとの衝突判定
    bool collide(
        const Ray& ray, float max_distance,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr,
        Actor** actor = nullptr) const;
    // 線分との衝突判定
    bool collide(
        const Line& line,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr,
        Actor** actor = nullptr) const;
    // 球体との衝突判定
    bool collide(
        const BoundingSphere& sphere,
        GSvector3* center = nullptr,
        Actor** actor = nullptr) const;

    // フィールド用アクターの追加
    void add(Actor* actor);
    // フィールド用アクターの消去
    void clear();

    // コピー禁止
    Field(const Field& other) = delete;
    Field& operator = (const Field& other) = delete;

private:
    // 描画用オクツリー
    GSuint  octree_;
    // 衝突判定判定用オクツリー
    GSuint  collider_;
    // スカイボックス
    GSuint  skybox_;
    // フィールドアクターマネージャー
    FieldActorManager actors_;
};

#endif