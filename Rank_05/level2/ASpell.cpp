
#include "ASpell.hpp"
#include "ATarget.hpp"

ASpell::ASpell(const std::string &name, const std::string &effects) :
    _name(name), _effects(effects) {}

ASpell::~ASpell(void) {}

const std::string &ASpell::getName(void) const {
    return (_name);
}

const std::string &ASpell::getEffects(void) const {
    return (_effects);
}

void ASpell::launch(const ATarget &target) const {
    target.getHitBySpell(*this);
}

ASpell::ASpell(void) {}

ASpell::ASpell(const ASpell &other) {
    static_cast<void>(other);
}

ASpell &ASpell::operator=(const ASpell &other) {
    static_cast<void>(other);
    return (*this);
}
