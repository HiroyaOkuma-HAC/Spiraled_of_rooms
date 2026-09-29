#include "Upgrade.h"
#include "Rendering/NumberTexture.h"
#include "Assets.h"
#include <gslib.h>
#include <fstream>


// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Upgrade::Upgrade() {
	// ‰Šú‰»
	initialize();
}

// ‰Šú‰»
void Upgrade::initialize() {
}


// XV
void Upgrade::update(float delta_time) {
}

// •`‰æ
void Upgrade::draw() const {
	// ”wŒi‚Ì•`‰æ
	draw_background();
}


// ”wŒi‚Ì•`‰æ
void Upgrade::draw_background()const {
	// ”¼“§–¾‚ÌÂ‚¢”wŒi‚ğ•\¦
	static const GScolor bg_color{ 1.0f,1.0f,1.0f,0.5f, };
	static const GSvector2 bg_position{ 20.0f,100.0f };
	gsDrawSprite2D(Texture_BlueBack, &bg_position, NULL, NULL, &bg_color, NULL, 0.0f);
}
