/*

#include "EnemyProximity.h"

#include "World/IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"

#include "Collision/AttackCollider.h"
//#include "RailGunCharge.h"
//#include "ZonbieDamageNumber.h"
#include "World/World.h"
//#include "Math/Combo.h"
#include "Math/Score.h"

enum {                      // 敵のモーション番号
    MotionIdle = 0,	        // アイドル
    MotionWalk = 1,	        // 歩き
    MotionTurnLeft = 2,	    // 左に振り向く
    MotionTurnRight = 3,	// 右に振り向く
    MotionDamage = 4,	    // ダメージ
    MotionAttack = 5,	    // 攻撃

    /// 追加！
    MotionDown = 6          // ダウン
    ///
};

// 振り向き判定の距離
const float TurnDistance{ 1.5f };
// 攻撃判定の距離
const float AttackDistance{ 1.6f };
// 移動判定の距離
const float WalkDistance{ 10.0f };
// 移動スピード
const float WalkSpeed{ 0.025f };
// 振り向く角度
const float TurnAngle{ 2.5f };

/// 追加！
// 自分の高さ
const float EnemyHeight{ 1.0f };
// 衝突判定用の半径
const float EnemyRadius{ 0.6f };
// 足元のオフセット
const float FootOffset{ 0.1f };
// 重力
const float Gravity{ -0.016f };
///

// コンストラクタ
EnemyProximity::EnemyProximity(IWorld* world, const GSvector3& position) :
    // mesh_{ 1, MotionIdle, true },変更！↓
    mesh_{ Mesh_Enemy, MotionIdle, true },

    motion_{ MotionIdle },
    motion_loop_{ true },
    state_{ State::Idle },
    state_timer_{ 0.0f },
    //player_{ player } {
    player_{ nullptr },
    health_{ 240 } {
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
    transform_.rotate(0.0f, gsRandf(0.0f, 360.0f), 0.0f);
}



// 更新
void EnemyProximity::update(float delta_time) {

    if (health_ > 250) health_ = 250;

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
}

// 描画
void EnemyProximity::draw() const {
    // 色を変える
    GScolor4 meshcolor = { 0.4f,0.9f,0.4f,1.0f };
    glColor4fv(meshcolor);

    // メッシュの描画
    mesh_.draw();

    // 色をもどす
    meshcolor = { 1.0f,1.0f,1.0f,1.0f };
    glColor4fv(meshcolor);


    // 衝突判定のデバッグ表示
    ///collider().draw();
}

// 衝突処理
void EnemyProximity::react(Actor& other) {
    // ダメージ中またはダウン中の場合は何もしない
    if (state_ == State::Damage || state_ == State::Down) return;

    // エネミーヒールゾーンに当たったら回復する
        // 衝突したら
    if (other.tag() == "EnemyHealZoneTag") {
        // 体力を加算
        health_++;
        if (health_ <= 0) {
            // 残りの体力がなければダウン状態に遷移
            change_state(State::Down, MotionDown, false);
        }
        return;
    }

    // プレーヤーの弾に衝突したら
    if (other.tag() == "PlayerBulletTag") {
        // 体力を減らす
        damage_ = 50;
        health_ -= damage_;
        world_->score().add((damage_ / 2) * world_->streak().get());

        /// 与えたダメージを描画する処理！ //////
        GSvector3 NumberPositionSet = transform_.position();
        NumberPositionSet.y += gsRandf(2.0f, 3.0f);
        NumberPositionSet.x += gsRandf(-1.0f, 1.0f);
        const GSvector3 NumberPosition = NumberPositionSet;
       // world_->add_actor(new ZonbieDamageNumber(world_, NumberPosition, damage_));
        /// /////////////////////////////////////////////
        //const GSvector3 NumberPosition = transform_.position();

        // レールガンパワーをチャージ
        //world_->railguncharge().add(6 + world_->railguncharge().get() / 2);
        if (health_ <= 0) {
            world_->streak_limit().initialize();
            world_->combo().add(1);
            world_->score().add(50 * world_->streak().get());

            // 残りの体力がなければダウン状態に遷移
            change_state(State::Down, MotionDown, false);
        }
        else {
            // 弾の進行方向にノックバックする移動量を求める
            velocity_ = other.velocity().getNormalized() * 0.5f;
            // ダメージ状態に遷移
            change_state(State::Damage, MotionDamage, false);
        }
        return;
    }

    // プレーヤーの弾に衝突したら
    if (other.tag() == "RailGunTag") {
        // 体力を減らす
       // damage_ = world_->railguncharge().get();
        health_ -= damage_;
        world_->score().add((damage_ / 2) * world_->streak().get());

        /// 与えたダメージを描画する処理！ //////
        GSvector3 NumberPositionSet = transform_.position();
        NumberPositionSet.y += gsRandf(2.0f, 3.0f);
        NumberPositionSet.x += gsRandf(-1.0f, 1.0f);
        const GSvector3 NumberPosition = NumberPositionSet;
        //world_->add_actor(new ZonbieDamageNumber(world_, NumberPosition, damage_));
        /// /////////////////////////////////////////////

        if (health_ <= 0) {
            world_->streak_limit().initialize();
            world_->combo().add(1);
            world_->score().add(50 * world_->streak().get());

            // 残りの体力がなければダウン状態に遷移
            change_state(State::Down, MotionDown, false);
        }
        else {
            // 弾の進行方向にノックバックする移動量を求める
            velocity_ = other.velocity().getNormalized() * 0.5f;
            // ダメージ状態に遷移
            change_state(State::Damage, MotionDamage, false);
        }
        return;
    }

    // ダンスフロアに当たったら
    if (other.tag() == "MyPlayerBulletTag") {
        change_state(State::Dance, MotionTurnLeft, true);


        return;
    }

    // プレーヤーまたは敵に衝突したら
    if (other.tag() == "PlayerTag" || other.tag() == "EnemyTag") {
        collide_actor(other);
    }

    
}

// 状態の更新
void EnemyProximity::update_state(float delta_time) {
    // 各状態に分岐する
    switch (state_) {
    case State::Idle:   idle(delta_time);   break;
    case State::Attack: attack(delta_time); break;
    case State::Walk:   walk(delta_time);   break;
    case State::Damage: damage(delta_time); break;
    case State::Turn:   turn(delta_time);   break;
    case State::Dance:   dance(delta_time);   break;


    case State::Down:   down(delta_time);   break;

    }
    // 状態タイマの更新
    state_timer_ += delta_time;
}

// 状態の変更
void EnemyProximity::change_state(State state, GSuint motion, bool loop) {
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
void EnemyProximity::idle(float delta_time) {
    // 攻撃するか？
    if (is_attack()) {
        // 攻撃判定を生成
        generate_attack_collider();
        // 攻撃状態に遷移
        change_state(State::Attack, MotionAttack);
        return;
    }
    // 歩くか？
    if (is_walk()) {
        // 歩き状態に遷移
        change_state(State::Walk, MotionWalk);
        return;
    }
    // 振り向くか？
    if (is_turn()) {
        // 左に振り向くか？右に振り向くか？
        GSuint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
        // 振り向き状態に遷移
        change_state(State::Turn, motion);
        return;
    }
    // 何もなければ、アイドル中のまま
    change_state(State::Idle, MotionIdle);
}

// 移動中
void EnemyProximity::walk(float delta_time) {
    // ターゲット方向の角度を求める
    float angle = target_signed_angle();
    // 角度差が大きい場合は、少しずつ向きを変えるように角度を制限する
    angle = CLAMP(angle, -TurnAngle, TurnAngle) * delta_time;
    // 向きを変える
    transform_.rotate(0.0f, angle, 0.0f);


    // 前進する（ローカル座標基準）
    transform_.translate(0.0f, 0.0f, WalkSpeed * delta_time);
    // 攻撃するか？
    if (is_attack()) {
        // 攻撃判定を生成
        generate_attack_collider();
        // 攻撃状態に遷移する
        change_state(State::Attack, MotionAttack);
    }
}

// 振り向き中
void EnemyProximity::turn(float delta_time) {
    if (state_timer_ >= mesh_.motion_end_time()) {
        // 振り向きモーションが終了したらアイドル中に遷移
        idle(delta_time);
    }
    else {
        // 振り向きモーションをしながらターゲット方向を向く
        float angle = (target_signed_angle() >= 0.0f) ? TurnAngle : -TurnAngle;
        transform_.rotate(0.0f, angle * delta_time, 0.0f);
    }
}

// 攻撃中
void EnemyProximity::attack(float delta_time) {
    if (state_timer_ >= mesh_.motion_end_time()) {
        // 攻撃モーションが終了したらアイドル中に遷移
        idle(delta_time);
    }
}

// ダメージ中
void EnemyProximity::damage(float delta_time) {
    if (state_timer_ < mesh_.motion_end_time() / 2) {
        // ダメージモーション中は何もしない
        // ↓　変更して要素を追加！　↓
            // ノックバックする
        transform_.translate(velocity_ * delta_time, GStransform::Space::World);
        velocity_ -= GSvector3{ velocity_.x, 0.0f, velocity_.z } *0.5f * delta_time;

        return;
    }
    // ダメージモーション終了後、ターゲット方向との角度差が90.0度以上（背後）なら振り向き中に遷移
    if (target_angle() >= 90.0f) {
        // 振り向き中に遷移
        GSuint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
        change_state(State::Turn, motion);
    }
    else {
        // アイドル中に遷移
        idle(delta_time);
    }
}

// ダウン中
void EnemyProximity::down(float delta_time) {
    if (state_timer_ >= mesh_.motion_end_time()) {
        // ダウンモーションが終了したら死亡 
        die();
    }
}

// ダンス中
void EnemyProximity::dance(float delta_time) {
    counta_ += 2.0f;
    transform_.rotate(0.0f, delta_time * 6, 0.0f);
    // エフェクトを再生（生成）する
    if (counta_ >= 180.0f) {
        // 振り向き中に遷移
        GSuint motion = (target_signed_angle() >= 0.0f) ? MotionTurnLeft : MotionTurnRight;
        change_state(State::Turn, motion);
    }
    world_->score().add(1 * world_->streak().get());
}

// 振り向き判定
bool EnemyProximity::is_turn() const {
    // 振り向き距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が90.0度以上か？
    return (target_distance() <= TurnDistance) && (target_angle() >= 90.0f);
}

// 攻撃判定
bool EnemyProximity::is_attack() const {
    // 攻撃距離内かつ前向き方向のベクトルとターゲット方向のベクトルの角度差が20.0度以下か？
    return (target_distance() <= AttackDistance) && (target_angle() <= 20.0f);
}

// 移動判定
bool EnemyProximity::is_walk() const {
    // 移動距離内かつ前方向と前向き方向のベクトルとターゲット方向のベクトルの角度差が100.0度以下か？
    return (target_distance() <= WalkDistance) && (target_angle() <= 100.0f);
}

// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号付き）
float EnemyProximity::target_signed_angle() const {

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
float EnemyProximity::target_angle() const {
    return std::abs(target_signed_angle());
}

// ターゲットとの距離を求める
float EnemyProximity::target_distance() const {

    // ターゲットがいなければ最大距離を返す
    if (player_ == nullptr) return FLT_MAX; // float型の最大値

    // ターゲットとの距離を計算する
    return GSvector3::distance(player_->transform().position(), transform_.position());
}

/// ここから↓はフィールド系のクラスの追加！

// フィールドとの衝突判定
void EnemyProximity::collide_field() {
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
void EnemyProximity::collide_actor(Actor& other) {
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
void EnemyProximity::generate_attack_collider() {
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


*/