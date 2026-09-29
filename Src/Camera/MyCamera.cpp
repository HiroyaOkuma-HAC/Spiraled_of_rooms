#include "Camera/Mycamera.h"
#include "World/IWorld.h"
#include "Field.h"
#include "Line.h"

#include <windows.h>
#include <hidusage.h>





// プレーヤーからの相対位置（z座標のみ）
const GSvector3 PlayerOffset{ 0.0f, 0.0f, -5.0f };


// カメラの注視点の補正値
const GSvector3 ReferencePointOffset{ 0.0f, 1.8f, 0.0f };
// ADSモード
const GSvector3 ADSOffset{ 0.0f, 1.55f, 0.5f };

float x = 0.0f;
float y = 0.0f;
float lastX = 0.0f;
float lastY = 0.0f;

float pitch_manager_ = 0.0f;


// コンストラクタ
MyCamera::MyCamera(IWorld* world,
    const GSvector3& position, const GSvector3& at) {
    // ワールドを設定
    world_ = world;
    // タグの設定
    tag_ = "CameraTag";
    // 名前の設定
    name_ = "Camera";
    // 視点の位置を設定
    transform_.position(position);
    // 注視点を設定
    transform_.lookAt(at);
    // ｘ軸周りの回転角度の初期化
    pitch_ = (at - position).getPitch();
    // ｙ軸周りの回転角度の初期化
    //yaw_ = (at - position).getYaw();
    yaw_ = 90.0f;
}

// 更新
void MyCamera::update(float delta_time) {


    // デバッグ表示
  //  printf("[yaw %.0f] [pitch %.0f] \n", yaw_, pitch_);


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





    //　５５　＞　おす　＞　　４０になってほしい　ー１５
    //ー５５　＞　おす　＞　１１５になってほしい　＋１７０




    /// ////
    pitch_manager_ = pitch_;

    if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_2)) {
        
        pitch_ = 72.0f - pitch_manager_;
        yaw_ += 180.0f;
    }
    if (gsGetMouseButtonDetach(GMOUSE_BUTTON_2)) {
        pitch_ = 72.0f - pitch_manager_;
        yaw_ -= 180.0f;
    }
  
    if (gsGetMouseButtonState(GMOUSE_BUTTON_2)) {
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        x = cursorPos.x / 3;
        y = cursorPos.y / 3;

        yaw_ -= x - lastX;
        pitch_ -= y - lastY;

        lastX = x;
        lastY = y;

        // x軸まわりの回転角度の制限をする
        pitch_ = CLAMP(pitch_, 13.0f, 127.0f);
        // 視点の位置
        GSvector3 eye = ADSOffset * player->transform().localToWorldMatrix();
        // 注視点の位置
        GSvector3 at = eye + player->transform().forward();
        // カメラの座標を求める
        GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * ADSOffset;
        // 座標の設定
        transform_.position(position);
        // 注視点の方向を見る
        transform_.lookAt(at);

        // 視点の位置を設定
        transform_.position(position);
        // 注視点を設定
        transform_.lookAt(at);
    }
    else {
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        x = cursorPos.x / 3;
        y = cursorPos.y / 3;

        yaw_ -= x - lastX;
        pitch_ += y - lastY;

        lastX = x;
        lastY = y;

        // x軸まわりの回転角度の制限をする
        pitch_ = CLAMP(pitch_, -53.0f, 57.0f);
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

}

// 描画
void MyCamera::draw() const {

 
 

        // 視点の位置
        GSvector3 eye = transform_.position();
        // 注視点の位置
        GSvector3 at = eye + transform_.forward();
        // 視点の上方向
        GSvector3 up = transform_.up();
        // 視点の位置
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            eye.x, eye.y, eye.z,     // 視点の位置
            at.x, at.y, at.z,     // 注視点の位置
            up.x, up.y, up.z      // 視点の上方向
        );
}

