
#include "Warlock.hpp"
#include <iostream>

Warlock::Warlock(const std::string &name, const std::string &title) :
    _name(name), _title(title) {
    std::cout << _name << ": This looks like another boring day." << std::endl;
}

Warlock::~Warlock(void) {
    std::cout << _name << ": My job here is done!" << std::endl;
}

Warlock::Warlock(void) {}

Warlock::Warlock(const Warlock &other) {
    static_cast<void>(other);
}

Warlock &Warlock::operator=(const Warlock &other) {
    static_cast<void>(other);
    return (*this);
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
