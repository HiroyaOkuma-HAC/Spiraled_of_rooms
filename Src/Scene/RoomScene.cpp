#include "Scene/RoomScene.h"
#include "Actor/Player.h"
#include "Actor/PlayerBOB.h"
#include "Field.h"
#include "Light.h"
#include "Camera/CameraTPS.h"
#include "Assets.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Enemy/EnemyGenerator.h"
#include "Math/Score.h"
#include "Rendering/NumberTexture.h"
#include "Actor/Enemy/GOLDKARATE.h"

#include "Radar.h"

#include <GSstandard_shader.h>
#include "Camera/Mycamera.h"

#include "Actor/Prop/MovingFloor.h"
#include "Camera/CameraFPS.h"

#include "Upgrade.h"
#include "Math/UpgradeMagazine.h"

#include "Actor/Prop/Block.h"

#include "Tween/Tween.h"

#include "Actor/Prop/Goal.h"
#include "Actor/Prop/EnemySpowner.h"
#include "Actor/Prop/MoveBlock.h"
#include "Actor/Prop/TrapBlock.h"

const int UPM = 2;
const int UPDA = 10;
const float UPS = 1.0f;
const int UPH = 20;
const int UPD = 1;

// 開始
void RoomScene::start() {

    // デフォルトシェーダーの初期化 (メッシュファイルを読み込む前に有効にする)
    gsInitDefaultShader();
    // シャドウマップの作成
    static const GSuint shadow_map_size[] = { 2048, 2048 };
    gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
    // シャドウマップを適用する距離 (視点からの距離)
    gsSetShadowMapDistance(60.0f);
    // カスケードシャドウマップの分割位置を調整 (デフォルトは0.5)
    gsSetShadowMapCascadeLamda(0.7f);
    // シャドウの濃さを設定 ( 0.0：濃い～ 1.0：薄い)
    gsSetShadowMapAttenuation(0.0f);
 

    // BGMの再生
    /// gsPlayBGM(Sound_PlayingBGM);
    // 開始効果音（いくぜ！の掛け声）
    //gsPlaySE(Se_GameStart);
    gsSetVolumeBGM(0.0f);
    gsPlayBGM(Sound_PlayingBGM);

    // フィールドクラスの追加
    world_.add_field(new Field{ Octree_Koutei, Octree_KouteiCollider, Texture_Skybox });
    // カメラクラスの追加
   // world_.add_camera(new CameraTPS{
      //        &world_, GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });
      // カメラクラスの追加
    world_.add_camera(new MyCamera{
              &world_, GSvector3{0.0f, 3.2f, -4.8f}, GSvector3{0.0f, 1.92, 0.0f} });

    //FPS カメラクラスの追加
    //world_.add_camera(new CameraFPS{ &world_ });


      // ライトクラスの追加
    world_.add_light(new Light{ &world_ });
    // プレーヤーを追加
    //world_.add_actor(new Player{ &world_, GSvector3{ 3.0f * 7.5f, 30.0f,  3.0f * 7.5f } });

    world_.add_actor(new Player{ &world_, GSvector3{ 3.0f * 7.5f, 30.0f,  3.0f * 7.5f } });

    // 敵生成クラスを追加
    //world_.add_actor(new EnemyGenerator{ &world_ });
    // レーダークラスを追加
   /// world_.add_actor(new Radar{ &world_ });

    // スコアの初期化
    world_.score().initialize();
    // タイマの初期化
    world_.timer().initialize(300.0f);
    // リザルトの初期化
    result_.initialize();
    // ランキングファイルの読み込み
    result_.load("Assets/ranking.txt");
    // 状態の初期化
    state_ = State::Playing;
    //　シーン終了フラグの初期化
    is_end_ = false;

    // フォグの設定
    const static float fog_color[4]{ 1.0f,0.723f,0.216f,1.0f };
    const static float fog_start{ 10.0f };
    const static float fog_end{ 300.0f };
    glFogi(GL_FOG_MODE, GL_LINEAR); // 線形フォグ
    glFogfv(GL_COLOR, fog_color);           // フォグの色
    glFogf(GL_FOG_START, fog_start);    // フォグの開始位置 (視点からの距離)
    glFogf(GL_FOG_END, fog_end);            // フォグの終了位置 (視点からの距離)
    glEnable(GL_FOG);

    // 移動床を追加する
    world_.field()->add(new MovingFloor{ &world_,GSvector3{ 29.0f, 0.0, -5.5f }, Mesh_SlideDoor, Mesh_SlideDoor, 1, 0 });
    world_.field()->add(new MovingFloor{ &world_,GSvector3{ 29.0f, 0.0, -9.39f }, Mesh_SlideDoor, Mesh_SlideDoor ,0 ,0 });


    create_room();
    

    // コインの初期化！
    world_.coin().initialize();

    gsSetMouseCursorPosition(0, 720 / 2);

    world_.upgrademagazine().initialize();
    world_.upgradedamage().initialize();
    world_.upgradeshootspeed().initialize();
    world_.upgradehp().initialize();
    world_.upgradedeffence().initialize();

    world_.roomcount().initialize();

    world_.isgoal().initialize();
    world_.add_ammo(30);
    fadecolor_ = 0.0f;

    endnow_ = false;

     Ma = { 0.6f };
     Daa = { 0.6f };
     Sa = { 0.6f };
     Ha = { 0.6f };
     Da = { 0.6f };

     MLevel_ = { 1 };
     DaLevel_ = { 1 };
     SLevel_ = { 1 };
     HLevel_ = { 1 };
     DLevel_ = { 1 };

     coincount_ = { 0 };

     Mmn = { 0 };
     Man = { 0 };
     Damn = { 0 };
     Daan = { 0 };
     Smn = { 0 };
     San = { 0 };
     Hmn = { 0 };
     Han = { 0 };
     Dmn = { 0 };
     Dan = { 0 };
   
     gsLoadEffect(Effect_Clew, "Assets/effect/Claw.efkefc");
     gsLoadEffect(Effect_Jump, "Assets/effect/Aura03.efkefc");
     gsLoadEffect(Effect_MissileBoost, "Assets/effect/main_shot.efk");
     gsLoadEffect(Effect_Bone, "Assets/effect/railgun.efk"); 
     gsLoadEffect(Effect_Goal, "Assets/effect/Buff02.efkefc"); 
     gsLoadEffect(Effect_BOBShort, "Assets/effect/Arrow02_Blue.efkefc");

     fadetimer_1 = 60.0f;
     fadetimer_2 = 40.0f;
    
     scoreaddsita = { false };
     Tween::value(0.0f, 1.0f, 120.0f, gsSetVolumeBGM);
}


// 更新
void RoomScene::update(float delta_time) {



  
    if (fadetimer_1 > 0) fadetimer_1 -= delta_time;
    if (fadetimer_2 > 0) fadetimer_2 -= delta_time;


    if (state_ == State::Playing){}
   // gsHideMouseCursor();
    else{}

     //gsShowMouseCursor();


 
    printf("fadecolor %f    \n isgoal %d \n", fadecolor_, world_.isgoal().get());
    if (world_.isgoal().get() == 1) {
        if(fadecolor_ < 1.0f)     fadecolor_ += 0.02f;
         
    }

    if (fadecolor_ >= 1.0f && world_.isgoal().get() == 1) {
        world_.isgoal().sub(4);
    }
    else  if (world_.isgoal().get() == 2) {
        world_.isgoal().sub(3);

        state_ = State::Upgrade;
    }

    if (state_ == State::Playing) {

            

        POINT cursorPos;
        if (GetCursorPos(&cursorPos)) {
            if (cursorPos.x <= 0) {
                gsSetMouseCursorPosition(1080, cursorPos.y);
            }
            if (cursorPos.x >= 1081) {
                gsSetMouseCursorPosition(1, cursorPos.y);
            }
        }
    }



    // 状態別の更新
    switch (state_) {
    case State::Playing: update_playing(delta_time); break;
    case State::Result: update_result(delta_time); break;
    case State::Upgrade: update_upgrade(delta_time); break;

    }
    coincount_ = world_.coin().get();
    roomcount_ = world_.roomcount().get();
    hpint_ = world_.player_hp().getmax();
    ammoint_ = world_.upgrademagazine().get();

    if (state_ == State::Upgrade) {
        world_.stamina().initialize();
    }
    Mmn = world_.upgrademagazine().get();
    Man = world_.upgrademagazine().get() + UPM;
    Damn = world_.upgradedamage().get();
    Daan = world_.upgradedamage().get() + UPDA;
    Smn = world_.upgradeshootspeed().get();
    San = world_.upgradeshootspeed().get() - UPS;
    Hmn = world_.player_hp().getmax();
    Han = world_.player_hp().getmax() +UPH;
    Dmn = world_.upgradedeffence().get();
    Dan = world_.upgradedeffence().get() + UPD;
    

    if (world_.player_hp().get() <= 0) {
        endnow_ = true;
        if (gsGetKeyTrigger(GKEY_SPACE) && scoreaddsita == false){
            scoreaddsita = true;
            result_.add_score(world_.score().get());
            state_ = State::Result;
    
        }
    }
}

// 描画
void RoomScene::draw() const {

    world_.draw();

    if (endnow_ == true && state_ == State::Playing) {
        GSvector2 GAMEOVERPOS = { 0,0 };
        gsDrawSprite2D(Texture_GAMEOVER, &GAMEOVERPOS, NULL, NULL, NULL, NULL, 0.0f);
    }


    static const NumberTexture number{ Texture_Number, 64, 64 };
    GSvector2 maxdrawPos{ -25 + 90,590 };
    number.draw(maxdrawPos, hpint_, 10);

    GSvector2 jmaxdrawPos{ -25 + 90 + 870,590 };
    number.draw(jmaxdrawPos, ammoint_, 10);



    GSvector2 Fadeposition{ 0,0 };
    GScolor4 Fadecolor = { 1.0f,1.0f,1.0f,fadecolor_ };
    gsDrawSprite2D(Texture_Fade, &Fadeposition, NULL, NULL, &Fadecolor, NULL, 0.0f);

    if (state_ == State::Playing) {
        GSvector2 ReticulePos = { (1280 / 2) - 21 / 2, (720 / 2) - 21 / 2 };
        gsDrawSprite2D(Texture_Reticule, &ReticulePos, NULL, NULL, NULL, NULL, 0);
    }

    // リザルト中はリザルトを描画
    if (state_ == State::Result) {
        // リザルトの描画
        result_.draw();
        // 点滅
        if (result_timer_ >= 60.0f && fmod(result_timer_, 40.0f) < 20.0f) {
            static const GSvector2 position{ 250,700 };
            gsDrawSprite2D(Texture_Start, &position, NULL, NULL, NULL, NULL, 0.0f);
        }
    }

    if (state_ == State::Upgrade) {
        static const GSvector2 Mposition{ 0,60 };
        GSrect Mr{ 0,0,1280,190 };
        GSvector2 Ms{ 0.55f,0.55f };
        GScolor4 Mc = { 1.0f,1.0f,1.0f,Ma };
        gsDrawSprite2D(Texture_Upgrade_UI, &Mposition, &Mr, NULL, &Mc, &Ms, 0.0f);

        static const GSvector2 Daposition{ 0,170 };
        GSrect Dar{ 0,0,1280,190 };
        GSvector2 Das{ 0.55f,0.55f };
        GScolor4 Dac{ 1.0f,1.0f,1.0f,Daa };
        gsDrawSprite2D(Texture_Upgrade_UI2, &Daposition, &Dar, NULL, &Dac, &Das, 0.0f);

        static const GSvector2 Sposition{ 0,295 };
        GSrect Sr{ 0,200,1280,380 };
        GSvector2 Ss{ 0.55f,0.55f };
        GScolor4 Sc{ 1.0f,1.0f,1.0f,Sa };
        gsDrawSprite2D(Texture_Upgrade_UI, &Sposition, &Sr, NULL, &Sc, &Ss, 0.0f);

        static const GSvector2 Hposition{ 0,410 };
        GSrect Hr{ 0,380,1280,550 };
        GSvector2 Hs{ 0.55f,0.55f };
        GScolor4 Hc{ 1.0f,1.0f,1.0f,Ha };
        gsDrawSprite2D(Texture_Upgrade_UI, &Hposition, &Hr, NULL, &Hc, &Hs, 0.0f);

        static const GSvector2 Dposition{ 0,525 };
        GSrect Dr{ 0,560,1280,730 };
        GSvector2 Ds{ 0.55f,0.55f };
        GScolor4 Dc{ 1.0f,1.0f,1.0f,Da };
        gsDrawSprite2D(Texture_Upgrade_UI, &Dposition, &Dr, NULL, &Dc, &Ds, 0.0f);

        /// //////////////

        static const NumberTexture number{ Texture_Number, 64, 64 };

        GSvector2 McostPos1 = { 500,95 };
        number.draw(McostPos1, 30 * MLevel_, 7);
        GSvector2 McostPos2 = { 680,95 };
        number.draw(McostPos2, Mmn, 7);
        GSvector2 McostPos3 = { 840,95 };
        number.draw(McostPos3, Man, 7);
        GSvector2 McostPos4 = { 1010,95 };
        number.draw(McostPos4, MLevel_, 7);

        GSvector2 DacostPos1 = { 500,205 };
        number.draw(DacostPos1, 30 * DaLevel_, 7);
        GSvector2 DacostPos2 = { 680,205 };
        number.draw(DacostPos2, Damn, 7);
        GSvector2 DacostPos3 = { 840,205 };
        number.draw(DacostPos3, Daan, 7);
        GSvector2 DacostPos4 = { 1010,205 };
        number.draw(DacostPos4, DaLevel_, 7);

        GSvector2 ScostPos1 = { 500,330 };
        if (SLevel_ >= 30) {}
        else
            number.draw(ScostPos1, 30 * SLevel_, 7);
        GSvector2 ScostPos2 = { 680,330 };
        number.draw(ScostPos2, Smn, 7);
        GSvector2 ScostPos3 = { 840,330 };
        if (SLevel_ >= 30) {}
        else
            number.draw(ScostPos3, San, 7);
        GSvector2 ScostPos4 = { 1010,330 };
        number.draw(ScostPos4, SLevel_, 7);

        GSvector2 HcostPos1 = { 500,445 };
        number.draw(HcostPos1, 30 * HLevel_, 7);
        GSvector2 HcostPos2 = { 680,445 };
        number.draw(HcostPos2, Hmn, 7);
        GSvector2 HcostPos3 = { 840,445 };
        number.draw(HcostPos3, Han, 7);
        GSvector2 HcostPos4 = { 1010,445 };
        number.draw(HcostPos4, HLevel_, 7);

        GSvector2 DcostPos1 = { 500,560 };
        number.draw(DcostPos1, 500 * DLevel_, 7);
        GSvector2 DcostPos2 = { 680,560 };
        number.draw(DcostPos2, Dmn, 7);
        GSvector2 DcostPos3 = { 840,560 };
        number.draw(DcostPos3, Dan, 7);
        GSvector2 DcostPos4 = { 1010,560 };
        number.draw(DcostPos4, DLevel_, 7);

        GSvector2 UpgradeinfoPos = { 0,0 };
        gsDrawSprite2D(Texture_Upgrade_Info, &UpgradeinfoPos, NULL, NULL, NULL, NULL, 0);
    }


    ///GSvector2 ReticulePos = { (1024 / 2) / 2, (768 / 2) / 2 }; 1280,720
    GSvector2 CoinRoomPos = { 0,0 };
    gsDrawSprite2D(Texture_Coin_Room, &CoinRoomPos, NULL, NULL, NULL, NULL, 0);

    //  static const NumberTexture number{ Texture_Number, 64, 64 };
    GSvector2 drawPos{ 250 , 10 };
    number.draw(drawPos, coincount_, 6);
    GSvector2 roomdrawPos{ 1020 , 10 };
    number.draw(roomdrawPos, roomcount_, 6);

    if (fadetimer_1 > 0) {
        const static GSvector2 position5{ 1280.0f / 2, 720.0f / 2 };
        GSvector2 scale5 = { fadetimer_1,fadetimer_1 };
        GSvector2 center5 = { 16,16 };
        GScolor4 color5 = { 1.0f,1.0f,1.0f,1.0 };
        gsDrawSprite2D(Texture_TitleFade, &position5, NULL, &center5, &color5, &scale5, fadetimer_1 * 2);
    }

    if (fadetimer_2 > 0) {
        const static GSvector2 position6{ 1280.0f / 2, 720.0f / 2 };
        GSvector2 scale6 = { fadetimer_2,fadetimer_2 };
        GSvector2 center6 = { 16,16 };
        GScolor4 color6 = { 0.0f,0.0f,0.0f,1.0 };
        gsDrawSprite2D(Texture_TitleFade, &position6, NULL, &center6, &color6, &scale6, fadetimer_1 * 2);
    }


    const static GSvector2 position1{ 1280.0f / 2, 720.0f / 2 };
    GSvector2 scale1 = { fadetimer_,fadetimer_ };
    GSvector2 center1 = { 16,16 };
    GScolor4 color1 = { 1.0f,1.0f,1.0f,0.3f };
    gsDrawSprite2D(Texture_TitleFade, &position1, NULL, &center1, &color1, &scale1, fadetimer_ * 2);

    const static GSvector2 position2{ 1280.0f / 2, 720.0f / 2 };
    GSvector2 scale2 = { fadetimer_2a,fadetimer_2a };
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

}

// 終了しているか？
bool RoomScene::is_end() const {
    return is_end_;
}

// 次のシーンを返す
std::string RoomScene::next() const {
    return "TitleScene";
}

// 終了
void RoomScene::end() {
    // BGMの停止
    gsStopBGM();
    // ランキングファイルの保存
    result_.save("Assets/ranking.txt");
    // ワールドを消去
    world_.clear();
    // アセットの削除
    gsDeleteSkinMesh(Mesh_Player);
    gsDeleteSkinMesh(Mesh_Enemy);
    gsDeleteSkinMesh(Mesh_GoldEnemy);
    gsDeleteSkinMesh(Mesh_EnemyWhiteWolf);
    gsDeleteSkinMesh(Mesh_EnemyRedWolf);
    gsDeleteSkinMesh(Mesh_EnemyFlow);
    gsDeleteSkinMesh(Mesh_SlideDoor);
    gsDeleteSkinMesh(Mesh_Weapon);
    gsDeleteSkinMesh(Mesh_Block);
    gsDeleteSkinMesh(Mesh_Portion);
    gsDeleteSkinMesh(Mesh_Coin);
    gsDeleteSkinMesh(Mesh_Ladder);

    gsDeleteOctree(Octree_Koutei);
    gsDeleteOctree(Octree_KouteiCollider);
    gsDeleteOctree(room_1);
    gsDeleteOctree(room_1_collider);

    gsDeleteTexture(Texture_Skybox);
    gsDeleteTexture(Texture_Title);
    gsDeleteTexture(Texture_Kendo);
    gsDeleteTexture(Texture_Karate);
    gsDeleteTexture(Texture_Number);
    gsDeleteTexture(Texture_Text);
    gsDeleteTexture(Texture_BlueBack);
    gsDeleteTexture(Texture_Result1);
    gsDeleteTexture(Texture_Result2);
    gsDeleteTexture(Texture_Start);
    gsDeleteTexture(Texture_Radar);
    gsDeleteTexture(Texture_RadarPoint);
    gsDeleteTexture(Texture_EffectFlash);
    gsDeleteTexture(Texture_EffectlazerOrange);
    gsDeleteTexture(Texture_Reticule);
    gsDeleteTexture(GaugeWhite);
    gsDeleteTexture(GaugeBlack);
    gsDeleteTexture(HPFlame);
    gsDeleteTexture(Texture_Fade);
    gsDeleteTexture(Texture_Upgrade_UI);
    gsDeleteTexture(Texture_Upgrade_UI2);
    gsDeleteTexture(Texture_Upgrade_Magazine);
    gsDeleteTexture(Texture_Upgrade_Speed);
    gsDeleteTexture(Texture_Upgrade_Hp);
    gsDeleteTexture(Texture_Upgrade_Deffence);
    gsDeleteTexture(Texture_Upgrade_Info);
    gsDeleteTexture(Texture_Coin_Room);
    gsDeleteTexture(Texture_ADS);
    gsDeleteTexture(Texture_GAMEOVER);

    gsDeleteBGM(Sound_PlayingBGM);
    gsDeleteBGM(Sound_TitleBGM);
    gsDeleteBGM(Sound_ResultBGM);


    gsDeleteSE(Se_GameStart);
    gsDeleteSE(Se_PlayerAttack);
    gsDeleteSE(Se_PlayerDamage);
    gsDeleteSE(Se_EnemyDamage);
    gsDeleteSE(Se_Timeout);
    gsDeleteSE(Se_BISI);
    gsDeleteSE(Se_reload);
    gsDeleteSE(Se_EnemyAttack);
    gsDeleteSE(Se_coin);
    gsDeleteSE(Se_heal);
    gsDeleteSE(Se_upgrade);
    gsDeleteSE(Se_EnemyDead);

    gsDeleteEffect(Effect_Clew);
    gsDeleteEffect(Effect_Jump);
    gsDeleteEffect(Effect_MissileBoost);
    gsDeleteEffect(Effect_Bone);
}

// ゲームプレイ中の更新
void RoomScene::update_playing(float delta_time) {

    // ワールドクラスの更新
    world_.update(delta_time);
    // ゲームオーバーになったらリザルト状態に遷移
    if (world_.is_game_over()) {
        // タイムアウト効果音を再生
        gsPlaySE(Se_Timeout);
        // BGMを停止
        gsStopBGM();
        // リザルトシーン用のBGMを再生
        gsPlayBGM(Sound_ResultBGM);
        // 点数を追加
        //result_.add_score(world_.score().get());
        // リザルト中タイマーを初期化
        result_timer_ = 0.0f;
        // リザルト状態に遷移
        state_ = State::Result;
    }
    if (gsGetKeyTrigger(GKEY_TAB)) {
       // world_.upgrademagazine().add(10);
       // world_.upgradeshootspeed().add(-1.0f);
       // state_ = State::Upgrade;
    }

    if (gsGetKeyTrigger(GKEY_1)) {

   
       // world_.player_hp().add(1);
    
    }
    if (gsGetKeyTrigger(GKEY_2) && gsGetKeyState(GKEY_0)) {
        gsPlaySE(Se_QUIZ);
        world_.player_hp().maxup(100);

    }
    if (world_.isgoal().get() == 0) {
        if (fadecolor_ > 0.0f)     fadecolor_ -= 0.02f;

    }
}

// リザルト中の更新
void RoomScene::update_result(float delta_time) {
    // リザルトの更新
    result_.update(delta_time);
    // リザルト中のタイマーの更新
    result_timer_ += delta_time;
    // 最低１秒間はリザルトを表示して、スペースキーを押したらシーン終了
    if (result_timer_ >= 60.0f && gsGetKeyTrigger(GKEY_SPACE)) {
        gsPlaySE(Se_Click);

        Space_ = true;
    }
        if (Space_) {
            fadetimer_ += delta_time;

        }
        if (Space_ && fadetimer_ >= 10.0f) {
            fadetimer_2a += delta_time;
        }
        if (Space_ && fadetimer_2a >= 30.0f) {
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
            fadetimer_2a = 0.0f;
            fadetimer_3 = 0.0f;
            fadetimer_4 = 0.0f;
            fadetimer_5 = 0.0f;
            fadetimer_6 = 0.0f;
            Space_ = false;
        }
    
    //Tween::value(1.0f, 0.0f, 180.0f, gsSetMusicVolume);
}

// アップグレード画面中の更新
void RoomScene::update_upgrade(float delta_time) {


    
    // ワールドクラスの更新
    upgrade_.update(delta_time);

    if (Ma > 1.0f) Ma = 1.0f;
    if (Daa > 1.0f) Daa = 1.0f;
    if (Sa > 1.0f) Sa = 1.0f;
    if (Ha > 1.0f) Ha = 1.0f;
    if (Da > 1.0f) Da = 1.0f;

    if (gsGetKeyTrigger(GKEY_3) && gsGetKeyState(GKEY_0)) {
        gsPlaySE(Se_QUIZ);
        world_.coin().add(10000);
    }


    POINT cursorPos;
    if (GetCursorPos(&cursorPos)) {
        if (cursorPos.y >= 620 && cursorPos.y <= 720 && cursorPos.x >= 850 ) {
            if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
                gsPlaySE(Se_Click);

                world_.isgoal().sub(0);
                create_room();
                state_ = State::Playing;
            }
        }


        if (cursorPos.y >= 60 && cursorPos.y <= 60+90 && world_.coin().get() >= 30 * MLevel_) {
            Ma += 0.05f;
            if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
                world_.upgrademagazine().add(UPM);
                world_.coin().sub(30 * MLevel_);
                MLevel_ += 1;
                gsPlaySE(Se_upgrade);
            }
        } 
        else  if (Ma > 0.6f) Ma -= 0.05f;
     

        if (cursorPos.y >= 170 && cursorPos.y <= 170+90 && world_.coin().get() >= 30 * DaLevel_) {
            Daa += 0.05f;
            if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
                world_.upgradedamage().add(UPDA);
                world_.coin().sub(30 * DaLevel_);
                DaLevel_ += 1;
                gsPlaySE(Se_upgrade);
            }
        }
        else  if (Daa > 0.6f) Daa -= 0.05f;
        if (cursorPos.y >= 295 && cursorPos.y <= 295+90 && world_.coin().get() >= 30 * SLevel_ && SLevel_ <= 29) {
            Sa += 0.05f;
            if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
                world_.upgradeshootspeed().add(-UPS);
                world_.coin().sub(30 * SLevel_);
                SLevel_ += 1;
                gsPlaySE(Se_upgrade);
            }
        }
        else  if (Sa > 0.6f) Sa -= 0.05f;
        if (cursorPos.y >= 410 && cursorPos.y <= 410+ 90 && world_.coin().get() >= 30 * HLevel_) {
            Ha += 0.05f;
            if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
                world_.player_hp().maxup(UPH);
                world_.coin().sub(30 * HLevel_);
                HLevel_ += 1;
                gsPlaySE(Se_upgrade);
            }
        }
        else  if (Ha > 0.6f) Ha -= 0.05f;
        if (cursorPos.y >= 525 && cursorPos.y <= 525+90 && world_.coin().get() >= 500 * DLevel_ ) {
            Da += 0.05f;
            if (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1)) {
                world_.upgradedeffence().add(UPD);
                world_.coin().sub(500 * DLevel_);
                DLevel_ += 1;
                gsPlaySE(Se_upgrade);
            }
        }
        else  if (Da > 0.6f) Da -= 0.05f;
    }


    if (gsGetKeyTrigger(GKEY_SPACE)) {
        gsPlaySE(Se_Click);

        world_.isgoal().sub(0); 
        create_room();
        state_ = State::Playing;
    }
    //Tween::value(1.0f, 0.0f, 120.0f, gsSetVolumeBGM);
  
}




// ルームを作成
void RoomScene::create_room() {


    /// 配列を初期化！
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            for (int z = 0; z < 20; z++) {
                blockdata_[x][y][z] = 0;
            }
        }
    }

    /// ルームのスタート地点座標！
    /// とあなあけマス数の定義！
    data_x_ = 3;
    data_y_ = 10;
    data_z_ = 3;
    int onoff = gsRand(0, 5);

    /// あなあけ回数ぶんループするよっ！
    int loop = 27 + world_.roomcount().get();
    if (loop >= 15) loop = 15;
    for (int i = 0; i < loop; i++) {
        int randroomtype = gsRand(1, 10);
        if (world_.roomcount().get() < 5) randroomtype = gsRand(6, 10);
        else  if (world_.roomcount().get() < 8) randroomtype = gsRand(3, 10);
        if (i == 0 || i == loop) randroomtype = 10;
       // world_.field()->add(new TrapBlock{ &world_,GSvector3{ data_x_ * 7.5f, (-1 + data_y_) * 3.0f, data_z_ * 7.5f }, Mesh_BlockMove, Mesh_BlockMove });
        
        if (randroomtype == 3 || randroomtype == 4)
        {
            world_.field()->add(new MoveBlock{ &world_,GSvector3{ data_x_ * 7.5f, (-1 + data_y_) * 3.0f, data_z_ * 7.5f }, Mesh_BlockMove, Mesh_BlockMove });
            world_.field()->add(new MoveBlock{ &world_,GSvector3{ data_x_ * 7.5f,  (-1 + data_y_) * 3.0f, (1 + data_z_) * 7.5f }, Mesh_BlockMove, Mesh_BlockMove });

            /// １段目だよっ！
            blockdata_[data_x_][data_y_][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_][data_z_] = 1;
            blockdata_[data_x_][data_y_][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_][data_z_ + 1] = 1;
            /// 2段目だよ～！
            blockdata_[data_x_][data_y_ + 1][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_ + 1][data_z_] = 1;
            blockdata_[data_x_][data_y_ + 1][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_ + 1][data_z_ + 1] = 1;
            /// ３段モードっ！
            blockdata_[data_x_][data_y_ + 2][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_ + 2][data_z_] = 1;
            blockdata_[data_x_][data_y_ + 2][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_ + 2][data_z_ + 1] = 1;
            /// した2段目だよ～！
            blockdata_[data_x_][data_y_ - 1][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_ - 1][data_z_] = 1;
            blockdata_[data_x_][data_y_ - 1][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_ - 1][data_z_ + 1] = 1;
            /// した３段モードっ！
            blockdata_[data_x_][data_y_ - 2][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_ - 2][data_z_] = 1;
            blockdata_[data_x_][data_y_ - 2][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_ - 2][data_z_ + 1] = 1;
            /// した4段モードっ！
            blockdata_[data_x_][data_y_ - 3][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_ - 3][data_z_] = 1;
            blockdata_[data_x_][data_y_ - 3][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_ - 3][data_z_ + 1] = 1;
        }
        else if (randroomtype == 1 || randroomtype == 2)
        {
            world_.field()->add(new TrapBlock{ &world_,GSvector3{ data_x_ * 7.5f, ( data_y_) * 3.0f, data_z_ * 7.5f }, Mesh_Trap_Default, Mesh_Trap_Default });
            world_.field()->add(new TrapBlock{ &world_,GSvector3{ data_x_ * 7.5f,  ( data_y_) * 3.0f, (1 + data_z_) * 7.5f }, Mesh_Trap_Default, Mesh_Trap_Default });
            world_.field()->add(new TrapBlock{ &world_,GSvector3{ (1 +data_x_) * 7.5f, (data_y_) * 3.0f, data_z_ * 7.5f }, Mesh_Trap_Default, Mesh_Trap_Default });
            world_.field()->add(new TrapBlock{ &world_,GSvector3{ (1 +data_x_) * 7.5f,  (data_y_) * 3.0f, (1 + data_z_) * 7.5f }, Mesh_Trap_Default, Mesh_Trap_Default });

            ///開始地点からOミノを倒した感じであなあけパ～ンチ！
            /// １段目だよっ！
            blockdata_[data_x_][data_y_][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_][data_z_] = 1;
            blockdata_[data_x_][data_y_][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_][data_z_ + 1] = 1;
            /// 2段目だよ～！
            blockdata_[data_x_][data_y_ + 1][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_ + 1][data_z_] = 1;
            blockdata_[data_x_][data_y_ + 1][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_ + 1][data_z_ + 1] = 1;
            /// ３段モードっ！
            blockdata_[data_x_][data_y_ + 2][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_ + 2][data_z_] = 1;
            blockdata_[data_x_][data_y_ + 2][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_ + 2][data_z_ + 1] = 1;
        }
        else
        {
            ///開始地点からOミノを倒した感じであなあけパ～ンチ！
            /// １段目だよっ！
            blockdata_[data_x_][data_y_][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_][data_z_] = 1;
            blockdata_[data_x_][data_y_][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_][data_z_ + 1] = 1;
            /// 2段目だよ～！
            blockdata_[data_x_][data_y_ + 1][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_ + 1][data_z_] = 1;
            blockdata_[data_x_][data_y_ + 1][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_ + 1][data_z_ + 1] = 1;
            /// ３段モードっ！
            blockdata_[data_x_][data_y_ + 2][data_z_] = 1;
            blockdata_[data_x_ + 1][data_y_ + 2][data_z_] = 1;
            blockdata_[data_x_][data_y_ + 2][data_z_ + 1] = 1;
            blockdata_[data_x_ + 1][data_y_ + 2][data_z_ + 1] = 1;
        }

        if (i == loop - 1) {
            world_.add_actor(new Goal{ &world_,GSvector3{ 1 + (float)data_x_ * 7.5f, (float)data_y_ * 3 , 1 + (float)data_z_ * 7.5f } });
        }

        bool spownenable = gsRand(0, 3);
        if (spownenable == 1 && i  >= 2 && i != loop - 1) {
            world_.add_actor(new EnemySpowner{ &world_,GSvector3{ (float)data_x_ * 7.5f, (float)data_y_ * 3 , (float)data_z_ * 7.5f } });
        }


        /// 穴あけ方向をランダムできめましょう！
        onoff = gsRand(0, 8);
        if (onoff == 1 || onoff == 2 || onoff == 3) {
            data_x_ += gsRand(1, 2); nextYok = true;
        }
        if (onoff == 4 || onoff == 5 || onoff == 6) { data_z_ += gsRand(1, 2); nextYok = true; }
        if (onoff == 7 && nextYok || onoff == 8 && nextYok) {
            data_y_ += gsRand(-1, 2);
        nextYok = false;
        }

        if (data_y_ >= 16)  data_y_ = 16;
        if (data_y_ <= 4)  data_y_ = 4;
        if (data_x_ >= 16)  data_x_ = 16;
        if (data_x_ <= 4)  data_x_ = 4;
        if (data_z_ >= 16)  data_z_ = 16;
        if (data_z_ <= 4)  data_z_ = 4;

    }

    /// ブロックを配置するよっ！
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            for (int z = 0; z < 20; z++) {

                if (blockdata_[x][y][z] == 0) {
                    /// まわりがかこまれていたら・・・！
                    if (x > 0 && x < 20 - 1 &&
                        y > 0 && y < 20 - 1 &&
                        z > 0 && z < 20 - 1) {

                        if (blockdata_[x + 1][y][z] == 0 &&
                            blockdata_[x - 1][y][z] == 0 &&
                            blockdata_[x][y + 1][z] == 0 &&
                            blockdata_[x][y - 1][z] == 0 &&
                            blockdata_[x][y][z + 1] == 0 &&
                            blockdata_[x][y][z - 1] == 0) {

                            // 安全に処理ができるっ！
                        }

                        else if (x == 0) {

                        }
                        else {
                            world_.field()->add(new Block{ &world_,GSvector3{ (float)x * 7.5f, (float)y * 3, (float)z * 7.5f }, Mesh_Block, Mesh_Block });

                        }
                    }
                }
            }
        }

        //world_.field()->add(new Goal{ &world_,GSvector3{ (float)data_x_ * 7.5f, (float)data_y_ * 3 , (float)data_z_ * 7.5f }, Mesh_EnemyWhiteWolf, Mesh_EnemyWhiteWolf });
    }
    ///
            /*
                        blockdata_[x][y][z] = 1;
                        blockdata_[x + 1][y][z] = 1;
                        blockdata_[x][y + 1][z] = 1;
                        blockdata_[x][y][z + 1] = 1;
                        blockdata_[x + 1][y + 1][z] = 1;
                        blockdata_[x][y + 1][z + 1] = 1;
                        blockdata_[x + 1][y][z + 1] = 1;
                        blockdata_[x + 1][y + 1][z + 1] = 1;
                        blockdata_[x][y + 2][z] = 1;
                        blockdata_[x + 1][y + 2][z] = 1;
                        blockdata_[x][y + 2][z + 1] = 1;
                        blockdata_[x + 1][y + 2][z + 1] = 1;
                        */
}
