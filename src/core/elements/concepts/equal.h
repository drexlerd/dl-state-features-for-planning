#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class EqualConcept : public Concept {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    EqualConcept(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Concept(vocabulary, "c_equal"), m_role_left(role_left), m_role_right(role_right) {
        if (!(role_left && role_right)) {
            throw std::runtime_error("EqualConcept::EqualConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state, EvaluationCaches& caches, ConceptDenotation result) const override {
        RoleDenotation r = m_role_left->evaluate(state, caches);
        dlplan::utils::BitsetView r_data = r.get_data();
        RoleDenotation s = m_role_right->evaluate(state, caches);
        dlplan::utils::BitsetView s_data = s.get_data();
        int num_objects = state.get_instance_info()->get_num_objects();
        dlplan::utils::BitsetView result_data = result.get_data();
        result_data.set();
        // find counterexample [(a,b) in R and (a,b) not in S] or [(a,b) not in R and (a,b) in S]
        for (int i = 0; i < num_objects; ++i) {
            for (int j = 0; j < num_objects; ++j) {
                int index = i * num_objects + j;
                if (r_data.test(index) != s_data.test(index)) {
                    result_data.reset(i);
                    break;
                }
            }
        }
        return result;
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";

    }
};

}

#endif
