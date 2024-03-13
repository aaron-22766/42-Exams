
#pragma once

#include <string>

class ATarget;

class ASpell {

public:

    ASpell(void);
    ASpell(const std::string &name, const std::string &effects);
    ASpell(const ASpell &other);
    ASpell &operator=(const ASpell &other);
    virtual ~ASpell(void);

    const std::string &getName(void) const;
    const std::string &getEffects(void) const;

    virtual ASpell *clone(void) const = 0;

    void launch(const ATarget &target) const;

protected:

    std::string _name;
    std::string _effects;

};
