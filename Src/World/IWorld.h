#ifndef IWORLD_H_
#define IWORLD_H_

#include <string>
#include <vector>

/// アクタークラスの前方宣言
class Actor;
class Field;
class Coin;

class PlayerHitPoint;
class Ammo;
class Stamina;
class Score;
class Streak;
class StreakLimit;
class Combo;
//class Score;

/// アップグレード
class UpgradeMagazine;
class UpgradeDamage;
class UpgradeShootSpeed;
class UpgradeHp;
class UpgradeDeffence;

class RoomCount;
class isGoal;

// ワールド抽象インターフェース
class IWorld {
public:
	// 仮想デストラクタ
	virtual ~IWorld() = default;

	// アクターを追加
	virtual void add_actor(Actor* actor) = 0;
	// アクターを検索
	virtual Actor* find_actor(const std::string& name) const = 0;
	// 指定したタグ名をもつアクターの検索
	virtual std::vector<Actor*> find_actor_with_tag(const std::string& tag) const = 0;
	// アクター数を返す
	virtual int count_actor() const = 0;
	// 指定したタグ名を持つアクター数を返す
	virtual int count_actor_with_tag(const std::string& tag) const = 0;
	// メッセージの送信
	virtual void send_message(const std::string& message, void* param = nullptr) = 0;

	// フィールドの取得
	virtual Field* field() = 0;
	// カメラの取得
	virtual Actor* camera() = 0;
	// ライトの取得
	virtual Actor* light() = 0;
	// スコアを取得
	//virtual Score& score() = 0;
	// スタミナの取得
	virtual Coin& coin() = 0;
	// スコアの加算
	//virtual void add_score(int score) = 0;

	// プレイヤーがダウンしているか？
	bool Down_now_ = false;
	// スコア
	int score_ = 0;
	// ドア
	bool Door_1 = false;

	// 弾薬数の追加
	virtual void add_ammo(int ammo) = 0;
	// 弾薬数の取得
	virtual  Ammo& ammo_count() = 0;
	// プレイヤーHPを取得
	virtual PlayerHitPoint& player_hp() = 0;
	// スタミナの取得
	virtual Stamina& stamina() = 0;
	// ストリークを取得
	virtual Streak& streak() = 0;
	// ストリークの制限時間を取得
	virtual StreakLimit& streak_limit() = 0;
	// コンボを取得
	virtual Combo& combo() = 0;
	// スコアを取得
	virtual Score& score() = 0;

	/// アップグレード
		// マガジンの取得
	virtual UpgradeMagazine& upgrademagazine() = 0;
	virtual UpgradeDamage& upgradedamage() = 0;
	virtual UpgradeShootSpeed& upgradeshootspeed() = 0;
	virtual UpgradeHp& upgradehp() = 0;
	virtual UpgradeDeffence& upgradedeffence() = 0;

	virtual RoomCount& roomcount() = 0;
	virtual isGoal& isgoal() = 0;
};

#endif