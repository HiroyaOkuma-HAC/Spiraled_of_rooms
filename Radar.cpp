#include "Radar.h"
#include "World/IWorld.h"
#include "Actor/Actor.h"
#include "Assets.h"

// レーダーの画面上の表示位置
const GSvector2 RadarPosition{ 16.0f, 20.0f };
// レーダーの画像サイズ
const float RadarSize{ 256.0f };
// レーダーの範囲（プレイヤー位置から-20~20の範囲）
const float RadarRange{ 20.0f };
// グラウンドのサイズ
const float GroundSize{ 21.0f };

// コンストラクタ
Radar::Radar(IWorld* world) {
	world_ = world;
	tag_ = "RadarTag";
	name_ = "Radar";
	enable_collider_ = false; // 衝突判定をしない
}

// 描画
void Radar::draw_gui() const {
	// レーダーの背景を描画
	static const GScolor4 back_color{ 1.0f,1.0f,1.0f,0.5f };
	gsDrawSprite2D(Texture_Radar, &RadarPosition, NULL, NULL, &back_color, NULL, 0.0f);
	// プレイヤーの検索
	Actor* player = world_->find_actor("Player");
	// レーダーの座標系に変換する行列
	GSmatrix4 radar_matrix{ GS_MATRIX4_IDENTITY };
	if (player != nullptr) {
		// プレイヤーの逆行列を取得（プレーヤーの位置を基準とする座標を計算するため）
		radar_matrix = player->transform().worldToLocalMatrix();
	}
	// レーダーの上方向をプレーヤーの進行方向に合わせる
	radar_matrix *= GSmatrix4::rotateY(180.0f);
	// グラウンドの外枠を表示
	for (float i = -GroundSize; i <= GroundSize; i += 1.0f) {
		// 枠の点の色（水色）
		static const GScolor frame_color{ 0.0f,1.0f,1.0f,1.0f };
		// 4辺の点を描画
		draw_point(radar_matrix.multiplyPoint(GSvector3{ i,0.0f,-GroundSize }), frame_color);
		draw_point(radar_matrix.multiplyPoint(GSvector3{ i,0.0f, GroundSize }), frame_color);
		draw_point(radar_matrix.multiplyPoint(GSvector3{ -GroundSize,0.0f,i }), frame_color);
		draw_point(radar_matrix.multiplyPoint(GSvector3{ GroundSize,0.0f,i }), frame_color);
	}
	// 敵の位置を描画
	for (Actor* enemy : world_->find_actor_with_tag("EnemyTag")) {
		// 敵の点の色（緑）
		static const GScolor enemy_color{ 0.0f,1.0f,0.0f,1.0f };
		 // 敵の点を描画
		draw_point(radar_matrix.multiplyPoint(enemy->transform().position()), enemy_color);
	}
	// プレーヤーの点の色（赤）
	static const GScolor player_color{ 1.0f,0.0f,0.0f,1.0f };
	// プレーヤーの位置を描画（プレーヤーの点は原点）
	draw_point(GSvector3{ 0.0f,0.0f,0.0f }, player_color);
}

// レーダー内の点を描画
void Radar::draw_point(const GSvector3& position, const GScolor& color) const {
	// レーダーの2D座標にして正規化する（表示範囲を-1.0~1.0の座標に変換）
	GSvector2 nolmalized_positioin{ position.x / RadarRange, position.z / RadarRange };
	// レーダーの範囲外は描画しない
	if (nolmalized_positioin.x < -1.0f || nolmalized_positioin.x > 1.0f
		|| nolmalized_positioin.y < -1.0f || nolmalized_positioin.y > 1.0f) {
		return;
	}
	// レーダーの画像サイズの半分（１２８ピクセル）
	const float RadarHalfSize{ RadarSize / 2.0f };
	// レーダーの画像サイズにあわせて拡大する（-128~128）
	GSvector2 point_position = nolmalized_positioin * RadarHalfSize;
	// 原点（プレイヤーの位置）がレーダーの中央になるように補正（０～２５６）
	point_position += GSvector2{ RadarHalfSize, RadarHalfSize };
	// レーダーの表示位置にあわせて平行移動
	point_position += RadarPosition;
	// 点の画像の中心位置を補正する（画像の中心が原点になるように）
	static const GSvector2 center{ 4.0f,4.0f };
	// 点の画像を描画
	gsDrawSprite2D(Texture_RadarPoint, &point_position, NULL, &center, &color, NULL, 0.0f);
}