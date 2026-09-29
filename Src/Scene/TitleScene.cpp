#include "Scene/TitleScene.h"
#include "Assets.h"


#include "Rendering/TitleFade.h"

int start_color_counter_ = 0;

float Tsca_{ 0.6f };
float Ssca_{ 0.6f };

// 開始
void TitleScene::start() {
    // 画像の読み込み
    gsLoadTexture(Texture_Title, "Assets/texture/title.png");
    gsLoadTexture(Texture_Start, "Assets/texture/pushspace.png");
    gsLoadTexture(Texture_Kendo, "Assets/texture/mini_kendo.png");
    gsLoadTexture(Texture_Karate, "Assets/texture/mini_karate.png");

    gsLoadTexture(Texture_TitleFade, "Assets/texture/TitleFade.png");
    gsLoadTexture(Texture_loading, "Assets/texture/loading.png");

    gsLoadTexture(Texture_TUTORIAL, "Assets/texture/TUTORIAL.jpg");
    gsLoadTexture(Texture_SCOREATTACK, "Assets/texture/SCOREATTACK.jpg");

    gsLoadSE(Se_Click, "Assets/sound/Click.wav", 1, GWAVE_DEFAULT);


    // タイトルシーンBGMの読み込み
    gsLoadBGM(Sound_ResultBGM, "Assets/sound/RoomScene.ogg", GS_TRUE);
    // BGMの再生
    gsPlayBGM(Sound_TitleBGM);

    // 終了フラグの初期化
    is_end_ = false;

    fadetimer_1a = 60.0f;
    fadetimer_2a = 40.0f;
}

// 更新
void TitleScene::update(float delta_time) {

    if (fadetimer_1a > 0) fadetimer_1a -= delta_time;
    if (fadetimer_2a > 0) fadetimer_2a -= delta_time;

    // スペースキーを押したらシーン終了
    if (gsGetKeyTrigger(GKEY_S)) {
        Space_ = true;
        next_sc_ = 0;
    }
    if (gsGetKeyTrigger(GKEY_0)) {
        Space_ = true;
        next_sc_ = 1;
    }
    start_color_counter_++;

    if (Space_) {
        fadetimer_ += delta_time ;

    }
    if (Space_ && fadetimer_ >= 10.0f) {
        fadetimer_2 += delta_time;
    }
    if (Space_ && fadetimer_2 >= 30.0f) {
        fadetimer_3 += delta_time;
    }
    if (Space_ && fadetimer_3 >= 10.0f) {
        fadetimer_4 += delta_time;
    }
    if (Space_ && fadetimer_4 >= 30.0f) {
        fadetimer_5 += delta_time;
    }
    if (Space_ && fadetimer_5 >= 10.0f) {
        fadetimer_6 += delta_time;
    }
    if (Space_ && fadetimer_6 >= 180.0f) {
        is_end_ = true;     // シーン終了
        fadetimer_ = 0.0f;
        fadetimer_2 = 0.0f;
        fadetimer_3 = 0.0f;
        fadetimer_4 = 0.0f;
        fadetimer_5 = 0.0f;
        fadetimer_6 = 0.0f;
        Space_ = false;
    }

    POINT cursorPos;
    if (GetCursorPos(&cursorPos) && Space_ == false ) {

        if (cursorPos.x >= 1000 - 128 &&
            cursorPos.x <= 1000 + 128 &&
            cursorPos.y >= 520 - 40 &&
            cursorPos.y <= 520 + 40
            ) {
            if (Tsca_ < 0.65f) Tsca_ += 0.02f;
            if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
                gsPlaySE(Se_Click);
                Space_ = true;
                next_sc_ = 1;
            }
        }

        else if (Tsca_ > 0.6f) Tsca_ -= 0.02f;


        if (cursorPos.x >= 1000 - 128 &&
            cursorPos.x <= 1000 + 128 &&
            cursorPos.y >= 620 - 40 &&
            cursorPos.y <= 620 + 40
            ) {
            if (Ssca_ < 0.65f) Ssca_ += 0.02f;
            if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
                gsPlaySE(Se_Click);
                Space_ = true;
                next_sc_ = 0;
            }
        }

        else if (Ssca_ > 0.6f) Ssca_ -= 0.02f;
    }
}

// 描画
void TitleScene::draw() const {



    // タイトル画面（背景）の描画
    const static GSvector2 position_title{ 0.0f, 0.0f };
    gsDrawSprite2D(Texture_Title, &position_title, NULL, NULL, NULL, NULL, 0.0f);


    GSvector2 Tpos{ 1000,520 };
    GSvector2 Tcen{ 256,64 };
    GSvector2 Tsca{ Tsca_,Tsca_ };
    gsDrawSprite2D(Texture_TUTORIAL, &Tpos, NULL, &Tcen, NULL, &Tsca, 0.0f);

    GSvector2 Spos{ 1000,620 };
    GSvector2 Scen{ 256,64 };
    GSvector2 Ssca{ Ssca_,Ssca_ };
    gsDrawSprite2D(Texture_SCOREATTACK, &Spos, NULL, &Scen, NULL, &Ssca, 0.0f);

    


    // 開始ボタンを押忍！マークの描画
    const static GSvector2 position_start{ 0,0 };
        GScolor color_start{ 1.0f,1.0f,1.0f,0.3f };
        //gsDrawSprite2D(Texture_Start, &position_start, NULL, NULL, NULL, NULL, 0.0f);
    

            const static GSvector2 position1{ 1280.0f / 2, 720.0f / 2 };
            GSvector2 scale1 = { fadetimer_,fadetimer_ };
            GSvector2 center1 = { 16,16 };
            GScolor4 color1 = { 1.0f,1.0f,1.0f,0.3f };
            gsDrawSprite2D(Texture_TitleFade, &position1, NULL, &center1, &color1, &scale1, fadetimer_ * 2);

            const static GSvector2 position2{ 1280.0f / 2, 720.0f / 2 };
            GSvector2 scale2 = { fadetimer_2,fadetimer_2 };
            GSvector2 center2 = { 16,16 };
            GScolor4 color2 = { 0.0f,0.0f,0.0f,0.4f };
            gsDrawSprite2D(Texture_TitleFade, &position2, NULL, &center2, &color2, &scale2, fadetimer_ * 2);

            const static GSvector2 position3{ 1280.0f / 2, 720.0f / 2 };
            GSvector2 scale3 = { fadetimer_3,fadetimer_3 };
            GSvector2 center3 = { 16,16 };
            GScolor4 color3 = { 1.0f,1.0f,1.0f,0.4f };
            gsDrawSprite2D(Texture_TitleFade, &position3, NULL, &center3, &color3, &scale3, fadetimer_ * 2);

            const static GSvector2 position4{ 1280.0f / 2, 720.0f / 2 };
            GSvector2 scale4 = { fadetimer_4,fadetimer_4 };
            GSvector2 center4 = { 16,16 };
            GScolor4 color4 = { 0.0f,0.0f,0.0f,1.0 };
            gsDrawSprite2D(Texture_TitleFade, &position4, NULL, &center4, &color4, &scale4, fadetimer_ * 2);

            const static GSvector2 position5{ 1280.0f / 2, 720.0f / 2 };
            GSvector2 scale5 = { fadetimer_5,fadetimer_5 };
            GSvector2 center5 = { 16,16 };
            GScolor4 color5 = { 1.0f,1.0f,1.0f,1.0 };
            gsDrawSprite2D(Texture_TitleFade, &position5, NULL, &center5, &color5, &scale5, fadetimer_ * 2);

            const static GSvector2 position6{ 1280.0f / 2, 720.0f / 2 };
            GSvector2 scale6 = { fadetimer_6,fadetimer_6 };
            GSvector2 center6 = { 16,16 };
            GScolor4 color6 = { 0.0f,0.0f,0.0f,1.0 };
            gsDrawSprite2D(Texture_TitleFade, &position6, NULL, &center6, &color6, &scale6, fadetimer_ * 2);


            if (fadetimer_1a > 0) {
                const static GSvector2 position5{ 1280.0f / 2, 720.0f / 2 };
                GSvector2 scale5 = { fadetimer_1a,fadetimer_1a };
                GSvector2 center5 = { 16,16 };
                GScolor4 color5 = { 1.0f,1.0f,1.0f,1.0 };
                gsDrawSprite2D(Texture_TitleFade, &position5, NULL, &center5, &color5, &scale5, fadetimer_1a * 2);
            }

            if (fadetimer_2a > 0) {
                const static GSvector2 position6{ 1280.0f / 2, 720.0f / 2 };
                GSvector2 scale6 = { fadetimer_2a,fadetimer_2a };
                GSvector2 center6 = { 16,16 };
                GScolor4 color6 = { 0.0f,0.0f,0.0f,1.0 };
                gsDrawSprite2D(Texture_TitleFade, &position6, NULL, &center6, &color6, &scale6, fadetimer_1a * 2);
            }
}

// 終了しているか？
bool TitleScene::is_end() const {
    return is_end_;         // 終了フラグを返す
}

// 次のシーン名を返す
std::string TitleScene::next() const {
    if (next_sc_ == 0)
    return "LoadRoomScene"; // 次のシーン名を返す
    if (next_sc_ == 1)
        return "LoadTutrial"; // 次のシーン名を返す
}

// 終了
void TitleScene::end() {
    // BGMの停止
    gsStopBGM();
    // 画像の削除
    gsDeleteTexture(Texture_Title);
    gsDeleteTexture(Texture_Kendo);
    gsDeleteTexture(Texture_Karate);
    gsDeleteTexture(Texture_Start);
    gsDeleteBGM(Sound_TitleBGM);

}
