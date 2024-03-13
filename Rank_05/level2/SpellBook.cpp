
#include "SpellBook.hpp"
#include "ASpell.hpp"
#include <iterator>

SpellBook::SpellBook(void) {}

SpellBook::~SpellBook(void) {
    for (std::map<std::string, ASpell *>::iterator it = _spells.begin();
        it != _spells.end(); ++it) {
        delete it->second;
    }
    _spells.clear();
}

void SpellBook::learnSpell(ASpell *spell) {
    if (spell && _spells.find(spell->getName()) == _spells.end()) {
        _spells[spell->getName()] = spell->clone();
    }
}

void SpellBook::forgetSpell(const std::string &spell_name) {
    std::map<std::string, ASpell *>::iterator it = _spells.find(spell_name); 
    if (it != _spells.end()) {
        delete it->second;
    }
    _spells.erase(spell_name);
}

ASpell *SpellBook::createSpell(const std::string &spell_name) {
    std::map<std::string, ASpell *>::iterator it = _spells.find(spell_name); 
    if (it != _spells.end()) {
        return (it->second->clone());
    }
    return (NULL);
}
