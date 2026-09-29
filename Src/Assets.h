#ifndef ASSETS_H_
#define ASSETS_H_

enum {                      // メッシュデータ
    Mesh_Player,            // プレーヤ
    Mesh_Enemy,             // 敵
    Mesh_EnemyWhiteWolf,
    Mesh_EnemyRedWolf,
    Mesh_EnemyFlow,
    Mesh_Enemy_reaper,
    Mesh_Enemy_Gold,
    Mesh_GoldEnemy,
    Mesh_SlideDoor,     // かな網
    Mesh_Weapon, // 武器
    Mesh_Block,
    Mesh_BlockMove,
    Mesh_Portion,
    Mesh_Coin,
    Mesh_Ladder,
    Mesh_BoneAttack,

    Mesh_Trap_Default,
    Mesh_Trap_Ready,
    Mesh_Trap_Stage1,
    Mesh_Trap_Stage2,

    Mesh_PlayerBOB,
};

enum {                      // オクトリーデータ
    Octree_Koutei,          // 校庭
    Octree_KouteiCollider,   // 校庭衝突判定

    room_1,
    room_1_collider,
};

enum {                      // テクスチャデータ
    Texture_Skybox,         // スカイボックス
    Texture_Title,          // タイトル
    Texture_Kendo,          // 剣道部長
    Texture_Karate,         // 空手部長
    Texture_TitleFade,
    Texture_Damage,
    Texture_Start,          // 開始メッセージ
    Texture_Number,         // 数字フォント
    Texture_Text,           // テキスト画像
    Texture_BlueBack,       // リザルト用背景
    Texture_Result1,        // リザルト用テキスト1
    Texture_Result2,        // リザルト用テキスト2
    Texture_Radar,          // レーダー画像
    Texture_RadarPoint,      // レーダーの点

    Texture_EffectFlash,
    Texture_EffectlazerOrange,

    Texture_Reticule,

    GaugeWhite,
    GaugeBlack,
    HPFlame,

    Texture_Fade,
    Texture_Upgrade_UI,
    Texture_Upgrade_UI2,
    Texture_Upgrade_Magazine,
    Texture_Upgrade_Speed,
    Texture_Upgrade_Hp,
    Texture_Upgrade_Deffence,
    Texture_Upgrade_Info,
    Texture_Coin_Room,
    Texture_ADS,
    Texture_BOBADS,
    Texture_GAMEOVER,
    Texture_Titlelogo,
    Texture_loading,
    Texture_result,

    Texture_TUTORIAL,
    Texture_SCOREATTACK,

    Texture_Tu0,
    Texture_Tu1,
    Texture_Tu2,
    Texture_Tu3,
    Texture_Tu4,
    Texture_Tu5,
    Texture_Tu6,
    Texture_Tu7,
    Texture_Tu8,
    Texture_Tu9,
    Texture_Tu10,
    Texture_Tu11,
};

enum {
    Sound_TitleBGM,             // タイトルシーン
    Sound_PlayingBGM,       //ゲームプレイシーン
    Sound_ResultBGM,        // リザルト
};

enum { 
    Se_GameStart,                 // ゲーム開始
    Se_PlayerAttack,                 // 剣道部長　攻撃
    Se_PlayerDamage,        // 剣道部長　やられた
    Se_EnemyDamage,       // 空手部長　やられた
    Se_Timeout,                     // タイムアウト
    Se_BISI,                          // 敵に攻撃をあてた音
    Se_reload,
    Se_EnemyAttack,
    Se_coin,
    Se_heal,
    Se_upgrade,
    Se_EnemyDead,
    Se_Jump,
    Se_Jump_end,
    Se_Click,
    Se_QUIZ,
    Se_Goal,

    Se_PlayerBOBAttack,
    Se_PlayerBOBDamage,

};

enum {
    Effect_Clew,
    Effect_Jump,
    Effect_MissileBoost,
    Effect_Bone,
    Effect_Goal,
    Effect_BOBShort, 
};

#endif

