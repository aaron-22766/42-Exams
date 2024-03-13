
#pragma once

#include <string>
#include <map>

class ATarget;

class TargetGenerator {

public:

    TargetGenerator(void);
    ~TargetGenerator(void);

    void learnTargetType(ATarget *target);
    void forgetTargetType(const std::string &target_type);
    ATarget *createTarget(const std::string &target_type);

private:

    TargetGenerator(const TargetGenerator &other);
    TargetGenerator &operator=(const TargetGenerator &other);

    std::map<std::string, ATarget *> _targets;

};
