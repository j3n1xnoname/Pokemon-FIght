#include "Attack.hpp"

Attack::Attack(int ATTACK_DAMAGE, std::string ATTACK_NAME, int WASTE_MANA)
{
    attackDamage = ATTACK_DAMAGE;
    attackName = ATTACK_NAME;
    wasteMana = WASTE_MANA;
}

Attack::~Attack()
{
    attackDamage = 0;
    attackName = "";
    wasteMana = 0;
}
