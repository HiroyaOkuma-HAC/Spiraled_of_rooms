#ifndef LOAD_TUTRIAL_H_
#define LOAD_TUTRIAL_H_

#include "IScene.h"

// ロード画面（ゲームプレイシーンで使う素材をロードする）
class LoadTutrial : public IScene {
public:
	// 開始
	virtual void start() override;
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 終了しているか？
	virtual bool is_end() const override;
	// 次のシーンを返す
	virtual std::string next() const override;
	// 終了
	virtual void end() override;
private:
	// 素材の読み込み
	void load();
	// 終了フラグ
	bool is_end_;
	// 読み込んだアセット数
	int loaded_count;
};

#endif