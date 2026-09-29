#include "Rendering/ResultTX.h"
#include <sstream>
#include <iomanip>

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
ResultTX::ResultTX(GSuint texture, int width, int height) :
    texture_{ texture }, width_{ width }, height_{ height } {
}

// •`‰æ (‰E‹l‚ß)
void ResultTX::draw(const GSvector2& position, int num, int digit, char fill, const GScolor& color) const {
    std::stringstream ss;
    ss << std::setw(digit) << std::setfill(fill) << num;
    draw(position, ss.str(), color);
}

// •`‰æ (¶‹l‚ß)
void ResultTX::draw(const GSvector2& position, int num, const GScolor& color) const {
    draw(position, std::to_string(num), color);
}

// •`‰æ
void ResultTX::draw(const GSvector2& position, const std::string& num, const GScolor& color) const {
    // ”Žš‚ð1•¶Žš‚¸‚Â•`‰æ‚·‚é
    for (int i = 0; i < (int)num.size(); ++i) {
        if (num[i] == ' ') continue; // ‹ó”’•¶Žš‚ÍƒXƒLƒbƒv
        // charŒ^‚ðintŒ^‚É•ÏŠ·
        //int n = num[i] - '0';
        const int n = (num[i] != '.') ? num[i] - '0' : 10; // ¬”“_‚Í9‚ÌŽŸ‚É‚ ‚é‚±‚Æ‚ð‘O’ñ!!
        // ”Žš‚É‘Î‰ž‚·‚é‰æ‘œ‚ðØ‚èo‚·‚½‚ß‚Ì‹éŒ`‚ðŒvŽZ‚·‚é(”’lƒtƒHƒ“ƒg‰æ‘œ“à‚ÌˆÊ’uj
        GSrect rect(n * width_, 0.0f, (n * width_) + width_, height_);
        // ”Žš‚ð•`‰æ‚·‚éÀ•W‚ðŒvŽZ
        GSvector2 pos{ position.x + i * width_ , position.y };
        // ‘å‚«‚³
        GSvector2 scale{ 1.0f,1.0f };
        // ”Žš‚ð‚PŒ…•`‰æ
        gsDrawSprite2D(texture_, &pos, &rect, NULL, &color, &scale, 0);
    }
}

