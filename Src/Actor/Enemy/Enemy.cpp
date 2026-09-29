#include "Enemy.h"
#include "Actor/Player.h"

#include "World/IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"

#include "Collision/AttackCollider.h"
#include "World/World.h"

#include "Actor/Explosion.h"

#include "Actor/Item/DroppedCoin.h"

enum {                      // 敵のモーション番号
    Motion_Idle = 0,	        // アイドル
    Motion_Walk = 1,	        // 歩き
    Motion_Guard = 10,	    // ガード
    Motion_Hit = 11,    // プレイヤーからダメージをうけて耐えたとき
    Motion_Damage = 12,	// ダメージ中
    Motion_Down = 14          // ダウン
};

// 攻撃判定の距離
const float AttackDistance{ 1.6f };
// 移動判定の距離
const float WalkDistance{ 10000.0f };
// 移動スピード
const float WalkSpeed{ 0.025f };

/// 追加！
// 自分の高さ
const float EnemyHeight{ 1.0f };
// 衝突判定用の半径
const float EnemyRadius{ 0.4f };
// 足元のオフセット
const float FootOffset{ 0.1f };
// 重力
const float Gravity{ -0.016f };
///



// コンストラクタ
Enemy::Enemy(IWorld* world, const GSvector3& position) :
    // mesh_{ 1, MotionIdle, true },変更！↓
    mesh_{ Mesh_Enemy, Motion_Idle, true },

    motion_{ Motion_Idle },
    motion_loop_{ true },
    state_{ State::Idle },
    state_timer_{ 0.0f },
    //player_{ player } {
    player_{ nullptr },
    health_{ 3 } {
    // ワールドの設定
    world_ = world;
    // タグ名の設定
    tag_ = "EnemyTag";
    // 名前の設定
    name_ = "Enemy";
    // 衝突判定球の設定
    collider_ = BoundingSphere{ EnemyRadius, GSvector3{0.0f, EnemyHeight, 0.0f} };


    // 座標の初期化
    transform_.position(position);
    // ワールド変換行列の初期化
    mesh_.transform(transform_.localToWorldMatrix());
    // ランダムなむきでスポーン
    transform_.rotate(0.0f, 360.0f-target_signed_angle(), 0.0f);

}



// 更新
void Enemy::update(float delta_time) {

    if (health_ > 3) health_ = 3;

    // プレーヤーを検索する
    player_ = world_->find_actor("Player");

    // 状態の更新
    update_state(delta_time);

    // 重力を更新
    velocity_.y += Gravity * delta_time;
    // 重力を加える
    transform_.translate(0.0f, velocity_.y, 0.0f);
    // フィールドとの衝突判定
    collide_field();

    // モーションを変更
    mesh_.change_motion(motion_, motion_loop_);
    // メッシュを更新
    mesh_.update(delta_time);
    // 行列を設定
    mesh_.transform(transform_.localToWorldMatrix());


    /// 走りおわったら
    if (transform_.position().z < -22 || transform_.position().x < -22 || 
            transform_.position().z > 22 || transform_.position().x > 22) {
        if (state_ == State::Walk ) {
            change_state(State::Deleted, Motion_Guard, false);
        }
       
    }

}

// 描画
void Enemy::draw() const {
    // 色を変える
    //GScolor4 meshcolor = { 0.4f,0.9f,0.4f,1.0f };
    if (state_ == State::Idle || state_ == State::Deleted) {
        if ((int)state_timer_ % 5) {
            GScolor4 meshcolor = { 1.0f,1.0f,1.0f,0.0f };
            glColor4fv(meshcolor);
        }
        else {
            GScolor4 meshcolor = { 1.0f,1.0f,1.0f,1.0f };
            glColor4fv(meshcolor);
        }
    }


    // メッシュの描画
    mesh_.draw();

    // 衝突判定のデバッグ表示
    ///collider().draw();

    GScolor4 meshcolor = { 1.0f,1.0f,1.0f,1.0f };
    glColor4fv(meshcolor);
}

// 衝突処理
void Enemy::react(Actor& other) {
    // ダメージ中またはダウン中の場合は何もしない
    if (state_ == State::Damage || state_ == State::Down || state_ == State::Deleted || state_ == State::Hit) return;
    
 

    // プレーヤーの弾に衝突したら
    if (other.tag() == "PlayerBulletTag") {
        // 爆発エフェクトを生成する
        GSvector3 Effect_position = transform_.position();
        Effect_position.y += EnemyRadius * 3;
        world_->add_actor(new Explosion{ world_, Effect_position });
        gsPlaySE(Se_BISI);
        // 体力を減らす
        health_ -= 1;
        if (health_ >= 1) {
            // うわっとなる状態に遷移
            change_state(State::Hit, Motion_Hit);
            return;
        }
        else if (health_ <= 0) {

            GSvector3 DropCoinPos = transform_.position();
            world_->add_actor(new DroppedCoin{ world_, DropCoinPos, gsRand(1,20)});

            // ダメージ状態に遷移
            change_state(State::Damage, Motion_Damage, false);
            // スコアを加算
            //world_->score().add(1);
            world_->score().add(50 * world_->streak().get());
            // やられた効果音を再生
            gsPlaySE(Se_EnemyDamage);
            return;
        }
    }

  
    



    // プレーヤーに衝突したら
    if (world_->Down_now_ == false && other.tag() == "PlayerTag" ){
        // ダメージ状態に遷移
        change_state(State::Damage, Motion_Damage, false);
        // やられた効果音を再生
        gsPlaySE(Se_EnemyDamage);
    }

}

// 状態の更新
void Enemy::update_state(float delta_time) {
    // 各状態に分岐する
    switch (state_) {
    case State::Idle:   idle(delta_time);   break;
    case State::Walk:   walk(delta_time);   break;
    case State::Damage: damage(delta_time); break;
    case State::Down:  down(delta_time);   break;
    case State::Deleted: deleted(delta_time); break;
    case State::Hit: hit(delta_time); break;
    }
    // 状態タイマの更新
    state_timer_ += delta_time;
}

// 状態の変更
void Enemy::change_state(State state, GSuint motion, bool loop) {
    // モーション番号の更新
    motion_ = motion;
    // モーションのループ指定
    motion_loop_ = loop;
    // 状態の更新
    state_ = state;
    // 状態タイマの初期化
    state_timer_ = 0.0f;
}

// アイドル状態
void Enemy::idle(float delta_time) {
   
        // 振り向きモーションをしながらターゲット方向を向く
        float angle = (target_signed_angle() >= 0.0f) ? 6.0f : -6.0f;
        transform_.rotate(0.0f, angle * 1.0f, 0.0f);
   // LookAt関数を使うとかんたん！ 

    
    if (state_timer_ > 60.0f) {
        // 移動中に遷移する！
        change_state(State::Walk, Motion_Walk);
    }
}

// 移動中
void Enemy::walk(float delta_time) {


    // 前進する（ローカル座標基準）
    transform_.translate(0.0f, 0.0f, WalkSpeed * delta_time);
   
}



// 攻撃中
void Enemy::attack(float delta_time) {
    if (state_timer_ >= mesh_.motion_end_time()) {
        // 攻撃モーションが終了したらアイドル中に遷移
        idle(delta_time);
    }
}

// ダメージ中
void Enemy::damage(float delta_time) {
    world_->Down_now_ = true;
    // タグ名の設定
    tag_ = "NoneTag"; // タグを変えて当たり判定をけしているよ！
    if (state_timer_ > mesh_.motion_end_time()) {
        change_state(State::Down, Motion_Down, false);
    }
}

// ダウン中
void Enemy::down(float delta_time) {
    // 1秒たったら
    if (state_timer_ > 60.0f) {
        die();
    }
}

// 消滅中
void Enemy::deleted(float delta_time) {
    // 1秒たったら消滅
    if (state_timer_ > 60.0f) {
        die();
    }
}

// プレイヤーからダメージをうけて耐えた状態
void Enemy::hit(float delta_time) {
    // タグ名の設定
    tag_ = "NoneTag"; // タグを変えて当たり判定をけしているよ！
    if (state_timer_ > mesh_.motion_end_time() / 2) {
        change_state(State::Walk, Motion_Walk);
        tag_ = "EnemyTag";
    }
}




// 攻撃判定
bool Enemy::is_attack() const {
    // 攻撃距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が20.0度以下か？
    return (target_distance() <= AttackDistance) && (target_angle() <= 20.0f);
}

// 移動判定
bool Enemy::is_walk() const {
    // 移動距離内かつ前方向と前向き方向のベクトルとターゲット方向のベクトルの角度差が100.0度以下か？
    return (target_distance() <= WalkDistance) && (target_angle() <= 360.0f);
}

// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号付き）
float Enemy::target_signed_angle() const {

    // ターゲットがいなければ0を返す
    if (player_ == nullptr) return 0.0f;

    // ターゲット方向のベクトルを求める
    GSvector3 to_target = player_->transform().position() - transform_.position();
    // 前向き方向のベクトルを取得
    GSvector3 forward = transform_.forward();
    // ベクトルのy成分を無効にする
    forward.y = 0.0f;
    to_target.y = 0.0f;
    // 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める
    return GSvector3::signedAngle(forward, to_target);
}

// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号なし）
float Enemy::target_angle() const {
    return std::abs(target_signed_angle());
}

// ターゲットとの距離を求める
float Enemy::target_distance() const {

    // ターゲットがいなければ最大距離を返す
    if (player_ == nullptr) return FLT_MAX; // float型の最大値

    // ターゲットとの距離を計算する
    return GSvector3::distance(player_->transform().position(), transform_.position());
}

/// ここから↓はフィールド系のクラスの追加！

// フィールドとの衝突判定
void Enemy::collide_field() {
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
    line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
    GSvector3 intersect;  // 地面との交点
    if (world_->field()->collide(line, &intersect)) {
        // 交差した点からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }
}

// アクターとの衝突処理
void Enemy::collide_actor(Actor& other) {
    if (state_ == State::Damage || state_ == State::Down || state_ == State::Deleted) return;
    // ｙ座標を除く座標を求める
    GSvector3 position = transform_.position();
    position.y = 0.0f;
    GSvector3 target = other.transform().position();
    target.y = 0.0f;
    // 相手との距離
    float distance = GSvector3::distance(position, target);
    // 衝突判定球の半径同士を加えた長さを求める
    float length = collider_.radius + other.collider().radius;
    // 衝突判定球の重なっている長さを求める
    float overlap = length - distance;
    // 重なっている部分の半分の距離だけ離れる移動量を求める
    GSvector3 v = (position - target).getNormalized() * overlap * 0.1f;
    transform_.translate(v, GStransform::Space::World);
    // フィールドとの衝突判定
    collide_field();
}

// 攻撃判定を生成する
void Enemy::generate_attack_collider() {
    // 攻撃判定を出現させる場所の距離
    const float AttackColliderDistance{ 0.5f };
    // 攻撃判定の半径
    const float AttackColliderRadius{ 0.3f };
    // 攻撃判定を出す場所の高さ
    const float AttackColliderHeight{ 1.0f };

    // 攻撃判定が有効になるまでの遅延時間
    const float AttackColliderDelay{ 15.0f };
    // 攻撃判定の寿命
    const float AttackColliderLifeSpan{ 5.0f };

    // 衝突判定を出現させる座標を求める（前方の位置）
    GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // 高さの補正（足元からの高さ）
    position.y += AttackColliderHeight;
    // 衝突判定用の球を作成
    BoundingSphere collider{ AttackColliderRadius, position };
    // 衝突判定を出現させる
    world_->add_actor(new AttackCollider{ world_, collider,
        "EnemyAttackTag", "EnemyAttack", tag_, AttackColliderLifeSpan, AttackColliderDelay });
}


