#include "../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"
#include "policy_builder.h"
#include "reader.h"
#include "writer.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {


BaseCondition::BaseCondition(std::shared_ptr<const core::BaseElement> base_feature, int index)
    : m_base_feature(base_feature), m_index(index) { }

BaseCondition::BaseCondition(BaseCondition&& other) = default;

BaseCondition& BaseCondition::operator=(BaseCondition&& other) = default;

BaseCondition::~BaseCondition() = default;

void BaseCondition::set_index(int index) {
    m_index = index;
}

int BaseCondition::get_index() const {
    return m_index;
}

std::shared_ptr<const core::BaseElement> BaseCondition::get_base_feature() const {
    return m_base_feature;
}

std::string BaseCondition::str() const {
    return compute_repr();
}


BaseEffect::BaseEffect(std::shared_ptr<const core::BaseElement> base_feature, int index)
    : m_base_feature(base_feature), m_index(index) { }

BaseEffect::BaseEffect(BaseEffect&& other) = default;

BaseEffect& BaseEffect::operator=(BaseEffect&& other) = default;

BaseEffect::~BaseEffect() = default;

void BaseEffect::set_index(int index) {
    m_index = index;
}

int BaseEffect::get_index() const {
    return m_index;
}

std::shared_ptr<const core::BaseElement> BaseEffect::get_base_feature() const {
    return m_base_feature;
}

std::string BaseEffect::str() const {
    return compute_repr();
}


PolicyBuilder::PolicyBuilder() = default;

PolicyBuilder::PolicyBuilder(const PolicyBuilder& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyBuilder& PolicyBuilder::operator=(const PolicyBuilder& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyBuilder::PolicyBuilder(PolicyBuilder&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyBuilder& PolicyBuilder::operator=(PolicyBuilder&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

PolicyBuilder::~PolicyBuilder() = default;

std::shared_ptr<const core::Boolean> PolicyBuilder::add_boolean_feature(core::Boolean boolean) {
    return m_pImpl->add_boolean_feature(boolean);
}

std::shared_ptr<const core::Numerical> PolicyBuilder::add_numerical_feature(core::Numerical numerical) {
    return m_pImpl->add_numerical_feature(numerical);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_pos_condition(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_pos_condition(b);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_neg_condition(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_neg_condition(b);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_gt_condition(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_gt_condition(n);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_eq_condition(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_eq_condition(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_pos_effect(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_pos_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_neg_effect(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_neg_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_bot_effect(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_bot_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_inc_effect(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_inc_effect(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_dec_effect(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_dec_effect(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_bot_effect(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_bot_effect(n);
}

std::shared_ptr<const Rule> PolicyBuilder::add_rule(
    std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
    std::vector<std::shared_ptr<const BaseEffect>>&& effects) {
    return m_pImpl->add_rule(std::move(conditions), std::move(effects));
}

Policy PolicyBuilder::get_result() {
    return m_pImpl->get_result();
}


PolicyMinimizer::PolicyMinimizer() { }

PolicyMinimizer::PolicyMinimizer(const PolicyMinimizer& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(const PolicyMinimizer& other) = default;

PolicyMinimizer::PolicyMinimizer(PolicyMinimizer&& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(PolicyMinimizer&& other) = default;

PolicyMinimizer::~PolicyMinimizer() { }

Policy PolicyMinimizer::minimize_greedy(const Policy& policy) const {
    // just merge once for now
    PolicyBuilder builder;
    for (const auto& rule_1 : policy.get_rules()) {
        auto rule_1_conditions = std::unordered_set<std::shared_ptr<const BaseCondition>>(rule_1->get_conditions().begin(), rule_1->get_conditions().end());
        for (const auto& rule_2 : policy.get_rules()) {
            if (rule_1->get_index() >= rule_2->get_index()) {
                continue;
            }
            auto rule_2_conditions = std::unordered_set<std::shared_ptr<const BaseCondition>>(rule_2->get_conditions().begin(), rule_2->get_conditions().end());
            if (rule_1->get_effects() != rule_2->get_effects()) {
                continue;
            }
            std::vector<std::shared_ptr<const BaseCondition>> diff;
            std::set_symmetric_difference(rule_1_conditions.begin(), rule_1_conditions.end(), rule_2_conditions.begin(), rule_2_conditions.end(), diff.begin());
            if (diff.size() != 2) {
                continue;
            }
            if (diff[0]->get_base_feature()->get_index() != diff[1]->get_base_feature()->get_index()) {
                continue;
            }
            if (!(((std::dynamic_pointer_cast<const BooleanCondition>(diff[0]) != 0) &&
                (std::dynamic_pointer_cast<const BooleanCondition>(diff[1]) != 0)) ||
                ((std::dynamic_pointer_cast<const NumericalCondition>(diff[0]) != 0) &&
                (std::dynamic_pointer_cast<const NumericalCondition>(diff[1]) != 0)))) {
                continue;
            }

        }
        // rule_1 cannot be merged with any so we add it to the builder
        rule_1->visit(builder);
    }
    throw std::runtime_error("Not implemented.");
}

Policy PolicyMinimizer::minimize_greedy(const Policy& policy, const core::StatePairs& true_state_pairs, const core::StatePairs& false_state_pairs) const {
    /*
       Idea: A rule C -> E dominates a rules C' -> E' if C subseteq C', E subseteq E'
             and substituting C' -> E' with C -> E in policy leads to consistent classification.
    */
    /* 1. try to remove conditions such that classification remains.
    */
    /* 2. try to remove effects such that classification remains.
    */
    throw std::runtime_error("Not implemented.");
}


PolicyReader::PolicyReader() = default;

PolicyReader::PolicyReader(const PolicyReader& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyReader& PolicyReader::operator=(const PolicyReader& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyReader::PolicyReader(PolicyReader&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyReader& PolicyReader::operator=(PolicyReader&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

PolicyReader::~PolicyReader() = default;

Policy PolicyReader::read(const std::string& data, core::SyntacticElementFactory& factory) const {
    return m_pImpl->read(data, factory);
}


PolicyWriter::PolicyWriter() { }

PolicyWriter::PolicyWriter(const PolicyWriter& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyWriter& PolicyWriter::operator=(const PolicyWriter& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyWriter::PolicyWriter(PolicyWriter&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyWriter& PolicyWriter::operator=(PolicyWriter&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

PolicyWriter::~PolicyWriter() { }

std::string PolicyWriter::write(const Policy& policy) const {
    return m_pImpl->write(policy);
}

}
