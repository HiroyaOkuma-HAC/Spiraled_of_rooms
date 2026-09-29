#include "Actor/PlayerBOB.h"
#include "World/IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"
#include "PlayerBOBBullet.h"
#include "PlayerBOBBullet.h"
#include "Rendering/NumberTexture.h"



#include "Math/PlayerHitPoint.h"
#include "Camera/Mycamera.h"
#include "Math/Ammo.h"
#include "World/World.h"
#include "Math/Stamina.h"
#include "Rendering/GaugeTexture.h"


/// ーーーモーション番号の定義！ーーーー
enum {
	BMotion_Fire = 0,
	BMotion_Jump = 1,
	BMotion_FireWalk_BACK = 3,
	BMotion_FireWalk_FORWARD = 4,
	BMotion_FireWalk_LEFT = 5,
	BMotion_FireWalk_RIGHT = 6,
	BMotion_Idle = 7,
	BMotion_Walk_BACK = 10,
	BMotion_Walk_FORWARD = 11,
	BMotion_Walk_LEFT = 12,
	BMotion_Walk_RIGHT = 13,
	BMotion_Reload_Idle = 15,
	BMotion_Dead = 16,
	BMotion_Damage = 17,
	BMotion_Emote_Bubbin = 18,
	BMotion_Emote_Burnor = 19,
	BMotion_Emote_Show = 20,


	eBMotion_Idle = 100,	// アイドル
	BMotion_Walk,	// 歩く
	MotionJumpStart,    // ジャンプ開始
	MotionJumpMid,    // ジャンプ中
	MotionJumpEnd,    // 着地
	BMotion_Attack,	// 攻撃
	//BMotion_Damage ,	// 被弾
	BMotion_Wakeup,	// 起き上がり
	BMotion_Attack_2, // 2コンボ攻撃
	BMotion_Attack_3, // 3コンボ攻撃
	MotionJumpAttack     // ジャンプ攻撃
};
/// ーーー定数の定義！ーーーーーーーーーー
// 移動速度
const float BOBWalkSpeed{ 0.075f };
// 自分の高さ
const float BOBPlayerBOBHeight{ 0.7f };
// 衝突判定用の半径
const float BOBPlayerBOBRadius{ 0.4f };
// 足元のオフセット
const float BOBFootOffset{ 0.1f };
// 重力値
const float BOBGravity{ -0.009f };
// x軸の移動できる範囲の制限
const float BOBMOVE_AREA_MINUS_X{ -20.0f };
const float BOBMOVE_AREA_PLUS_X{ 20.0f };
// z軸の移動できる範囲の制限
const float BOBMOVE_AREA_MINUS_Z{ -20.0f };
const float BOBMOVE_AREA_PLUS_Z{ 20.0f };

bool BOBMOVE_USING{ false };
bool BOBJUMP_ATTACK_USE{ false };
bool BOBJUMP_ATTACK_USED{ false };

bool BOBCOMBO2_USING{ false };
bool BOBCOMBO3_USING{ false };

int BOBid = 36;
/// ーーーーーーーーーーーーーーーーーーーー

// コンストラクタ
PlayerBOB::PlayerBOB(IWorld* world, const GSvector3& position) :
	mesh_{ Mesh_PlayerBOB,BMotion_Idle,true },
	motion_{ BMotion_Idle },
	motion_loop_{ true },
	state_{ State::Move },
	state_timer_{ 0.0f }

{
	// ワールドを設定
	world_ = world;
	tag_ = "PlayerBOBTag";
	name_ = "PlayerBOB";
	// 衝突判定球の設定
	collider_ = BoundingSphere{ BOBPlayerBOBRadius, GSvector3{0.0f, BOBPlayerBOBHeight, 0.0f} };
	// 座標の初期化
	transform_.position(position);
	// メッシュの変換行列を初期化
	mesh_.transform(transform_.localToWorldMatrix());

	world_->Down_now_ = false;
	reload = false;

	reload_timer_ = 0.0f;
	reload_time_ = 60.0f;

	GSuint motion = BMotion_Idle;
}

// 更新
void PlayerBOB::update(float delta_time) {
	float damage_A_manager;
	if (world_->player_hp().get() <= (world_->player_hp().getmax() / 10) * 6) damage_A_manager = 0.7f;
	else if ((world_->roomcount().get() * 10) / world_->upgradedeffence().get() >= world_->player_hp().get()) damage_A_manager = 0.7f;
	else damage_A_manager = 0.0f;
	if (damage_A > damage_A_manager) damage_A -= 0.04f;

	if (transform_.position().y <= -10.0f) {
		transform_.position(GSvector3{ 3.0f * 7.5f, 30.0f,  3.0f * 7.5f });
	}


	// エフェクトに自身のワールド変換行列を設定
	GSmatrix4 world = transform_.localToWorldMatrix();
	gsSetEffectMatrix(Effect_Jump, &world); // ワールド変換行列を設定

	if (staminaCT > 0.0f) {
		staminaCT -= 1.0f * delta_time;
	}
	if (staminaCT <= 0.0f) {
		world_->stamina().add(10);
	}

	//printf("ammo %d", world_->ammo_count());

	// 状態の更新
	update_state(delta_time);
	// 重力で下向きに加速
	velocity_.y += BOBGravity * delta_time;
	// y方向に移動
	transform_.translate(0.0f, velocity_.y, 0.0f);
	// フィールドとの衝突判定
	collide_field();
	//collide_up_field();
	// モーションを変更
	mesh_.change_motion(motion_, motion_loop_);
	// メッシュを更新
	mesh_.update(delta_time);
	// 行列を設定
	mesh_.transform(transform_.localToWorldMatrix());

	/// リロード処理
// ステートがmoveの時にRキーを押したときにリロード
	if (gsGetKeyTrigger(GKEY_R) && state_ == State::Move || world_->ammo_count().get() == 0 && state_ == State::Move) {
		reload = true;

	}

	if (reload == true) {
		motion_ = BMotion_Reload_Idle;
		reload_timer_ += 1.0f;
	}

	if (reload_timer_ == 1.0f) gsPlaySE(Se_reload);

	if (reload_timer_ >= reload_time_) {
		reload = false;
		world_->add_ammo(10000);
		while (world_->ammo_count().get() > world_->upgrademagazine().get()) {
			world_->add_ammo(-1);
		}
		reload_timer_ = 0.0f;
	}

	if (world_->isgoal().get() == 4) {
		GSvector3 setpos{ 3.0f * 7.0f, 30.0f,  3.0f * 7.0f };
		transform_.position(setpos);
		world_->roomcount().add(1);
		tag_ = "PlayerBOBTag";
		world_->isgoal().sub(2);
	}

	if (color_ < 1.0f) color_ += 0.05f;
	if (color_ > 1.0f) color_ = 1.0f;

	if (world_->player_hp().get() <= 0) {



		a_color_ -= 0.02f;
		if (a_color_ <= 0.0f) {}
		//	die();
		is_dead_ = true;
		change_state(State::Dead, BMotion_Dead, false);

	}
}

// 描画
void PlayerBOB::draw() const {
	if (is_dead_ == true) {
	}
	if (!gsGetMouseButtonState(GMOUSE_BUTTON_2)) {
		// 色を変える
		GScolor4 meshcolor = { 1.0f,color_,color_,a_color_ };
		glColor4fv(meshcolor);

		// メッシュの描画
		mesh_.draw();

		// 色をもどす
		meshcolor = { 1.0f,1.0f,1.0f,1.0f };
		glColor4fv(meshcolor);
		// コライダーの描画
	   /// collider().draw();
		if (state_ == State::JumpEnd || state_ == State::JumpMid || state_ == State::JumpStart || state_ == State::Run || world_->player_hp().get() >= 1)
		{
		}
		else {
			// 武器の描画
			draw_weapon();
		}
	}



	if (gsGetMouseButtonState(GMOUSE_BUTTON_2))
	{
		GSvector2 ADSPOS = { 0 - (21 / 2),-30 - (21 / 2) };
		gsDrawSprite2D(Texture_BOBADS, &ADSPOS, NULL, NULL, NULL, NULL, 0.0f);
	}



	// 16, 16 は読み込んだ画像のサイズ
	static const  GaugeTexture RELOADgauge{ GaugeBlack, GaugeWhite	, 16, 16 };
	// ゲージのカラー（半透明）
	static const GScolor RELOADgauge_color{ 1.0f, 1.0f, 1.0f, 0.8f };
	// ゲージの背景カラー（半透明）
	static const GScolor RELOADbackground_color{ 1.0f, 1.0f, 1.0f, 0.8f };

	// (200, 2) の位置に、(100, 16)のサイズのゲージを表示（この位置とサイズは適当です）。
	// ゲージの数値は現在のHP、ゲージ最大値は最大HP。
	RELOADgauge.draw(GSvector2{ 920, 627 }, 220, 7, reload_timer_, reload_time_, RELOADgauge_color, RELOADbackground_color);

	// 16, 16 は読み込んだ画像のサイズ
	static const GaugeTexture ammogauge{ GaugeWhite, GaugeBlack, 16, 16 };
	// ゲージのカラー（半透明）
	static const GScolor ammogauge_color{ 1.0f, 1.0f, 1.0f, 0.8f };
	// ゲージの背景カラー（半透明）
	static const GScolor ammobackground_color{ 1.0f, 1.0f, 1.0f, 0.8f };

	// (120, 640) の位置に、(250, 40)のサイズのゲージを表示
	// ゲージの数値は現在のHP、ゲージ最大値は最大HP。
	ammogauge.draw(GSvector2{ 910, 640 }, 250, 40, world_->ammo_count().get(), world_->upgrademagazine().get(), ammogauge_color, ammobackground_color);


	GSvector2 DamagePOS = { 0 ,0 };
	GScolor4 DamageCol = { 1.0f,1.0f,1.0f,damage_A };
	gsDrawSprite2D(Texture_Damage, &DamagePOS, NULL, NULL, &DamageCol, NULL, 0.0f);

	///printf("%d \n", motion_);
}

/// ーーー状態の更新ーーーーーーーーーー
void PlayerBOB::update_state(float delta_time) {
	// 状態遷移
	switch (state_) {
	case State::Move:   move(delta_time); break;
	case State::Attack: attack(delta_time); break;
	case State::Attack2: attack2(delta_time); break;
	case State::Attack3: attack3(delta_time); break;
	case State::Damage: damage(delta_time); break;
	case State::Wakeup: wakeup(delta_time); break;
	case State::Graple: graple(delta_time); break;
	case State::Run: run(delta_time); break;
	case State::Dead: dead(delta_time); break;
	case State::JumpStart:  jump_start(delta_time);  break;
	case State::JumpMid:    jump_mid(delta_time);    break;
	case State::JumpEnd:    jump_end(delta_time);    break;
	case State::JumpAttack: jump_attack(delta_time); break;
	}
	// 状態タイマーの更新
	state_timer_ += delta_time;
}

/// ーーー状態の変更ーーーーーーーーーー
void PlayerBOB::change_state(State state, GSuint motion, bool loop) {
	motion_ = motion;
	motion_loop_ = loop;
	state_ = state;
	state_timer_ = 0.0f;
}

// 移動処理
void PlayerBOB::move(float delta_time) {
	world_->Down_now_ = false;



	if (SCT >= 0.0f) SCT -= delta_time;
	if (SCT <= 0.0f) {
		// スペースキーで攻撃
		if (gsGetMouseButtonState(GMOUSE_BUTTON_1) && world_->ammo_count().get() >= 1) {
			// 自分の前方に衝突判定を出現させる
			//generate_bullet1();
			SCT = world_->upgradeshootspeed().get();
			// 攻撃効果音を再生
			gsPlaySE(Se_PlayerBOBAttack);
			// 攻撃中に遷移
			change_state(State::Attack, BMotion_Fire, true);
			return;
		}
	}

	// 何もしなければアイドル状態
	if (SCT > 0) {
		motion_ = BMotion_Fire;
	}
	else if (reload == true) {
		motion_ = BMotion_Reload_Idle;
	}
	else if (gsGetKeyState(GKEY_1)) {
		motion_ = BMotion_Emote_Bubbin;
	}
	else if (gsGetKeyState(GKEY_2)) {
		motion_ = BMotion_Emote_Burnor;
	}
	else if (gsGetKeyState(GKEY_3)) {
		motion_ = BMotion_Emote_Show;
	}
	else {
		motion_ = BMotion_Idle;
	}
	// 左右キーでｙ軸周りに回転
	//float yaw{ 0.0f };
	//if (gsGetKeyState(GKEY_A))  yaw = 3.0f;
	//if (gsGetKeyState(GKEY_D)) yaw = -3.0f;
	//transform_.rotate(0.0f, yaw * delta_time, 0.0f);
	// 上キーで前進
	float speed{ 0.0f };
	float side_speed{ 0.0f };
	if (gsGetKeyState(GKEY_W)) {
		speed = 0.1f;
		//motion = BMotion_Walk;
		BOBMOVE_USING = true;
	}

	// ジャンプステートに遷移する
	if (gsGetKeyTrigger(GKEY_SPACE)) {
		gsPlaySE(Se_Jump);
		GSvector3 jumpeffectposition = transform_.position();
		jumpeffectposition.y += 0.3f;
		// gsPlayEffect(Effect_Jump, &jumpeffectposition);
		// ジャンプ開始状態へ
		change_state(State::JumpStart, BMotion_Jump, false);
		// ジャンプ
		velocity_.y = 0.25f;
		jump_attack_done_ = false;
		return;
	}
	BOBMOVE_USING = false;
	change_state(State::Move, motion_);
	// グラウンド内にクランプ
	GSvector3 position = transform_.position();
	//	position.x = CLAMP(position.x, -20.0f, 20.0f);
	//	position.z = CLAMP(position.z, -20.0f, 20.0f);
	transform_.position(position);

	/// ///////////////////////////////////////////////////////////////////////////////
  // カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	// カメラの右方向ベクトルを取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;

	// キーの入力値から移動ベクトルを計算
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}

	if (gsGetKeyState(GKEY_W)) {
		velocity += forward;
		speed = 0.1f;
		if (SCT > 0) {
			motion_ = BMotion_FireWalk_FORWARD;
		}
		else  if (reload == true) {
			motion_ = BMotion_Reload_Idle;
			speed = 0.05f;
		}
		else if (gsGetKeyTrigger(GKEY_LSHIFT) && world_->stamina().get() > 0) {
			motion_ = 14;
			change_state(State::Run, 14, true);
			return;
		}
		else {
			motion_ = BMotion_Walk_FORWARD;
		}
	}
	if (gsGetKeyState(GKEY_S)) {
		velocity -= forward;
		speed = -0.1f;
		if (SCT > 0) {
			motion_ = BMotion_FireWalk_BACK;
		}
		else  if (reload == true) {
			motion_ = BMotion_Reload_Idle;
			speed = -0.05f;
		}
		else {
			motion_ = BMotion_Walk_BACK;
		}
	}
	if (gsGetKeyState(GKEY_A)) {
		velocity -= right;
		side_speed = 0.1f;
		if (SCT > 0) {
			motion_ = BMotion_FireWalk_LEFT;
		}
		else  if (reload == true) {
			motion_ = BMotion_Reload_Idle;
			side_speed = 0.05f;
		}
		else {
			motion_ = BMotion_Walk_LEFT;
		}
	}
	if (gsGetKeyState(GKEY_D)) {
		velocity += right;
		side_speed = -0.1f;
		if (SCT > 0) {
			motion_ = BMotion_FireWalk_RIGHT;
		}
		else  if (reload == true) {
			motion_ = BMotion_Reload_Idle;
			side_speed = -0.05f;
		}
		else {
			motion_ = BMotion_Walk_RIGHT;
		}
	}
	velocity = velocity.normalized() * 0.075f * delta_time;

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}
	/// ///////////////////////////////////////////////////////////////////////////////

	if (walkenable_ == true) {
		// 前進する(ローカル座標基準）
		transform_.translate(side_speed * 2, 0.0f, speed * 2 * delta_time);
		walkenable_ = false;
	}

	else if (walkenable_ == false) {
		side_speed = 0.0f;
		speed = 0.0f;
		transform_.translate(side_speed, 0.0f, speed * delta_time);
		walkenable_ = true;
	}



}

// 攻撃中
void PlayerBOB::attack(float delta_time) {
	if (world_->ammo_count().get() <= 0) {

		change_state(State::Move, BMotion_Idle, true);
		return;
	}
	//motion_ = BMotion_Fire;
// 自分の前方に衝突判定を出現させる
	generate_bullet1();
	/// ///////////////////////////////////////////////////////////////////////////////
	float speed{ 0.0f };
	float side_speed{ 0.0f };
	// カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	// カメラの右方向ベクトルを取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;

	// キーの入力値から移動ベクトルを計算
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}


	if (gsGetKeyState(GKEY_W)) {
		velocity += forward;
		speed = 0.1f;
		motion_ = BMotion_FireWalk_FORWARD;
	}
	if (gsGetKeyState(GKEY_S)) {
		velocity -= forward;
		speed = -0.1f;
		motion_ = BMotion_FireWalk_BACK;
	}
	if (gsGetKeyState(GKEY_A)) {
		velocity -= right;
		side_speed = 0.1f;
		motion_ = BMotion_FireWalk_LEFT;
	}
	if (gsGetKeyState(GKEY_D)) {
		velocity += right;
		side_speed = -0.1f;
		motion_ = BMotion_FireWalk_RIGHT;
	}
	velocity = velocity.normalized() * 0.075f * delta_time;

	if (walkenable_ == true) {
		// 前進する(ローカル座標基準）
		transform_.translate(side_speed * 2, 0.0f, speed * 2 * delta_time);
		walkenable_ = false;
	}

	else if (walkenable_ == false) {
		side_speed = 0.0f;
		speed = 0.0f;
		transform_.translate(side_speed, 0.0f, speed * delta_time);
		walkenable_ = true;
	}


	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}
	/// ///////////////////////////////////////////////////////////////////////////////




	// 攻撃モーションの終了を待つ
///	if (state_timer_ >= 20.0f) { 
	if (state_timer_ >= 0.0f) {
		//if (gsGetMouseButtonState(GMOUSE_BUTTON_1)) {
		//	state_timer_ = 0.0f;
		//	return;
		//}
	//if (gsGetMouseButtonState(GMOUSE_BUTTON_1)) {
		move(delta_time);
		//motion_ = BMotion_Idle;
		//change_state(State::Move, BMotion_Idle,true);
		state_timer_ = 0.0f;
		return;
	}
}

// ２コンボ攻撃中
void PlayerBOB::attack2(float delta_time) {
	if (state_timer_ <= 20.0f && gsGetKeyState(GKEY_SPACE)) {
		BOBCOMBO3_USING = true;
	}
	if (state_timer_ >= 20.0f && BOBCOMBO3_USING) {
		change_state(State::Attack3, BMotion_Attack_3);
		BOBCOMBO3_USING = false;
		// 自分の前方に衝突判定を出現させる
		generate_bullet();
		// 攻撃効果音を再生
		gsPlaySE(Se_PlayerBOBAttack);
		return;
	}
	// 攻撃モーションの終了を待つ
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
		motion_ = BMotion_Idle;
		return;
	}
}

// ３コンボ攻撃中
void PlayerBOB::attack3(float delta_time) {
	// 攻撃モーションの終了を待つ
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
		motion_ = BMotion_Idle;
		return;
	}
}

// ダメージ中
void PlayerBOB::damage(float delta_time) {


	float speed{ 0.0f };
	float side_speed{ 0.0f };
	// カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	// カメラの右方向ベクトルを取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;



	speed = -0.1f + state_timer_ / 3000;



	if (walkenable_ == true) {
		// 前進する(ローカル座標基準）
		transform_.translate(side_speed * 2, 0.0f, speed * 2 * delta_time);
		walkenable_ = false;
	}

	else if (walkenable_ == false) {
		side_speed = 0.0f;
		speed = 0.0f;
		transform_.translate(side_speed, 0.0f, speed * delta_time);
		walkenable_ = true;
	}
	// ダメージモーションの終了を待つ
	if (state_timer_ >= mesh_.motion_end_time()) {
		// 起き上がり中に遷移する
		change_state(State::Wakeup, BMotion_Wakeup, false);
		return;
	}
}

// 起き上がり中
void PlayerBOB::wakeup(float delta_time) {
	// 攻撃モーションの終了を待つ
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
		return;
	}
}

// グラップル
void PlayerBOB::graple(float delta_time) {
	// 攻撃モーションの終了を待つ
	if (state_timer_ >= mesh_.motion_end_time()) {
		move(delta_time);
		return;
	}
}

// 走る
void PlayerBOB::run(float delta_time) {
	if (gsGetKeyTrigger(GKEY_SPACE)) {
		gsPlaySE(Se_Jump);
		// ジャンプ開始状態へ
		change_state(State::JumpStart, BMotion_Jump, false);
		// ジャンプ
		velocity_.y = 0.25f;
		jump_attack_done_ = false;
		return;
	}

	if (world_->stamina().get() <= 0) {
		move(delta_time);
		return;
	}
	float speed{ 0.0f };
	float side_speed{ 0.0f };
	// カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	// カメラの右方向ベクトルを取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;

	// キーの入力値から移動ベクトルを計算
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}


	if (gsGetKeyState(GKEY_A)) {
		side_speed = 0.07f;
	}
	if (gsGetKeyState(GKEY_D)) {
		side_speed = -0.07f;
	}

	if (gsGetKeyState(GKEY_W)) {
		velocity += forward;
		speed = 0.155f;
		motion_ = 9;
		world_->stamina().sub(5);
		staminaCT = 300.0f;
	}
	if (gsGetKeyState(GKEY_LSHIFT)) {
	}
	else {
		move(delta_time);
		return;
	}
	velocity = velocity.normalized() * 0.075f * delta_time;

	if (walkenable_ == true) {
		// 前進する(ローカル座標基準）
		transform_.translate(side_speed * 2, 0.0f, speed * 2 * delta_time);
		walkenable_ = false;
	}

	else if (walkenable_ == false) {
		side_speed = 0.0f;
		speed = 0.0f;
		transform_.translate(side_speed, 0.0f, speed * delta_time);
		walkenable_ = true;
	}

}

// でっど
void PlayerBOB::dead(float delta_time) {
	is_dead_ = true;
	if (state_timer_ >= mesh_.motion_end_time()) {
		a_color_ -= 0.02f;
		if (a_color_ <= 0.0f) {}
		//	die();
	}
}

// ジャンプ開始
void PlayerBOB::jump_start(float delta_time) {
	if (state_timer_ >= 8) {
		// ある程度したら、すぐにジャンプ中モーションへ
		change_state(State::JumpMid, BMotion_Jump, false);
		gsStopEffect(Effect_Jump);
	}


	float speed{ 0.0f };
	float side_speed{ 0.0f };
	// カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	// カメラの右方向ベクトルを取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;

	// キーの入力値から移動ベクトルを計算
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}


	if (gsGetKeyState(GKEY_W)) {
		velocity += forward;
		speed = 0.1f;
	}
	if (gsGetKeyState(GKEY_S)) {
		velocity -= forward;
		speed = -0.1f;
	}
	if (gsGetKeyState(GKEY_A)) {
		velocity -= right;
		side_speed = 0.1f;
	}
	if (gsGetKeyState(GKEY_D)) {
		velocity += right;
		side_speed = -0.1f;
	}
	velocity = velocity.normalized() * 0.075f * delta_time;

	if (walkenable_ == true) {
		// 前進する(ローカル座標基準）
		transform_.translate(side_speed * 2, 0.0f, speed * 2 * delta_time);
		walkenable_ = false;
	}

	else if (walkenable_ == false) {
		side_speed = 0.0f;
		speed = 0.0f;
		transform_.translate(side_speed, 0.0f, speed * delta_time);
		walkenable_ = true;
	}



	/// ///////////////////////////////////////////////////////////////////////////////
// カメラの前方向ベクトルを取得
	forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	// カメラの右方向ベクトルを取得
	right = world_->camera()->transform().right();
	right.y = 0.0f;

	// キーの入力値から移動ベクトルを計算
	velocity = { 0.0f, 0.0f, 0.0f };

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}

	if (gsGetKeyState(GKEY_W)) velocity += forward;
	if (gsGetKeyState(GKEY_S)) velocity -= forward;
	if (gsGetKeyState(GKEY_A)) velocity -= right;
	if (gsGetKeyState(GKEY_D)) velocity += right;
	velocity = velocity.normalized() * 0.075f * delta_time;

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}
	/// ///////////////////////////////////////////////////////////////////////////////
}

// ジャンプ中
void PlayerBOB::jump_mid(float delta_time) {

	if (gsGetKeyTrigger(GKEY_SPACE)) {
		gsPlaySE(Se_Jump);
		// ジャンプ開始状態へ
		change_state(State::JumpStart, BMotion_Jump, false);
		// ジャンプ
		velocity_.y = 0.25f;
		jump_attack_done_ = false;
		return;
	}


	float speed{ 0.0f };
	float side_speed{ 0.0f };
	// カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	// カメラの右方向ベクトルを取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;

	// キーの入力値から移動ベクトルを計算
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}


	if (gsGetKeyState(GKEY_W)) {
		velocity += forward;
		speed = 0.1f;
	}
	if (gsGetKeyState(GKEY_S)) {
		velocity -= forward;
		speed = -0.1f;
	}
	if (gsGetKeyState(GKEY_A)) {
		velocity -= right;
		side_speed = 0.1f;
	}
	if (gsGetKeyState(GKEY_D)) {
		velocity += right;
		side_speed = -0.1f;
	}
	velocity = velocity.normalized() * 0.075f * delta_time;

	if (walkenable_ == true) {
		// 前進する(ローカル座標基準）
		transform_.translate(side_speed * 2, 0.0f, speed * 2 * delta_time);
		walkenable_ = false;
	}

	else if (walkenable_ == false) {
		side_speed = 0.0f;
		speed = 0.0f;
		transform_.translate(side_speed, 0.0f, speed * delta_time);
		walkenable_ = true;
	}

	/// ///////////////////////////////////////////////////////////////////////////////
// カメラの前方向ベクトルを取得
	forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	// カメラの右方向ベクトルを取得
	right = world_->camera()->transform().right();
	right.y = 0.0f;

	// キーの入力値から移動ベクトルを計算
	velocity = { 0.0f, 0.0f, 0.0f };

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}

	if (gsGetKeyState(GKEY_W)) velocity += forward;
	if (gsGetKeyState(GKEY_S)) velocity -= forward;
	if (gsGetKeyState(GKEY_A)) velocity -= right;
	if (gsGetKeyState(GKEY_D)) velocity += right;
	velocity = velocity.normalized() * 0.075f * delta_time;

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}
	/// ///////////////////////////////////////////////////////////////////////////////
}

// ジャンプ終了（着地）
void PlayerBOB::jump_end(float delta_time) {
	if (state_timer_ >= 1) {
		// ある程度したら、すぐに通常状態へ
		motion_ = BMotion_Idle;
		change_state(State::Move, BMotion_Idle, true);
	}

	float speed{ 0.0f };
	float side_speed{ 0.0f };
	// カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	// カメラの右方向ベクトルを取得
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;

	// キーの入力値から移動ベクトルを計算
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}


	if (gsGetKeyState(GKEY_W)) {
		velocity += forward;
		speed = 0.1f;
	}
	if (gsGetKeyState(GKEY_S)) {
		velocity -= forward;
		speed = -0.1f;
	}
	if (gsGetKeyState(GKEY_A)) {
		velocity -= right;
		side_speed = 0.1f;
	}
	if (gsGetKeyState(GKEY_D)) {
		velocity += right;
		side_speed = -0.1f;
	}
	velocity = velocity.normalized() * 0.075f * delta_time;

	if (walkenable_ == true) {
		// 前進する(ローカル座標基準）
		transform_.translate(side_speed * 2, 0.0f, speed * 2 * delta_time);
		walkenable_ = false;
	}

	else if (walkenable_ == false) {
		side_speed = 0.0f;
		speed = 0.0f;
		transform_.translate(side_speed, 0.0f, speed * delta_time);
		walkenable_ = true;
	}

	/// ///////////////////////////////////////////////////////////////////////////////
// カメラの前方向ベクトルを取得
	forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	// カメラの右方向ベクトルを取得
	right = world_->camera()->transform().right();
	right.y = 0.0f;

	// キーの入力値から移動ベクトルを計算
	velocity = { 0.0f, 0.0f, 0.0f };

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}

	if (gsGetKeyState(GKEY_W)) velocity += forward;
	if (gsGetKeyState(GKEY_S)) velocity -= forward;
	if (gsGetKeyState(GKEY_A)) velocity -= right;
	if (gsGetKeyState(GKEY_D)) velocity += right;
	velocity = velocity.normalized() * 0.075f * delta_time;

	/// むきをかえる～！

	if (velocity.length() == 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(world_->camera()->transform().forward() / 1000), 12000.0f * delta_time);
		rotation.x = 0.0f;
		rotation.z = 0.0f;
		transform_.rotation(rotation);
	}
	/// ///////////////////////////////////////////////////////////////////////////////
}

// ジャンプ攻撃
void PlayerBOB::jump_attack(float delta_time) {
	// 移動しない
	velocity_ = GSvector3{ 0, 0, 0 };
	// モーションが終了したらジャンプ中に遷移
	if (state_timer_ >= mesh_.motion_end_time()) {
		// ジャンプ中状態に遷移
		change_state(State::JumpMid, MotionJumpMid);
	}
}

// 弾の生成
void  PlayerBOB::generate_bullet() {
	// 弾を生成する場所の距離
	const float GenerateDistance{ 1.0f };
	// 生成する位置の高さの補正値
	const float GenerateHeight{ 1.0f };
	// 弾の移動スピード
	const float Speed{ 0.0f };
	// 生成位置の計算
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	// 生成位置の高さを補正する
	position.y += GenerateHeight;
	// 移動量の計算
	GSvector3 velocity = transform_.forward() * Speed;
	// 弾の生成
	world_->add_actor(new PlayerBOBBullet{ world_, position, velocity });
}

/// ーーー 衝突リアクションーーーーーーーーーー
void PlayerBOB::react(Actor& other) {
	if (world_->player_hp().get() <= 0) return;
	if (tag_ == "NullTag") return;
	if (other.tag() == "GoalTag") {
		world_->isgoal().sub(1);
		tag_ = "NullTag";
	}
	// ダメージ中か、起き上がり中ならなにもしない
	if (state_ == State::Damage || state_ == State::Wakeup) return;
	if (other.tag() == "EnemyTag") {
		//change_state(State::Damage, BMotion_Damage, false);
		// やられた効果音を再生
		//gsPlaySE(Se_PlayerBOBDamage);
		//world_->player_hp().sub(world);
	}
	if (other.tag() == "WolfEnemyAttackTag" || other.tag() == "RemootAttackTag" || other.tag() == "TrapCollisionTag" || other.tag() == "BoneAttackTag") {

		color_ = 0.3f;
		damage_A = 1.0f;

		velocity_ = other.velocity().getNormalized() * -1.5f;
		change_state(State::Damage, BMotion_Damage, false);
		// やられた効果音を再生
		gsPlaySE(Se_PlayerBOBDamage);
		world_->player_hp().sub(
			(world_->roomcount().get() * 10) / world_->upgradedeffence().get()
		);
	}
}

// フィールドとの衝突判定
void PlayerBOB::collide_field() {
	// 壁との衝突判定（球体との判定）
	GSvector3 center; // 衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		// y座標は変更しない
		center.y = transform_.position().y;
		// 補正後の座標に変更する
		transform_.position(center);
	}
	// 地面との衝突判定（線分との交差判定）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f, -BOBFootOffset, 0.0f };
	GSvector3 intersect;            // 地面との交点
	// 衝突したフィールド用アクター
	Actor* field_actor{ nullptr };
	// 親をリセットしておく
	transform_.parent(nullptr);
	if (world_->field()->collide(line, &intersect, nullptr, &field_actor)) {
		// 交差した点からy座標のみ補正する
		position.y = intersect.y;
		// 座標を変更する
		transform_.position(position);
		// 重力を初期化する
		velocity_.y = 0.0f;
		// フィールド用のアクタークラスと衝突したか？
		if (field_actor != nullptr) {
			// 衝突したフィール用のアクターを親のトランスフォームクラスとして設定
			transform_.parent(&field_actor->transform());
		}
		// ジャンプ中だったら、着地する
		if (state_ == State::JumpMid) {
			// 速度を止める
			// 着地状態へ
			change_state(State::JumpEnd, BMotion_Jump, false);
			gsPlaySE(Se_Jump_end);
		}
	}



	Line up_line;
	up_line.start = position + collider_.center;
	up_line.end = up_line.start + GSvector3{ 0.0f, 0.3f, 0.0f }; // 上にしっかり伸ばす！

	GSvector3 ceiling_intersect;

	if (world_->field()->collide(up_line, &ceiling_intersect)) {
		float head_y = up_line.start.y;

		// 衝突点が「頭よりしっかり上にあるか？」
		if (ceiling_intersect.y > head_y - 0.01f) {
			// いちごっちん補正！
			position.y = ceiling_intersect.y - collider_.center.y - collider_.radius;
			transform_.position(position);

			if (velocity_.y > 0.0f) {
				velocity_.y = 0.0f;
			}

		}
		else {

		}
	}






}

// フィールドとの衝突判定 (天井)
void PlayerBOB::collide_up_field() {
	// 壁との衝突判定（球体との判定）
	GSvector3 center; // 衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		// y座標は変更しない
		center.y = transform_.position().y;
		// 補正後の座標に変更する
		transform_.position(center);
	}
	// 地面との衝突判定（線分との交差判定）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f, -BOBFootOffset, 0.0f };
	GSvector3 intersect;            // 地面との交点
	// 衝突したフィールド用アクター
	Actor* field_actor{ nullptr };
	// 親をリセットしておく
	transform_.parent(nullptr);
	if (world_->field()->collide(line, &intersect, nullptr, &field_actor)) {
		// 交差した点からy座標のみ補正する
		position.y = intersect.y;
		// 座標を変更する
		transform_.position(position);
		// 重力を初期化する
		velocity_.y = 0.0f;
		// フィールド用のアクタークラスと衝突したか？
		if (field_actor != nullptr) {
			// 衝突したフィール用のアクターを親のトランスフォームクラスとして設定
			transform_.parent(&field_actor->transform());
		}

		// ジャンプ中だったら、着地する
		if (state_ == State::JumpMid) {
			// 速度を止める
			velocity_ = GSvector3::zero();
			// 着地状態へ
			change_state(State::JumpEnd, BMotion_Jump, false);
		}
	}
}


/// /////////////
// 弾の生成
void  PlayerBOB::generate_bullet1() {
	// 弾を生成する場所の距離
	const float GenerateDistance{ 0.5f };
	// 生成する位置の高さの補正値
	const float GenerateHeight{ 1.5f };
	// 弾の移動スピード
	const float Speed{ 1.5f };
	// 生成位置の計算
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	// 生成位置の高さを補正する
	position.y += GenerateHeight;
	// 移動量の計算
	GSvector3 velocity = transform_.forward() * Speed;

	//world_->add_ammo(-1);

	// 効果音
	//gsPlaySE(Se_MainShot);
	world_->add_ammo(-1);

	// 弾の生成
	for (int i = 0; i < 7; ++i) {
		world_->add_actor(new PlayerBOBBullet{ world_, position, velocity });

	}
}

/// グラップル
void  PlayerBOB::generate_bulletgraple() {
	// 弾を生成する場所の距離
	const float GenerateDistance{ 0.5f };
	// 生成する位置の高さの補正値
	const float GenerateHeight{ 1.5f };
	// 弾の移動スピード
	const float Speed{ 1.5f };
	// 生成位置の計算
	GSvector3 position = transform_.position() + transform_.forward() * GenerateDistance;
	// 生成位置の高さを補正する
	position.y += GenerateHeight;
	// 移動量の計算
	GSvector3 velocity = transform_.forward() * Speed;

	//world_->add_ammo(-1);

	// 効果音
	//gsPlaySE(Se_MainShot);
	world_->add_ammo(-1);

	// 弾の生成
	//world_->add_actor(new PlayerBOBBulletGraple{ world_, position, velocity });
}

// 武器の描画
void PlayerBOB::draw_weapon() const {

	static const NumberTexture number{ Texture_Number, 64, 64 };

	glPushMatrix();
	//number.draw(GSvector2{ 20, 20 }, id, 10);
	// 手のボーン（19番目）の位置に武器のメッシュを描画
	glMultMatrixf(mesh_.bone_matrices(BOBid));



	gsDrawMesh(Mesh_Weapon);
	glPopMatrix();
}

/*
// フィールドとの衝突判定 (天井)
void PlayerBOB::collide_up_field() {
	// 壁との衝突判定（球体との判定）
	GSvector3 center; // 衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		// y座標は変更しない
		//center.y = transform_.position().y;
		// 補正後の座標に変更する
		transform_.position(center);
	}
	// 地面との衝突判定（線分との交差判定）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f, 0.15f, 0.0f };
	GSvector3 intersect;            // 地面との交点
	// 衝突したフィールド用アクター
	Actor* field_actor{ nullptr };
	// 親をリセットしておく
	transform_.parent(nullptr);
	if (world_->field()->collide(line, &intersect, nullptr, &field_actor)) {
		// 交差した点からy座標のみ補正する
		position.y -= 1.2f;
		// 座標を変更する
		transform_.position(position);
		// 重力を初期化する
		//velocity_.y = 0.0f;
		// フィールド用のアクタークラスと衝突したか？
		if (field_actor != nullptr) {
			// 衝突したフィール用のアクターを親のトランスフォームクラスとして設定
			transform_.parent(&field_actor->transform());
		}

		// ジャンプ中だったら、着地する
		if (state_ == State::JumpMid) {
			// 速度を止める
			velocity_.y -= 0.0f;
			// 着地状態へ
			//change_state(State::JumpEnd, MotionJumpEnd, false);
		}
	}
}
*/