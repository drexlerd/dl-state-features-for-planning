#ifndef DLPLAN_SRC_CORE_TYPES_H_
#define DLPLAN_SRC_CORE_TYPES_H_

#include <limits>
#include <vector>
#include <string>
#include <memory>

using namespace std::string_literals;

namespace dlplan::core {

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;

const int UNDEFINED = -1;
const int INF = std::numeric_limits<int>::max();

}

#endif
