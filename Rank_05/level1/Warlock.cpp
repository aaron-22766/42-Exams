
#include <iostream>
#include "Warlock.hpp"
#include "ASpell.hpp"
#include "ATarget.hpp"

Warlock::Warlock(const std::string &name, const std::string &title) :
    _name(name), _title(title) {
    std::cout << _name << ": This looks like another boring day." << std::endl;
}

Warlock::~Warlock(void) {
    std::cout << _name << ": My job here is done!" << std::endl;
    for (std::map<std::string, ASpell *>::iterator it = _spells.begin();
        it != _spells.end(); ++it) {
        delete it->second;
    }
    _spells.clear();
}

const std::string &Warlock::getName(void) const {
    return (_name);
}

const std::string &Warlock::getTitle(void) const {
    return (_title);
}

void Warlock::setTitle(const std::string &title) {
    _title = title;
}

void Warlock::introduce() const {
    std::cout << _name << ": I am " << _name << ", " << _title << "!" << std::endl;
}

void Warlock::learnSpell(ASpell *spell) {
    if (spell && _spells.find(spell->getName()) == _spells.end()) {
        _spells[spell->getName()] = spell->clone();
    }
}

void Warlock::forgetSpell(const std::string spell_name) {
    std::map<std::string, ASpell *>::iterator it = _spells.find(spell_name); 
    if (it != _spells.end()) {
        delete it->second;
    }
    _spells.erase(spell_name);
}

void Warlock::launchSpell(const std::string spell_name, const ATarget &target) {
    std::map<std::string, ASpell *>::iterator it = _spells.find(spell_name);
    if (it != _spells.end()) {
        it->second->launch(target);
    }
}
