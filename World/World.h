#ifndef WORLD_H_
#define WORLD_H_

#include "World/IWorld.h"
#include "Actor/ActorManager.h"

#include <gslib.h>

#include "Math/Score.h"
#include "Math/Timer.h"

#include "Math/Coin.h"

#include "Math/Score.h"
#include "Math/Streak.h"
#include "Math/StreakLimit.h"
#include "Math/Combo.h"
#include "Math/PlayerHitPoint.h"
#include "Math/Ammo.h"
#include "Math/Stamina.h"
#include "Math/RoomCount.h"
#include "Math/isGoal.h"
#include "Math/UpgradeHp.h"
#include "Math/UpgradeDeffence.h"

#include "Math/UpgradeMagazine.h"
#include "Math/UpgradeDamege.h"
#include "Math/UpgradeShootSpeed.h"

#include <GSeffect.h>

// ワールドクラス
class World : public IWorld {
public:
	// コンストラクタ
	World() = default;
	// デストラクタ
	~World();
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;
	// 消去
	void clear();
	// カメラの追加
	void add_camera(Actor* camera);
	// ライトの追加
	void add_light(Actor* light);
	// フィールドの追加
	void add_field(Field* field);

	// アクターを追加
	virtual void add_actor(Actor* actor) override;
	// アクターの検索
	virtual Actor* find_actor(const std::string& name) const override;
	// 指定したタグ名を持つアクターの検索
	virtual std::vector< Actor*> find_actor_with_tag(const std::string& tag) const override;
	// アクター数を返す
	virtual int count_actor() const override;
	// 指定したタグ名を持つアクター数を返す
	virtual int count_actor_with_tag(const std::string& tag) const override;
	// メッセージの送信
	virtual void send_message(const std::string& message, void* param = nullptr) override;

	// カメラの取得
	virtual Actor* camera() override;
	// ライトの取得
	virtual Actor* light() override;
	// フィールドの取得
	virtual Field* field() override;
	// スコアを取得
	//virtual Score& score() override;

	// コインを取得
	virtual Coin& coin() override;

	// プレイヤーHPを取得
	virtual PlayerHitPoint& player_hp() override;
	// 弾薬数の追加
	virtual void add_ammo(int ammo) override;
	// 弾薬数を取得
	virtual  Ammo& ammo_count() override;
	// スタミナを取得
	virtual Stamina& stamina() override;
	// スコアを取得
	virtual Score& score() override;
	// ストリークを取得
	virtual Streak& streak() override;
	// ストリークの制限時間を取得
	virtual StreakLimit& streak_limit() override;
	// コンボを取得
	virtual Combo& combo() override;

	/// アップグレード系
	// マガジンを取得
	virtual UpgradeMagazine& upgrademagazine() override;
	// ダメージを取得
	virtual UpgradeDamage& upgradedamage() override;
	// はやさを取得
	virtual UpgradeShootSpeed& upgradeshootspeed() override;
	// Hpを取得
	virtual UpgradeHp& upgradehp() override;
	// deffenceを取得
	virtual UpgradeDeffence& upgradedeffence() override;

	// ルーム数を取得
	virtual RoomCount& roomcount() override;
	// isgoalを取得
	virtual isGoal& isgoal() override;

	//Score& score();
	// タイマーの取得
	Timer& timer();
	// ゲームオーバーか？
	bool is_game_over() const;
	// スコアの加算
	//virtual void add_score(int score) override;


	// シャドウマップの描画用の関数
	static void shadow_map_callback(void* param, const GSmatrix4*, const GSmatrix4*);


	// ダウン中か？
	bool Down_now_ = false;


	// コピー禁止
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;
private:
	// アクターマネージャー
	ActorManager actors_;
	// ライト
	Actor* light_{ nullptr };
	// カメラ
	Actor* camera_{ nullptr };
	// フィールド
	Field* field_{ nullptr };
	// スコア
	Score score_;
	// タイマー
	Timer timer_{ 60.0f };

	// コイン
	Coin coin_;

	// プレイヤーHP
	PlayerHitPoint player_hp_;
	// 弾薬数
	Ammo           ammo_;
	// スタミナ
	Stamina stamina_;
	// ストリーク
	Streak streak_;
	// ストリークの制限時間
	StreakLimit streak_limit_;
	// コンボ
	Combo combo_;

	/// アップグレード
	// マガジン
	UpgradeMagazine upgrademagazine_;
	UpgradeDamage upgradedamage_;
	UpgradeShootSpeed upgradeshootspeed_;
	UpgradeHp upgradehp_;
	UpgradeDeffence upgradedeffence_;



	RoomCount roomcount_;
	isGoal isgoal_;

};

#endif