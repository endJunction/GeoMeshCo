#ifndef CLI_H
#define CLI_H

#include <vector>
#include <string>

struct CLI {

    CLI(int, char**);

    double z_scale;
    double bottom;
    double angular_bound;
    double approximation;
    double facet_size;
    std::string input;
    std::string output;
};

#endif  // CLI_H
