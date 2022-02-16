#ifndef DLPLAN_INCLUDE_DLPLAN_TYPES_H_
#define DLPLAN_INCLUDE_DLPLAN_TYPES_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>


namespace dlplan::core {
class State;

using pState = std::shared_ptr<State>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;
using IndexPair_Vec = std::vector<std::pair<int, int>>;

}

#endif
