#include "LoadTutrial.h"
#include <GSgame.h>
#include "Assets.h"
#include <GSstandard_shader.h>
#include "Rendering/GaugeTexture.h"

// ロード対象のアセット数
const int TOTAL_ASSET_NUM{ 78 };
float AcolorT{ 0.0f };

void LoadTutrial::start() {

    // カーソルを消す
    //gsHideMouseCursor();
    is_end_ = false;
    loaded_count = 0;
    // ロードを別スレッドで開始
    gslib::Game::run_thread([=] {load(); });
}

void LoadTutrial::update(float delta_time) {
}

void LoadTutrial::draw() const {

    const static GSvector2 position_start{ 0,0 };
    GScolor color_start{ 1.0f,1.0f,1.0f,AcolorT };
    gsDrawSprite2D(Texture_loading, &position_start, NULL, NULL, &color_start, NULL, 0.0f);
    if (loaded_count <= 59 && AcolorT < 1.0f) AcolorT += 0.005f;
    if (loaded_count >= 70) AcolorT -= 0.05f;

    //gsDrawText("ロード状況：%d/%d", loaded_count, TOTAL_ASSET_NUM);
    // 16, 16 は読み込んだ画像のサイズ
    static const GaugeTexture gauge{ GaugeWhite, GaugeBlack, 16, 16 };
    // ゲージのカラー（半透明）
    static const GScolor gauge_color{ 1.0f, 1.0f, 1.0f, 0.8f };
    // ゲージの背景カラー（半透明）
    static const GScolor background_color{ 1.0f, 1.0f, 1.0f, 0.8f };

    // (200, 2) の位置に、(100, 16)のサイズのゲージを表示（この位置とサイズは適当です）。
    // ゲージの数値は現在のHP、ゲージ最大値は最大HP。
    gauge.draw(GSvector2{ 0, 710 }, 1280, 10, loaded_count + 16, TOTAL_ASSET_NUM + 16, gauge_color, background_color);

}

bool LoadTutrial::is_end() const {
    return is_end_;
}

std::string LoadTutrial::next() const {
    return "Tutrial"; 
}

void LoadTutrial::end() {
}

void LoadTutrial::load() {

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


    // 校庭オクトリーの読み込み
    gsLoadOctree(Octree_Koutei, "Assets/model/Koutei/kouteiOG.oct"); ++loaded_count;
    // 校庭衝突判定用オクトリーの読み込み
    gsLoadOctree(Octree_KouteiCollider, "Assets/model/Koutei/koutei_colliderOG.oct"); ++loaded_count;

    // オクトリーの読み込み
    //gsLoadOctree(room_1, "Assets/model/Koutei/room_1.oct");
    // 衝突判定用オクトリーの読み込み
    //gsLoadOctree(room_1_collider, "Assets/model/Koutei/room_1_collider.oct");




    // 数値用テクスチャの読み込み
    gsLoadTexture(Texture_Number, "Assets/texture/NUM.png"); ++loaded_count;
    // 文字用テクスチャの読み込み
    gsLoadTexture(Texture_Text, "Assets/texture/text.png"); ++loaded_count;
    // リザルト中の背景の読み込み
    gsLoadTexture(Texture_BlueBack, "Assets/texture/blue.png"); ++loaded_count;
    // 評価タイトルの読み込み
    gsLoadTexture(Texture_Result1, "Assets/texture/result1.png"); ++loaded_count;
    // 評価文章 の読み込み
    gsLoadTexture(Texture_Result2, "Assets/texture/result2.png"); ++loaded_count;
    // 剣道部長画像の読み込み
    gsLoadTexture(Texture_Kendo, "Assets/texture/mini_kendo.png"); ++loaded_count;
    // 開始ボタンを押忍の読み込み
    gsLoadTexture(Texture_Start, "Assets/texture/osu.png"); ++loaded_count;

    // レーダーの背景の画像を読み込み
    gsLoadTexture(Texture_Radar, "Assets/texture/radar.png"); ++loaded_count;
    // レーダーの点の画像を読み込み
    gsLoadTexture(Texture_RadarPoint, "Assets/texture/pt.png"); ++loaded_count;
    // エフェクトの火花の画像を読み込み
    gsLoadTexture(Texture_EffectlazerOrange, "Assets/texture/pt.png"); ++loaded_count;
    //  エフェクトの爆発の画像を読み込み
    gsLoadTexture(Texture_EffectFlash, "Assets/texture/part_spark_large_dff.png"); ++loaded_count;
    // 

    gsLoadTexture(Texture_Reticule, "Assets/texture/Reticule.png"); ++loaded_count;

    gsLoadTexture(GaugeWhite, "Assets/texture/grp1.png"); ++loaded_count;
    gsLoadTexture(GaugeBlack, "Assets/texture/grp2.png"); ++loaded_count;
    gsLoadTexture(HPFlame, "Assets/texture/HPandStuminaUI.png"); ++loaded_count;
    gsLoadTexture(Texture_Fade, "Assets/texture/fade.png"); ++loaded_count;
    gsLoadTexture(Texture_Upgrade_UI, "Assets/texture/Upgrade_UI.png"); ++loaded_count;
    gsLoadTexture(Texture_Upgrade_UI2, "Assets/texture/Upgrade_UI2.png"); ++loaded_count;
    gsLoadTexture(Texture_Upgrade_Magazine, "Assets/texture/UPma.jpg"); ++loaded_count;
    gsLoadTexture(Texture_Upgrade_Speed, "Assets/texture/UIsp.jpg"); ++loaded_count;
    gsLoadTexture(Texture_Upgrade_Hp, "Assets/texture/UPhp.jpg"); ++loaded_count;
    gsLoadTexture(Texture_Upgrade_Deffence, "Assets/texture/UPde.jpg"); ++loaded_count;
    gsLoadTexture(Texture_Upgrade_Info, "Assets/texture/upgrade_info.png"); ++loaded_count;
    gsLoadTexture(Texture_Coin_Room, "Assets/texture/coin_room.png"); ++loaded_count;

    gsLoadTexture(Texture_ADS, "Assets/texture/ADS.png"); ++loaded_count;

    gsLoadTexture(Texture_GAMEOVER, "Assets/texture/GAMEOVER.png"); ++loaded_count;
    gsLoadTexture(Texture_TitleFade, "Assets/texture/TitleFade.png"); ++loaded_count;
    gsLoadTexture(Texture_Damage, "Assets/texture/Damage.png"); ++loaded_count;


    gsLoadTexture(Texture_Tu0, "Assets/texture/Ty_0.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu1, "Assets/texture/Ty_1.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu2, "Assets/texture/Ty_2.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu3, "Assets/texture/Ty_3.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu4, "Assets/texture/Ty_4.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu5, "Assets/texture/Ty_5.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu6, "Assets/texture/Ty_6.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu7, "Assets/texture/Ty_7.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu8, "Assets/texture/Ty_8.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu9, "Assets/texture/Ty_9.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu10, "Assets/texture/Ty_10.png"); ++loaded_count;
    gsLoadTexture(Texture_Tu11, "Assets/texture/Ty_11.png"); ++loaded_count;





    // ゲーム開始時の効果音の読み込み
    gsLoadSE(Se_GameStart, "Assets/sound/Select.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    // 剣道部長攻撃時効果音の読み込み
    gsLoadSE(Se_PlayerAttack, "Assets/sound/Attack1.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    // 剣道部長ダメージ効果音の読み込み
    gsLoadSE(Se_PlayerDamage, "Assets/sound/Damage2.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    // 空手部長ダメージ効果音の読み込み
    gsLoadSE(Se_EnemyDamage, "Assets/sound/hit.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    // タイムアウトの効果音の読み込み
    gsLoadSE(Se_Timeout, "Assets/sound/timeend.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    // タイムアウトの効果音の読み込み
    gsLoadSE(Se_BISI, "Assets/sound/hit.wav", 1, GWAVE_DEFAULT); ++loaded_count;

    gsLoadSE(Se_reload, "Assets/sound/reload.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    gsLoadSE(Se_EnemyAttack, "Assets/sound/damage3.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    gsLoadSE(Se_coin, "Assets/sound/get_gold.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    gsLoadSE(Se_heal, "Assets/sound/get_dya.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    gsLoadSE(Se_upgrade, "Assets/sound/heal.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    gsLoadSE(Se_EnemyDead, "Assets/sound/damage5.wav", 1, GWAVE_DEFAULT); ++loaded_count;
    gsLoadSE(Se_EnemyDamage, "Assets/sound/hit.wav", 1, GWAVE_DEFAULT); ++loaded_count;


    // ゲームプレイ中用BGMの読み込み
    gsLoadBGM(Sound_PlayingBGM, "Assets/sound/RoomBGM.ogg", GS_TRUE); ++loaded_count;
    // リザルト用BGMの読み込み
    gsLoadBGM(Sound_ResultBGM, "Assets/sound/ed.ogg", GS_TRUE); ++loaded_count;

    // ライトマップの読み込み
    gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt"); ++loaded_count;
    // リフレクションプローブの読み込み
    gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt"); ++loaded_count;
    // 視錐台カリングを有効にする
    gsEnable(GS_FRUSTUM_CULLING);

    // 剣道部長メッシュの読み込み
    //gsLoadSkinMesh(Mesh_Player, "Assets/model/Kendo/kendo.mshb");
    gsLoadSkinMesh(Mesh_Player, "Assets/model/Player/player.mshb"); ++loaded_count;

    // 敵メッシュの読み込み
    gsLoadSkinMesh(Mesh_Enemy, "Assets/model/Karate/karate.mshb"); ++loaded_count;
    // 敵メッシュの読み込み
    gsLoadSkinMesh(Mesh_Enemy, "Assets/model/Enemy/Enemy.mshb"); ++loaded_count;
    gsLoadSkinMesh(Mesh_EnemyWhiteWolf, "Assets/model/Enemy_White_Wolf/Enemy.mshb"); ++loaded_count;
    gsLoadSkinMesh(Mesh_EnemyRedWolf, "Assets/model/Enemy_Red_Wolf/Enemy3.mshb"); ++loaded_count;
    gsLoadSkinMesh(Mesh_EnemyFlow, "Assets/model/Enemy_flow/Enemy.mshb"); ++loaded_count;
    gsLoadSkinMesh(Mesh_Enemy_reaper, "Assets/model/Enemy_reaper/Enemy_reaper.mshb"); ++loaded_count;



    // 金敵メッシュの読み込み
    gsLoadSkinMesh(Mesh_GoldEnemy, "Assets/model/GoldKarate/karate.mshb"); ++loaded_count;
    // 金網メッシュの読み込み
    gsLoadSkinMesh(Mesh_SlideDoor, "Assets/model/SlideDoor/slidedoor.mshb"); ++loaded_count;
    // 武器メッシュの読み込み
    gsLoadMesh(Mesh_Weapon, "Assets/model/Weapon/w_magun01.msh"); ++loaded_count;
    gsLoadSkinMesh(Mesh_Block, "Assets/model/Block/block3.mshb"); ++loaded_count;
    gsLoadSkinMesh(Mesh_BlockMove, "Assets/model/BlockMove/BlockMove.mshb"); ++loaded_count;
    gsLoadSkinMesh(Mesh_Trap_Default, "Assets/model/Trap/Trap_Default/Trap_Default.mshb"); ++loaded_count;
    gsLoadSkinMesh(Mesh_Trap_Ready, "Assets/model/Trap/Trap_Ready/Trap_Ready.mshb"); ++loaded_count;
    gsLoadSkinMesh(Mesh_Trap_Stage1, "Assets/model/Trap/Trap_Stage1/Trap_Stage1.mshb"); ++loaded_count;
    gsLoadSkinMesh(Mesh_Trap_Stage2, "Assets/model/Trap/Trap_Stage2/Trap_Stage2.mshb"); ++loaded_count;


    gsLoadMesh(Mesh_Coin, "Assets/model/Coin/coin.mshb"); ++loaded_count;
    gsLoadMesh(Mesh_Portion, "Assets/model/Portion/Portion.mshb"); ++loaded_count;
    gsLoadMesh(Mesh_Ladder, "Assets/model/Ladder/Ladder.mshb"); ++loaded_count;

    gsLoadMesh(Mesh_BoneAttack, "Assets/model/BoneAttack/BoneAttack.mshb"); ++loaded_count;


    // スカイボックス用テクスチャの読み込み
    gsLoadTexture(Texture_Skybox, "Assets/texture/skydome.png"); ++loaded_count;


    // シーン終了フラグON
    is_end_ = true;
}
