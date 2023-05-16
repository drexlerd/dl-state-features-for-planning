#include "policy_builder.h"

#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/policy.h"

#include <stdexcept>


namespace dlplan::policy {

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_pos_condition(std::shared_ptr<const core::Boolean> b) {
    return *m_conditions.insert(m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(b)).first).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_neg_condition(std::shared_ptr<const core::Boolean> b) {
    return *m_conditions.insert(m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(b)).first).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_gt_condition(std::shared_ptr<const core::Numerical> n) {
    return *m_conditions.insert(m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(n)).first).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_eq_condition(std::shared_ptr<const core::Numerical> n) {
    return *m_conditions.insert(m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(n)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_pos_effect(std::shared_ptr<const core::Boolean> b) {
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(b)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_neg_effect(std::shared_ptr<const core::Boolean> b) {
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(b)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const core::Boolean> b) {
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(b)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_inc_effect(std::shared_ptr<const core::Numerical> n) {
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(n)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_dec_effect(std::shared_ptr<const core::Numerical> n) {
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(n)).first).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const core::Numerical> n) {
    return *m_effects.insert(m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(n)).first).first;
}

std::shared_ptr<const Rule> PolicyBuilderImpl::add_rule(Conditions&& conditions, Effects&& effects) {
    if (!(std::all_of(conditions.begin(), conditions.end(), [&](const auto& condition){ return m_conditions.count(condition); })
        && std::all_of(effects.begin(), effects.end(), [&](const auto& effect){ return m_effects.count(effect); }))) {
        throw std::runtime_error("PolicyBuilderImpl::add_rule - tried adding rules with condition or effect from different builder.");
    }
    return *m_rules.insert(m_caches.m_rule_cache->insert(std::unique_ptr<Rule>(new Rule(move(conditions), move(effects)))).first).first;
}

std::shared_ptr<const Policy> PolicyBuilderImpl::add_policy(
    std::set<std::shared_ptr<const Rule>>&& rules) {
    if (!std::all_of(rules.begin(), rules.end(), [&](const auto& rule){ return m_rules.count(rule); })) {
        throw std::runtime_error("PolicyBuilderImpl::add_policy - tried adding policy with rule from different builder.");
    }
    return *m_policies.insert(m_caches.m_policy_cache->insert(std::unique_ptr<Policy>(new Policy(std::move(rules)))).first).first;
}

}
