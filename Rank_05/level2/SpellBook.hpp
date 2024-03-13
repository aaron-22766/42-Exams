
#pragma once

#include <string>
#include <map>

class ASpell;

class SpellBook {

public:

    SpellBook(void);
    ~SpellBook(void);

    void learnSpell(ASpell *spell);
    void forgetSpell(const std::string &spell_name);
    ASpell *createSpell(const std::string &spell_name);

private:

    SpellBook(const SpellBook &other);
    SpellBook &operator=(const SpellBook &other);

    std::map<std::string, ASpell *> _spells;

};
