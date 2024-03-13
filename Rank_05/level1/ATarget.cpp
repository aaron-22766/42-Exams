
#include "ATarget.hpp"
#include "ASpell.hpp"
#include <iostream>

ATarget::ATarget(const std::string &type) : _type(type) {}

ATarget::~ATarget(void) {}

const std::string &ATarget::getType(void) const {
    return (_type);
}

void ATarget::getHitBySpell(const ASpell &spell) const {
    std::cout << _type << " has been " << spell.getEffects() << "!" << std::endl;
}

ATarget::ATarget(void) {}

ATarget::ATarget(const ATarget &other) {
    static_cast<void>(other);
}

ATarget &ATarget::operator=(const ATarget &other) {
    static_cast<void>(other);
    return (*this);
}
