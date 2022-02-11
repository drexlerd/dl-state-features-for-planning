#include "effect.h"

#include "evaluation_cache.h"
#include "../include/dlplan/policy.h"


namespace dlplan::policy {

std::unique_ptr<BaseEffect> PositiveBooleanEffect::clone_impl() const {
    return std::make_unique<PositiveBooleanEffect>(*this);
}

PositiveBooleanEffect::PositiveBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool PositiveBooleanEffect::evaluate(int, const core::State&, int target_index, const core::State& target, EvaluationCache& evaluation_cache, core::PerElementEvaluationCache& element_cache) const{
    return get_feature()->evaluate(target_index, target, evaluation_cache, element_cache);
}

std::string PositiveBooleanEffect::compute_repr() const{
    return "(:e_b_pos " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<BaseEffect> NegativeBooleanEffect::clone_impl() const{
    return std::make_unique<NegativeBooleanEffect>(*this);
}

NegativeBooleanEffect::NegativeBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool NegativeBooleanEffect::evaluate(int, const core::State&, int target_index, const core::State& target, EvaluationCache& evaluation_cache, core::PerElementEvaluationCache& element_cache) const{
    return !get_feature()->evaluate(target_index, target, evaluation_cache, element_cache);
}

std::string NegativeBooleanEffect::compute_repr() const{
    return "(:e_b_neg " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<BaseEffect> UnchangedBooleanEffect::clone_impl() const{
    return std::make_unique<UnchangedBooleanEffect>(*this);
}

UnchangedBooleanEffect::UnchangedBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool UnchangedBooleanEffect::evaluate(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_cache, core::PerElementEvaluationCache& element_cache) const{
    return get_feature()->evaluate(source_index, source, evaluation_cache, element_cache) == get_feature()->evaluate(target_index, target, evaluation_cache, element_cache);
}

std::string UnchangedBooleanEffect::compute_repr() const{
    return "(:e_b_bot " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<BaseEffect> IncrementNumericalEffect::clone_impl() const{
    return std::make_unique<IncrementNumericalEffect>(*this);
}

IncrementNumericalEffect::IncrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool IncrementNumericalEffect::evaluate(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_cache, core::PerElementEvaluationCache& element_cache) const{
    return get_feature()->evaluate(source_index, source, evaluation_cache, element_cache) < get_feature()->evaluate(target_index, target, evaluation_cache, element_cache);
}

std::string IncrementNumericalEffect::compute_repr() const{
    return "(:e_n_inc " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<BaseEffect> DecrementNumericalEffect::clone_impl() const{
    return std::make_unique<DecrementNumericalEffect>(*this);
}

DecrementNumericalEffect::DecrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool DecrementNumericalEffect::evaluate(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_cache, core::PerElementEvaluationCache& element_cache) const{
    return get_feature()->evaluate(source_index, source, evaluation_cache, element_cache) > get_feature()->evaluate(target_index, target, evaluation_cache, element_cache);
}

std::string DecrementNumericalEffect::compute_repr() const{
    return "(:e_n_dec " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<BaseEffect> UnchangedNumericalEffect::clone_impl() const{
    return std::make_unique<UnchangedNumericalEffect>(*this);
}

UnchangedNumericalEffect::UnchangedNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool UnchangedNumericalEffect::evaluate(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_cache, core::PerElementEvaluationCache& element_cache) const{
    return get_feature()->evaluate(source_index, source, evaluation_cache, element_cache) == get_feature()->evaluate(target_index, target, evaluation_cache, element_cache);
}

std::string UnchangedNumericalEffect::compute_repr() const{
    return "(:e_n_bot " + std::to_string(get_feature()->get_index()) + ")";
}

}
