#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class PrimitiveConceptElement : public ConceptElement {
protected:
    unsigned m_pos;

protected:
    virtual const Concepts& evaluate_impl(const StateImpl& state) override {
        const InstanceInfoImpl& info = *state.m_parent;
        // 1. Perform error checking.
        if (!info.exists_predicate_name(m_name)) {
            throw std::runtime_error("PrimitiveConceptElement::PrimitiveConceptElement - predicate ("s + m_name + ") is missing in InstanceInfo.");
        }
        unsigned predicate_idx = info.predicate_idx(m_name);
        unsigned predicate_arity = info.predicate(predicate_idx).m_arity;
        if (m_pos >= predicate_arity) {
            throw std::runtime_error("PrimitiveConceptElement::PrimitiveConceptElement - object index does not match predicate arity ("s + std::to_string(m_pos) + " > " + std::to_string(predicate_arity) + ").");
        }
        // 2. Compute the result.
        m_result.clear();
        for (unsigned atom_idx : state.m_atoms) {
            const AtomImpl& atom = info.atom(atom_idx);
            if (atom.predicate_idx() == predicate_idx) {
                m_result.push_back(atom.object_idx(m_pos));
            }
        }
        return m_result;
    }

public:
    PrimitiveConceptElement(const std::string& name, unsigned pos)
    : ConceptElement(name), m_pos(pos) {
    }

    virtual unsigned complexity() const override {
        return 1;
    }


    virtual std::string repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << std::to_string(m_pos) << ")";
        return ss.str();
    }
};

}
}
}

#endif
