#ifndef ACTOR_H_
#define ACTOR_H_

#include <gslib.h>
#include <GStransform.h>
#include <string>
#include "Collision/BoundingSphere.h"

#include "Tween/Tween.h"

class IWorld;				// ワールド抽象インターフェースの前方宣言

/// アクタークラス
class Actor {
public:
	// コンストラクタ
	Actor() = default;
	// 仮想デストラクタ
	virtual~Actor() = default;
	// 更新
	virtual void update(float delta_time);
	// 遅延更新
	virtual void late_update(float delta_time);
	// 描画
	virtual void draw() const;
	// 半透明の描画
	virtual void draw_transparent() const;
	// GUIの描画
	virtual void draw_gui() const;
	// 衝突リアクション
	virtual void react(Actor& other);
	// メッセージ処理
	virtual void handle_message(const std::string& message, void* param);
	// 衝突判定
	void collide(Actor& other);
	// 死亡する
	void die();
	// 衝突しているか？
	bool is_collide(const Actor& other) const;
	// 死亡しているか？
	bool is_dead() const;
	// 名前を取得
	const std::string& name() const;
	// タグ名を取得
	const std::string& tag() const;
	// トランスフォームを取得　const版！
	const GStransform& transform() const;
	// トランスフォームを取得
	GStransform& transform();
	// 移動量を取得
	GSvector3 velocity() const;
	// 衝突判定データを取得
	BoundingSphere collider() const;

	// Thetaを取得
	float theta() const;
	// HPを取得
	int   hp() const;
	// カウンターを取得
	int   count() const;
	// フェーズを取得
	int   phase() const;
	// カウンター２を取得
	int   counta() const;
	// カウンター３を取得
	int   countb() const;
	// カウンター４を取得
	int   countc() const;
	// floatカウンターを取得
	float countf() const;
	// floatカウンター２を取得
	float countfa() const;
	// floatカウンター３を取得
	float countfb() const;
	// floatカウンター４を取得
	float countfc() const;


	// 指定された場所までTweenで移動する
	TweenUnit& move_to(const GSvector3& to, float duration);

	// コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	// ワールド
	IWorld*			world_{ nullptr };
	// タグ名
	std::string		tag_;
	// 名前
	std::string		name_;
	// トランスフォーム
	GStransform		transform_;
	// 移動量
	GSvector3		velocity_{ 0.0f,0.0f,0.0f };
	// 衝突判定が有効か？
	bool			enable_collider_{ true };
	// 衝突判定
	BoundingSphere	collider_;
	// 死亡フラグ
	bool			dead_{ false };

	// エフェクシアのエフェクトを再生する
	void            play_effect(GSuint id, const GSvector3& local_position, const GSvector3& local_rotation, const GSvector3& local_scale);

	// Theta
	float           theta_{ 0.0f };
	// HP
	int				hp_{ 0 };
	// スタミナ
	int				stamina_{ 0 };
	// カウンター
	int                count_{ 0 };
	// フェーズ
	int                phase_{ 0 };
	// カウンター２
	int                counta_{ 0 };
	// カウンター３
	int                countb_{ 0 };
	// カウンター４
	int                countc_{ 0 };
	// カウンターfloat
	float              countf_{ 0.0f};
	// カウンターfloat２
	float              countfa_{ 0.0f };
	// カウンターfloat３
	float              countfb_{ 0.0f };
	// カウンターfloat４
	float              countfc_{ 0.0f };
};

#endif