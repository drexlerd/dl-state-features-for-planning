#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_

#include "../role.h"


namespace dlplan::core::element {

class PrimitiveRole : public Role {
protected:
    const Predicate m_predicate;
    const int m_pos_1;
    const int m_pos_2;

public:
    PrimitiveRole(const VocabularyInfo& vocabulary, const Predicate& predicate, int pos_1, int pos_2)
    : Role(vocabulary, "r_primitive"), m_predicate(predicate), m_pos_1(pos_1), m_pos_2(pos_2) {
        if (m_pos_1 >= predicate.get_arity() || m_pos_2 >= predicate.get_arity()) {
            throw std::runtime_error("PrimitiveRole::evaluate_impl - object index does not match predicate arity ("s + std::to_string(m_pos_1) + " or " + std::to_string(m_pos_2)  + " > " + std::to_string(predicate.get_arity()) + ").");
        }
        if (!vocabulary.exists_predicate(predicate)) {
            throw std::runtime_error("PrimitiveRole::PrimitiveRole - predicate does not exist in VocabularyInfo.");
        }
    }

    void evaluate(PerElementEvaluationContext& context, RoleDenotation& result) const override {
        const auto& state = *context.state;
        const InstanceInfo& info = *state.get_instance_info();
        int num_objects = info.get_num_objects();
        auto& result_data = result.get_data();
        result_data.reset();
        const auto& atoms = info.get_atoms();
        const auto& per_predicate_idx_static_atom_idxs = state.get_per_predicate_idx_static_atom_idxs();
        auto it = per_predicate_idx_static_atom_idxs.find(m_predicate.get_index());
        if (it != per_predicate_idx_static_atom_idxs.end()) {
            for (int atom_idx : it->second) {
                const auto& atom = atoms[atom_idx];
                result_data.set(atom.get_object(m_pos_1).get_index() * num_objects + atom.get_object(m_pos_2).get_index());
            }
        }
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(" << m_predicate.get_name() << "," << std::to_string(m_pos_1) << "," << std::to_string(m_pos_2) << ")";
    }
};

}

#endif
