#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SOME_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SOME_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class SomeConcept : public Concept {
protected:
    const Role_Ptr m_role;
    const Concept_Ptr m_concept;

public:
    SomeConcept(const VocabularyInfo& vocabulary, Role_Ptr role, Concept_Ptr concept)
    : Concept(vocabulary, "c_some"), m_role(role), m_concept(concept) {
        if (!(role && concept)) {
            throw std::runtime_error("SomeConcept::SomeConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        RoleDenotation r_vec = m_role->evaluate(state);
        const ConceptDenotation c_vec = m_concept->evaluate(state);
        const ConceptDenotation_Set c_set(c_vec.begin(), c_vec.end());
        // 1. perform existential abstraction to find elements for which some relation to b exists.
        ConceptDenotation_Set result_set;
        for (const auto& r : r_vec) {
            if (c_set.find(r.second) != c_set.end()) {
                result_set.insert(r.first);
            }
        }
        return ConceptDenotation(result_set.begin(), result_set.end());
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << "," << m_concept->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
