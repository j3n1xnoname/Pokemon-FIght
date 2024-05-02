#include "Attack.hpp"

Attack::Attack(std::string ATTACK_NAME, std::string ATTACK_DESCRIPTION, int ATTACK_DAMAGE, int WASTE_MANA)
{
    attackName = ATTACK_NAME;
    attackDescription = ATTACK_DESCRIPTION;
    attackDamage = ATTACK_DAMAGE;
    wasteMana = WASTE_MANA;
}

Attack::Attack()
{
    attackDamage = 0;
    attackName = "";
    wasteMana = 0;
}

Attack::~Attack()
{
    attackDamage = 0;
    attackName = "";
    wasteMana = 0;
    if (!attackDamageText.isEmpty())
        attackDamageText.free();
    if (!attackNameText.isEmpty())
        attackNameText.free();
    if (!wasteManaText.isEmpty())
        wasteManaText.free();
    if (!attackDescriptionText.isEmpty())
        attackDescriptionText.free();
}
