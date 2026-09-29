#include <GSgame.h>
#include "Scene/SceneManager.h"
#include "Scene/GamePlayScene.h"
#include "Scene/TitleScene.h"
#include "Scene/RoomScene.h"
#include  "Scene/LoadRoomScene.h"
#include "Scene/Tutorial.h"
#include "Scene/LoadTutrial.h"

#include "Tween/Tween.h"



/// エフェクト用！
#include <GSeffect.h>

// マイゲームクラス
class MyGame : public gslib::Game {
public:
    // コンストラクタ
    //MyGame() : gslib::Game{ 1024, 768,true } {
    //MyGame() : gslib::Game{ 1024/2, 768/2 ,false} {
      MyGame() :gslib::Game{ 1280,720,true } {
    }
    // 開始
    void start() override {
        // エフェクトの初期化
        gsInitEffect();

        scene_manager_.add("GamePlayScene", new GamePlayScene());
        scene_manager_.add("TitleScene", new TitleScene());
        scene_manager_.add("LoadRoomScene", new LoadRoomScene());
        scene_manager_.add("Tutrial", new Tutorial());

        scene_manager_.add("RoomScene", new RoomScene());
        scene_manager_.add("LoadTutrial", new LoadTutrial());

        scene_manager_.change("TitleScene");
        /// 開始シーン！
       // scene_manager_.change("RoomScene");
    }
    // 更新
    void update(float delta_time) override {
        if (gsGetKeyTrigger(GKEY_Z)) {
           // scene_manager_.change("RoomScene");
        }
            scene_manager_.update(delta_time);
            // Tweenの更新
            Tween::update(delta_time);
    }
    // 描画
    void draw() override {
        scene_manager_.draw();
    }
    // 終了
    void end() override {
        // エフェクトの終了
        gsFinishEffect();

        scene_manager_.end();
        scene_manager_.clear();
    }

private:
    // シーンマネージャー
    SceneManager scene_manager_;
};

// main関数
int main() {
    return MyGame().run();
}

