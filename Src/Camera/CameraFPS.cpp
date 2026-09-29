#include "CameraFPS.h"
#include "World/IWorld.h"
#include "Field.h"
#include "Line.h"

// プレーヤーの座標からのオフセット
const GSvector3 PlayerOffset{ 0.0f, 1.8f, -0.1f };
// カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f, 1.8f, 0.0f };

float Fx = 0.0f;
float Fy = 0.0f;
float FlastX = 0.0f;
float FlastY = 0.0f;

// コンストラクタ
CameraFPS::CameraFPS(IWorld* world) {
    // ワールドを設定
    world_ = world;
    // タグの設定
    tag_ = "CameraTag";
    // 名前の設定
    name_ = "Camera";
}

// 更新
void CameraFPS::update(float delta_time) {
    // プレーヤーを検索
    Actor* player = world_->find_actor("Player");
    if (player == nullptr) return;

    // y軸まわりにカメラを回転させる
    if (gsGetKeyState(GKEY_LEFT))  yaw_ += 1.0f * delta_time;
    if (gsGetKeyState(GKEY_RIGHT)) yaw_ -= 1.0f * delta_time;
    // x軸まわりにカメラを回転させる
    if (gsGetKeyState(GKEY_UP))   pitch_ += 1.0f * delta_time;
    if (gsGetKeyState(GKEY_DOWN)) pitch_ -= 1.0f * delta_time;

    /// ////



       // SetCursorPos(640, 360);


            // 視点を移動する処理を書く
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    Fx = cursorPos.x / 3;
    Fy = cursorPos.y / 3;

    yaw_ -= Fx - FlastX;
    pitch_ += Fy - FlastY;

    FlastX = Fx;
    FlastY = Fy;









    /// ////



    // x軸まわりの回転角度の制限をする
    pitch_ = CLAMP(pitch_, -75.0f, 75.0f);

    // 注視点の座標を求める
    GSvector3 at = player->transform().position() + ReferencePointOffset;
    // カメラの座標を求める
    GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;
    // 座標の設定
    transform_.position(position);
    // 注視点の方向を見る
    transform_.lookAt(at);

    // フィールドとの衝突判定
    Line line{ at, position };
    GSvector3 intersect;
    if (world_->field()->collide(line, &intersect)) {
        position = intersect;
    }
    // 視点の位置を設定
    transform_.position(position);
    // 注視点を設定
    transform_.lookAt(at);

}

// 描画
void CameraFPS::draw() const {
    // プレーヤーを検索
    Actor* player = world_->find_actor("Player");
    if (player == nullptr) return;

    // 視点の位置
    GSvector3 eye = PlayerOffset * player->transform().localToWorldMatrix();
    // 注視点の位置
    GSvector3 at = eye + player->transform().forward();
    // カメラの設定
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        eye.x, eye.y, eye.z,     // 視点の位置
        at.x, at.y, at.z,     // 注視点の位置
        0.0f, 1.0f, 0.0f      // 視点の上方向
    );
}
