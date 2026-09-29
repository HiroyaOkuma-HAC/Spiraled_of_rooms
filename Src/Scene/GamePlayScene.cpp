#include "Scene/GamePlayScene.h"
#include "Actor/Player.h"
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

// 開始
void GamePlayScene::start() {
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


    gsLoadEffect(Effect_Clew, "Assets/effect/Claw.efkefc");




    // ライトマップの読み込み
    gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");
    // リフレクションプローブの読み込み
    gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");
    // 視錐台カリングを有効にする
    gsEnable(GS_FRUSTUM_CULLING);

    // 剣道部長メッシュの読み込み
    //gsLoadSkinMesh(Mesh_Player, "Assets/model/Kendo/kendo.mshb");
    gsLoadSkinMesh(Mesh_Player, "Assets/model/Player/player.mshb");

    // 敵メッシュの読み込み
    gsLoadSkinMesh(Mesh_Enemy, "Assets/model/Karate/karate.mshb");
    // 敵メッシュの読み込み
    gsLoadSkinMesh(Mesh_Enemy, "Assets/model/Enemy/Enemy.mshb");
    gsLoadSkinMesh(Mesh_EnemyWhiteWolf, "Assets/model/Enemy_White_Wolf/Enemy.mshb");

    
    
    // 金敵メッシュの読み込み
    gsLoadSkinMesh(Mesh_GoldEnemy, "Assets/model/GoldKarate/karate.mshb");
    // 金網メッシュの読み込み
    gsLoadSkinMesh(Mesh_SlideDoor, "Assets/model/SlideDoor/slidedoor.mshb");
    // 武器メッシュの読み込み
    gsLoadMesh(Mesh_Weapon, "Assets/model/Weapon/w_magun01.msh");
   

    // スカイボックス用テクスチャの読み込み
    gsLoadTexture(Texture_Skybox, "Assets/texture/skydome.png");
    // 校庭オクトリーの読み込み
    gsLoadOctree(Octree_Koutei, "Assets/model/Koutei/cybercity.oct");
    // 校庭衝突判定用オクトリーの読み込み
    gsLoadOctree(Octree_KouteiCollider, "Assets/model/Koutei/cybercity_collider.oct");

    // オクトリーの読み込み
    gsLoadOctree(room_1, "Assets/model/Koutei/room_1.oct");
    // 衝突判定用オクトリーの読み込み
    gsLoadOctree(room_1_collider, "Assets/model/Koutei/room_1_collider.oct");




    // 数値用テクスチャの読み込み
    gsLoadTexture(Texture_Number, "Assets/texture/NUM.png");
    // 文字用テクスチャの読み込み
    gsLoadTexture(Texture_Text, "Assets/texture/text.png");
    // リザルト中の背景の読み込み
    gsLoadTexture(Texture_BlueBack, "Assets/texture/blue.png");
    // 評価タイトルの読み込み
    gsLoadTexture(Texture_Result1, "Assets/texture/result1.png");
    // 評価文章 の読み込み
    gsLoadTexture(Texture_Result2, "Assets/texture/result2.png");
    // 剣道部長画像の読み込み
    gsLoadTexture(Texture_Kendo, "Assets/texture/mini_kendo.png");
    // 開始ボタンを押忍の読み込み
    gsLoadTexture(Texture_Start, "Assets/texture/osu.png");

    // レーダーの背景の画像を読み込み
    gsLoadTexture(Texture_Radar, "Assets/texture/radar.png");
    // レーダーの点の画像を読み込み
    gsLoadTexture(Texture_RadarPoint, "Assets/texture/pt.png");
    // エフェクトの火花の画像を読み込み
    gsLoadTexture(Texture_EffectlazerOrange, "Assets/texture/pt.png");
    //  エフェクトの爆発の画像を読み込み
    gsLoadTexture(Texture_EffectFlash, "Assets/texture/part_spark_large_dff.png");
    // 

    gsLoadTexture(Texture_Reticule, "Assets/texture/Reticule.png");

    gsLoadTexture(GaugeWhite, "Assets/texture/grp1.png");
    gsLoadTexture(GaugeBlack, "Assets/texture/grp2.png");
    gsLoadTexture(HPFlame, "Assets/texture/HPandStuminaUI.png");

    gsLoadSkinMesh(Mesh_EnemyRedWolf, "Assets/model/Enemy_Red_Wolf/Enemy3.mshb");




    // ゲーム開始時の効果音の読み込み
    gsLoadSE(Se_GameStart, "Assets/sound/Select.wav", 1, GWAVE_DEFAULT);
    // 剣道部長攻撃時効果音の読み込み
    gsLoadSE(Se_PlayerAttack, "Assets/sound/Attack1.wav", 1, GWAVE_DEFAULT);
    // 剣道部長ダメージ効果音の読み込み
    gsLoadSE(Se_PlayerDamage, "Assets/sound/Damage2.wav", 1, GWAVE_DEFAULT);
    // 空手部長ダメージ効果音の読み込み
    gsLoadSE(Se_EnemyDamage, "Assets/sound/Attack2.wav", 1, GWAVE_DEFAULT);
    // タイムアウトの効果音の読み込み
    gsLoadSE(Se_Timeout, "Assets/sound/timeend.wav", 1, GWAVE_DEFAULT);
        // タイムアウトの効果音の読み込み
    gsLoadSE(Se_BISI, "Assets/sound/hit.wav", 1, GWAVE_DEFAULT);

    // ゲームプレイ中用BGMの読み込み
    gsLoadBGM(Sound_PlayingBGM, "Assets/sound/kendo.ogg", GS_TRUE);
    // リザルト用BGMの読み込み
    gsLoadBGM(Sound_ResultBGM, "Assets/sound/ed.ogg", GS_TRUE);

    // BGMの再生
    gsPlayBGM(Sound_PlayingBGM);
    // 開始効果音（いくぜ！の掛け声）
    //gsPlaySE(Se_GameStart);

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
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    // 敵生成クラスを追加
    world_.add_actor(new EnemyGenerator{ &world_ });
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
    world_.field()->add(new MovingFloor{ &world_,GSvector3{ 29.0f, 0.0, -9.39f }, Mesh_SlideDoor, Mesh_SlideDoor ,0 ,0});

    // コインの初期化！
    world_.coin().initialize();

    gsSetMouseCursorPosition(1280 /2, 720/2);

    world_.upgrademagazine().initialize();
    world_.upgradedamage().initialize();
    world_.upgradeshootspeed().initialize();
    world_.upgradehp().initialize();
    world_.upgradedeffence().initialize();

}


// 更新
void GamePlayScene::update(float delta_time) {
    POINT cursorPos;
    if (GetCursorPos(&cursorPos)) {
        if (cursorPos.x <= 0) {
            gsSetMouseCursorPosition(1080, cursorPos.y);
        }
        if (cursorPos.x >= 1081) {
            gsSetMouseCursorPosition(1, cursorPos.y);
        }
    }

    if (gsGetKeyTrigger(GKEY_Q)) {
        // 敵を追加
        world_.add_actor(new GoldKarate{ &world_, GSvector3{ 0.0f, 0.0f, 20.0f } });
          // 敵を追加
        //world_.add_actor(new Enemy{ &world_, GSvector3{ 0.0f, 0.0f, 20.0f } });
        //world_.add_field(new Field{ room_1, room_1_collider, Texture_Skybox });
    }

    // 状態別の更新
    switch (state_) {
    case State::Playing: update_playing(delta_time); break;
    case State::Result: update_result(delta_time); break;
    case State::Upgrade: update_upgrade(delta_time); break;

    }
}

// 描画
void GamePlayScene::draw() const {

    world_.draw();
  
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

    GSvector2 Tokuten_drawPos{ -120 + 1024 / 2, 20 };
    GSrect Tokuten_rect{ 0,64,128,128 };
    gsDrawSprite2D(Texture_Text, &Tokuten_drawPos, &Tokuten_rect, NULL, NULL, NULL, 0);

    ///GSvector2 ReticulePos = { (1024 / 2) / 2, (768 / 2) / 2 };
    GSvector2 ReticulePos = { 1024 / 2, 768 / 2 };
    gsDrawSprite2D(Texture_Reticule, &ReticulePos, NULL, NULL, NULL, NULL, 0);

}

// 終了しているか？
bool GamePlayScene::is_end() const {
    return is_end_;
}

// 次のシーンを返す
std::string GamePlayScene::next() const {
    return "TitleScene";
}

// 終了
void GamePlayScene::end() {
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
    gsDeleteOctree(Octree_Koutei);
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
    gsDeleteBGM(Sound_PlayingBGM);
    gsDeleteBGM(Sound_ResultBGM);
    gsDeleteSE(Se_GameStart);
    gsDeleteSE(Se_PlayerAttack);
    gsDeleteSE(Se_PlayerDamage);
    gsDeleteSE(Se_EnemyDamage);
    gsDeleteSE(Se_Timeout);
    gsDeleteSE(Se_BISI);
}

// ゲームプレイ中の更新
void GamePlayScene::update_playing(float delta_time) {
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
        result_.add_score(world_.score().get());
        // リザルト中タイマーを初期化
        result_timer_ = 0.0f;
        // リザルト状態に遷移
        state_ = State::Result;
    }
    if (gsGetKeyTrigger(GKEY_TAB)) {
        world_.upgrademagazine().add(10);
        world_.upgradeshootspeed().add(-1.0f);
        state_ = State::Upgrade;
    }
}

// リザルト中の更新
void GamePlayScene::update_result(float delta_time) {
    // リザルトの更新
    result_.update(delta_time);
    // リザルト中のタイマーの更新
    result_timer_ += delta_time;
    // 最低１秒間はリザルトを表示して、スペースキーを押したらシーン終了
    if (result_timer_ >= 60.0f && gsGetKeyTrigger(GKEY_SPACE)) {
        is_end_ = true;
    }
}

// アップグレード画面中の更新
void GamePlayScene::update_upgrade(float delta_time) {
    // ワールドクラスの更新
    upgrade_.update(delta_time);
    if (gsGetKeyTrigger(GKEY_TAB)) {
        state_ = State::Playing;
    }
}
