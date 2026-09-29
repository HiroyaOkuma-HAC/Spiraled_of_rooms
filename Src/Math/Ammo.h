#ifndef AMMO_H_
#define AMMO_H_

// ’e–ò”ƒNƒ‰ƒX
class Ammo {
public:
    // ƒRƒ“ƒXƒgƒ‰ƒNƒ^
    Ammo(int ammo = 0);
    // ’e–ò”‚Ì‰Šú‰»
    void initialize(int ammo = 0);
    // ’e–ò”‚Ì‰ÁZ
    void add(int ammo);
    // ’e–ò”‚Ì•`‰æ
    void draw() const;
    // ’e–ò”‚Ìæ“¾
    int get() const;
    // ’e–ò”‚ÌƒNƒŠƒA
    void clear();

private:
    // ’e–ò”
    int ammo_;
};

#endif

