
#pragma once

#include <string>

class Warlock {

public:

    Warlock(const std::string &name, const std::string &title);
    ~Warlock(void);

    const std::string &getName(void) const;
    const std::string &getTitle(void) const;
    void setTitle(const std::string &title);

    void introduce() const;

private:

    Warlock(void);
    Warlock(const Warlock &other);
    Warlock &operator=(const Warlock &other);

    const std::string _name;
    std::string _title;

};
