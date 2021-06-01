#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_OR_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_OR_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class OrConcept : public Concept {
protected:
    Concept_Ptr m_concept_left;
    Concept_Ptr m_concept_right;

protected:
    virtual const ConceptDenotation& evaluate_impl(const StateImpl& state) override {
        const ConceptDenotation& l_vec = m_concept_left->evaluate(state);
        const ConceptDenotation& r_vec = m_concept_right->evaluate(state);
        ConceptDenotation_Set r_set(l_vec.begin(), l_vec.end());
        r_set.insert(r_vec.begin(), r_vec.end());
        m_result.clear();
        m_result.insert(m_result.begin(), r_set.begin(), r_set.end());
        return m_result;
    }

public:
    OrConcept(const VocabularyInfoImpl& vocabulary, Concept_Ptr concept_1, Concept_Ptr concept_2)
    : Concept(vocabulary, "c_or") {
        if (!(concept_1 && concept_2)) {
            throw std::runtime_error("OrConcept::make_concept_element - at least one child is a nullptr.");
        }
        // Element is commutative. Hence sort lexicographically.
        if (concept_1->compute_repr() < concept_2->compute_repr()) {
            m_concept_left = concept_1;
            m_concept_right = concept_2;
        } else {
            m_concept_left = concept_2;
            m_concept_right = concept_1;
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept_left->compute_repr() << "," << m_concept_right->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif
