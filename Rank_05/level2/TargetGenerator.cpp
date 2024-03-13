
#include "TargetGenerator.hpp"
#include "ATarget.hpp"
#include <iterator>

TargetGenerator::TargetGenerator(void) {}

TargetGenerator::~TargetGenerator(void) {
    for (std::map<std::string, ATarget *>::iterator it = _targets.begin();
        it != _targets.end(); ++it) {
        delete it->second;
    }
    _targets.clear();
}

void TargetGenerator::learnTargetType(ATarget *target) {
    if (target && _targets.find(target->getType()) == _targets.end()) {
        _targets[target->getType()] = target->clone();
    }
}

void TargetGenerator::forgetTargetType(const std::string &target_type) {
    std::map<std::string, ATarget *>::iterator it = _targets.find(target_type); 
    if (it != _targets.end()) {
        delete it->second;
    }
    _targets.erase(target_type);
}

ATarget *TargetGenerator::createTarget(const std::string &target_type) {
    std::map<std::string, ATarget *>::iterator it = _targets.find(target_type); 
    if (it != _targets.end()) {
        return (it->second->clone());
    }
    return (NULL);
}
