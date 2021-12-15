#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SUBSET_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SUBSET_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class SubsetConcept : public Concept {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    SubsetConcept(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Concept(vocabulary, "c_subset"), m_role_left(role_left), m_role_right(role_right) {
        if (!(role_left && role_right)) {
            throw std::runtime_error("SubsetConcept::SubsetConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        const RoleDenotation r = m_role_left->evaluate(state);
        const RoleDenotation s = m_role_right->evaluate(state);
        int num_objects = state.get_instance_info()->get_num_objects();
        ConceptDenotation result = state.get_instance_info()->get_top_concept_vec();
        // find counterexamples a : exists b . (a,b) in R and (a,b) notin S
        for (int i = 0; i < num_objects; ++i) {
            for (int j = 0; j < num_objects; ++j) {
                int index = i * num_objects + j;
                if (r[index] && !s[index]) {
                    result.reset(i);
                    break;
                }
            }
        }
        return result;
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role_left->compute_repr() << "," << m_role_right->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
