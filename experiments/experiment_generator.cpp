#include <vector>
#include <iostream>
#include <chrono>

#include "../include/dlplan/core.h"
#include "../include/dlplan/generator.h"
#include "../include/dlplan/state_space.h"

using namespace dlplan;


int main(int argc, char** argv) {
    std::cout << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
    if (argc != 11) {
        std::cout << "User error. Expected: ./experiment_core <str:domain_filename> <str:instance_filename> <int:concept_complexity_limit> <int:role_complexity_limit> <int:boolean_complexity_limit> <int:numerical_complexity_limit> <int:time_limit> <int:features_limit> <int:limit_threads> <int:num_iterations>" << std::endl;
        return 1;
    }

    state_space::StateSpaceGenerator().generate_state_space(argv[1], argv[2]);
    auto state_space = state_space::StateSpaceReader().read(nullptr, 0);
    std::cout << "Started generating features" << std::endl;
    std::cout << "Number of states: " << state_space.get_num_states() << std::endl;
    auto syntactic_element_factory = core::SyntacticElementFactory(state_space.get_instance_info()->get_vocabulary_info());
    auto feature_generator = generator::FeatureGenerator();
    feature_generator.set_generate_inclusion_boolean(false);
    feature_generator.set_generate_diff_concept(false);
    feature_generator.set_generate_or_concept(false);
    feature_generator.set_generate_subset_concept(false);
    feature_generator.set_generate_and_role(false);
    feature_generator.set_generate_compose_role(false);
    feature_generator.set_generate_diff_role(false);
    feature_generator.set_generate_identity_role(false);
    feature_generator.set_generate_not_role(false);
    feature_generator.set_generate_or_role(false);
    feature_generator.set_generate_top_role(false);
    feature_generator.set_generate_transitive_reflexive_closure_role(false);
    auto feature_reprs = feature_generator.generate(syntactic_element_factory, std::atoi(argv[3]), std::atoi(argv[4]), std::atoi(argv[5]), std::atoi(argv[6]), std::atoi(argv[7]), std::atoi(argv[8]), std::atoi(argv[9]), core::States(state_space.get_states_ref().begin(), state_space.get_states_ref().end()));

    std::vector<core::Boolean> boolean_features;
    std::vector<core::Numerical> numerical_features;
    for (const auto& repr : feature_reprs) {
        if (repr.substr(0, 2) == "b_") {
            boolean_features.push_back(syntactic_element_factory.parse_boolean(repr));
        } else if (repr.substr(0, 2) == "n_") {
            numerical_features.push_back(syntactic_element_factory.parse_numerical(repr));
        }
    }

    std::cout << "Started element evaluation" << std::endl;
    std::cout << "Number of boolean elements: " << boolean_features.size() << std::endl;
    std::cout << "Number of numerical elements: " << numerical_features.size() << std::endl;
    std::cout << "Number of states: " << state_space.get_num_states() << std::endl;
    std::cout << "Number of evaluations per element: " << argv[10] << std::endl;

    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < std::atoi(argv[10]); ++i) {
            for (const auto& state : state_space.get_states_ref()) {
                for (const auto& boolean : boolean_features) {
                    boolean.evaluate(state);
                }
                for (const auto& numerical : numerical_features) {
                    numerical.evaluate(state);
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time evaluate features for single state without cache: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "ms" << std::endl;
    }

    {
        auto start = std::chrono::steady_clock::now();
        core::DenotationsCaches caches;
        for (int i = 0; i < std::atoi(argv[10]); ++i) {
            for (const auto& state : state_space.get_states_ref()) {
                for (const auto& boolean : boolean_features) {
                    boolean.evaluate(state, caches);
                }
                for (const auto& numerical : numerical_features) {
                    numerical.evaluate(state, caches);
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time evaluate features for single state with cache: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "ms" << std::endl;
    }

    {
        auto start = std::chrono::steady_clock::now();
        core::DenotationsCaches caches;
        core::States states_vec(state_space.get_states_ref().begin(), state_space.get_states_ref().end());
        for (int i = 0; i < std::atoi(argv[10]); ++i) {
            for (const auto& boolean : boolean_features) {
                boolean.evaluate(states_vec, caches);
            }
            for (const auto& numerical : numerical_features) {
                numerical.evaluate(states_vec, caches);
            }
        }
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time evaluate features for all states with cache: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "ms" << std::endl;
    }
    return 0;
}