#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_

#include "../concept.h"


namespace dlplan::core::element {

class BotConcept : public Concept {
public:
    BotConcept(const VocabularyInfo& vocabulary)
    : Concept(vocabulary, "c_bot") {
    }

    ConceptDenotation evaluate(const State&, PerElementEvaluationCache&, ConceptDenotation result) const override {
        result.get_data().reset();
        return result;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name;
    }
};

}

#endif
