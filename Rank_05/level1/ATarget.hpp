
#pragma once
#include <string>

class ASpell;

class ATarget {

public:

    ATarget(void);
    ATarget(const std::string &type);
    ATarget(const ATarget &other);
    ATarget &operator=(const ATarget &other);
    virtual ~ATarget(void);

    const std::string &getType(void) const;

    virtual ATarget *clone(void) const = 0;

    void getHitBySpell(const ASpell &spell) const;

protected:

    std::string _type;

};
