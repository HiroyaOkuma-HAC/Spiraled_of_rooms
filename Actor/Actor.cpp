#include "Actor/Actor.h"
#include <GSeffect.h>

// 更新
void Actor::update(float) {}

// 遅延更新
void Actor::late_update(float) {}

// 描画
void Actor::draw() const {}

// 半透明オブジェクトの描画
void Actor::draw_transparent() const {}

// GUIの描画
void Actor::draw_gui() const {}

// 衝突リアクション
void Actor::react(Actor&) {}

// メッセージ処理
void Actor::handle_message(const std::string& message, void* param) {}

// 衝突判定
void Actor::collide(Actor& other) {
	// どちらのアクターも衝突判定が有効か？
	if (enable_collider_ && other.enable_collider_) {
		// 衝突判定をする
		if (is_collide(other)) {
			// 衝突した場合は、お互いに衝突リアクションをする
			react(other);
			other.react(*this);
		}
	}
}

// 死亡する
void Actor::die() {
	dead_ = true;
}

// 衝突しているか？
bool Actor::is_collide(const Actor& other) const {
	return collider().intersects(other.collider());
}

// 死亡しているか？
bool Actor::is_dead() const {
	return dead_;
}

// 名前を取得
const std::string& Actor::name() const {
	return name_;
}

// タグを取得
const std::string& Actor::tag() const {
	return tag_;
}

// トランスフォームを取得　const版！
const GStransform& Actor::transform() const {
	return transform_;
}

// トランスフォームを取得
GStransform& Actor::transform() {
	return transform_;
}

// 移動量を取得
GSvector3 Actor::velocity() const {
	return velocity_;
}

// 衝突判定データを取得
BoundingSphere Actor::collider() const {
	return collider_.transform(transform_.localToWorldMatrix());
}

// Thetaを取得
float Actor::theta() const {
	return theta_;
}

// HPを取得
int Actor::hp() const {
	return hp_;
}

// カウンターを取得
int   Actor::count() const {
	return count_;
}

// フェーズを取得
int   Actor::phase() const {
	return phase_;
}

// カウンター２を取得
int   Actor::counta() const {
	return counta_;
}

// カウンター３を取得
int   Actor::countb() const {
	return countb_;
}

// カウンター４を取得
int   Actor::countc() const {
	return countc_;
}

// floatカウンターを取得
float Actor::countf() const {
	return countf_;
}

// floatカウンター２を取得
float Actor::countfa() const {
	return countfa_;
}

// floatカウンター３を取得
float Actor::countfb() const {
	return countfb_;
}

// floatカウンター４を取得
float Actor::countfc() const {
	return countfc_;
}

TweenUnit& Actor::move_to(const GSvector3& to, float duration) {
	// 現在の場所から指定された場所まで、Tweenで移動する
	return Tween::vector3(transform_.position(), to, duration,
		[=](GSvector3 pos) {transform_.position(pos); });
}

// エフェクシアのエフェクトを再生する
void Actor::play_effect(GSuint id, const GSvector3& local_position, const GSvector3& local_rotation, const GSvector3& local_scale) {
	// 指定されたTranslate, Rotation, Scaleの行列を作成する
	GSmatrix4 local_matrix = GSmatrix4::TRS(local_position, GSquaternion::euler(local_rotation), local_scale);
	// ワールド空間に変換する
	GSmatrix4 world_matrix = local_matrix * transform_.localToWorldMatrix();
	// エフェクトを再生する
	gsPlayEffectEx(id, &world_matrix);
}