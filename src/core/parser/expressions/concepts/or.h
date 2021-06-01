#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_OR_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_OR_H_

#include "../concept.h"
#include "../../../elements/concepts/or.h"
#include "../../utils.h"


namespace dlp {
namespace core {
namespace parser {

class OrConcept : public Concept {
protected:
    virtual element::Concept_Ptr parse_concept_impl(const VocabularyInfoImpl& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("OrConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        element::Concept_Ptr l = m_children[0]->parse_concept(vocabulary, cache);
        element::Concept_Ptr r = m_children[1]->parse_concept(vocabulary, cache);
        if (!(l && r)) {
            throw std::runtime_error("OrConcept::parse_concept_impl - children are not of type Concept.");
        }
        // 2. Construct element
        return std::make_shared<element::OrConcept>(vocabulary, l, r);
    }

public:
    OrConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, sort_children_lexicographically(std::move(children))) { }
};

}
}
}

#endif
