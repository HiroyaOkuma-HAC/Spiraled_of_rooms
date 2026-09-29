#include "World/World.h"
#include "Field.h"
#include "Actor/Actor.h"
#include <GSeffect.h>


// デストラクタ
World::~World() {
	clear();
}

// 更新
void World::update(float delta_time) {
	// フィールドの更新
	field_->update(delta_time);
	// アクターの更新
	actors_.update(delta_time);
	// アクターの衝突
	actors_.collide();
	// アクターの遅延更新
	actors_.remove();
	// カメラの更新
	camera_->update(delta_time);
	// ライトの更新
	light_->update(delta_time);
	// アクターの消去
	actors_.remove();
	// エフェクトの更新処理
	gsUpdateEffect(delta_time);
	// タイマーの更新
	timer_.update(delta_time);
}

// 描画
void World::draw() const {
	// カメラの設定
	camera_->draw();
	// エフェクト用のカメラを設定
	gsSetEffectCamera();
	// ライトの設定
	light_->draw();

	// シャドウマップの描画
	gsDrawShadowMap(World::shadow_map_callback, (void*)this);

	// フィールドの描画
	field_->draw();
	// アクターの描画
	actors_.draw();
	// 半透明アクターの描画
	actors_.draw_transparent();
	// エフェクトの描画
	gsDrawEffect();
	// GUIの描画
	actors_.draw_gui();


	// スコアの描画
	//score_.draw();
	// タイマーの描画
	timer_.draw();
	// コインの描画
	coin_.draw();

	// プレイヤーHPの描画
	player_hp_.draw();
	// 弾薬数の描画
	ammo_.draw();
	// スタミナの描画
	stamina_.draw();
	// スコアの描画
	score_.draw();
	// ストリークの描画
	streak_.draw();
	// ストリークの制限時間の描画
	streak_limit_.draw();
	// コンボの描画
	combo_.draw();

	/// アップグレード
	// マガジンの描画
	upgrademagazine_.draw();
	upgradedamage_.draw();
	upgradeshootspeed_.draw();
	upgradehp_.draw();
	upgradedeffence_.draw();

	roomcount_.draw();
	isgoal_.draw();
}

// 消去
void World::clear() {
	// アクターの消去
	actors_.clear();
	// カメラを消去
	delete camera_;
	camera_ = nullptr;
	// ライトの消去
	delete light_;
	light_ = nullptr;
	// フィールドを消去
	delete field_;
	field_ = nullptr;

	// スコアの削除
	//score_.initialize();
	// コインの削除
	coin_.initialize();


	// プレイヤーのHPを初期化
	player_hp_.initialize();
	// 弾薬数の消去
	ammo_.clear();
	// スタミナの削除
	stamina_.initialize();

	// スコアの削除
	score_.initialize();
	// ストリークの削除
	streak_.initialize();
	// ストリークの制限時間の削除
	streak_limit_.initialize();
	// コンボの削除
	combo_.initialize();

	/// アップグレード ////////////////////////////////////////////////
		// マガジンの削除
	upgrademagazine_.initialize();
	upgradedamage_.initialize();
	upgradeshootspeed_.initialize();
	upgradehp_.initialize();
	upgradedeffence_.initialize();

	roomcount_.initialize();
	isgoal_.initialize();
}


// カメラの追加
void World::add_camera(Actor* camera) {
	delete camera_; // 現在のカメラを消去
	camera_ = camera;
}

// ライトの追加
void World::add_light(Actor* light) {
	delete light_; // 現在のライトを消去
	light_ = light;
}

// フィールドの追加
void World::add_field(Field* field) {
	delete field_;	// 現在のフィールドを削除
	field_ = field;
}

// アクターの追加
void World::add_actor(Actor* actor) {
	actors_.add(actor);
}

// アクターの検索
Actor* World::find_actor(const std::string& name) const {
	return actors_.find(name);
}

// 指定したタグ名を持つアクターの検索
std::vector<Actor*> World::find_actor_with_tag(const std::string& tag) const {
	return actors_.find_with_tag(tag);
}

// アクター数を返す
int World::count_actor() const {
	return actors_.count();
}

// 指定したタグ名をもつアクター数を返す
int World::count_actor_with_tag(const std::string& tag) const {
	return actors_.count_with_tag(tag);
}

// メッセージを送信
void World::send_message(const std::string& message, void* param) {
	actors_.send_message(message, param);
}

// カメラを取得
Actor* World::camera() {
	return camera_;
}

// ライトを取得
Actor* World::light() {
	return light_;
}

// フィールドの取得
Field* World::field() {
	return field_;
}

// シャドウマップの描画用の関数
void World::shadow_map_callback(void* param, const GSmatrix4*, const GSmatrix4*) {
	World* self = (World*)param;
	// シャドウマップにはアクターのみ描画
	self->actors_.draw();

	// シャドウマップにフィールド用のアクターを描画
	self->field_->draw_actors();
}

// スコアを取得
//Score& World::score() {
//	return score_;
//}

// スコアの加算
//void World::add_score(int score) {
//	score_.add(score);
//}

// タイマーの取得
Timer& World::timer() {
	return timer_;
}

// ゲームオーバーか？
bool World::is_game_over() const {
	return timer_.is_timeout(); // 時間切れか？
}




// コインを取得
Coin& World::coin() {
	return coin_;
}

/// /
// プレイヤーHPを取得
PlayerHitPoint& World::player_hp() {
	return player_hp_;
}

// スタミナを取得
Stamina& World::stamina() {
	return stamina_;
}

// 弾薬数
void World::add_ammo(int ammo) {
	ammo_.add(ammo);
}

// 弾薬数を取得
Ammo& World::ammo_count() {
	return ammo_;
}

// スコアを取得
Score& World::score() {
	return score_;
}

// ストリークを取得
Streak& World::streak() {
	return streak_;
}

// ストリークの制限時間を取得
StreakLimit& World::streak_limit() {
	return streak_limit_;
}

// コンボを取得
Combo& World::combo() {
	return combo_;
}


/// アップグレード
// マガジンを取得
UpgradeMagazine& World::upgrademagazine() {
	return upgrademagazine_;
}
UpgradeDamage& World::upgradedamage() {
	return upgradedamage_;
}
UpgradeShootSpeed& World::upgradeshootspeed() {
	return upgradeshootspeed_;
}
UpgradeHp& World::upgradehp() {
	return upgradehp_;
}
UpgradeDeffence& World::upgradedeffence() {
	return upgradedeffence_;
}
RoomCount& World::roomcount() {
	return roomcount_;
}
isGoal& World::isgoal() {
	return isgoal_;
}