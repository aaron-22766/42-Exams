
#pragma once

#include <string>
#include "SpellBook.hpp"

class ASpell;
class ATarget;

class Warlock {

public:

    Warlock(const std::string &name, const std::string &title);
    ~Warlock(void);

    const std::string &getName(void) const;
    const std::string &getTitle(void) const;
    void setTitle(const std::string &title);

    void introduce() const;

    void learnSpell(ASpell *spell);
    void forgetSpell(const std::string spell_name);
    void launchSpell(const std::string spell_name, const ATarget &target);

private:

    Warlock(void);
    Warlock(const Warlock &other);
    Warlock &operator=(const Warlock &other);

    const std::string _name;
    std::string _title;
    SpellBook _spellBook;

};
