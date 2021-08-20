#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_

#include "../role.h"


namespace dlplan::core::element {

class NotRole : public Role {
protected:
    const Role_Ptr m_role;
    mutable RoleDenotation_Set m_universe_set;

public:
    NotRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_not"), m_role(role) {
        if (!role) {
            throw std::runtime_error("NotRole::NotRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        if (m_universe_set.empty()) {
            int num_objects = state.get_instance_info()->get_num_objects();
            for (int object_idx_1 = 0; object_idx_1 < num_objects; ++object_idx_1) {
                for (int object_idx_2 = 0; object_idx_2 < num_objects; ++object_idx_2) {
                    m_universe_set.emplace(object_idx_1, object_idx_2);
                }
            }
        }
        RoleDenotation_Set r_set = m_universe_set;
        const RoleDenotation r_vec = m_role->evaluate(state);
        for (const auto& r : r_vec) {
            r_set.erase(r);
        }
        return RoleDenotation(r_set.begin(), r_set.end());
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
