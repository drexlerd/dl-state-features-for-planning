#include "../../include/dlplan/core.h"

#include <iostream>
#include <algorithm>

#include "elements/concept.h"
#include "elements/role.h"
#include "elements/numerical.h"
#include "elements/boolean.h"

#include "element_factory.h"
#include "instance_info.h"
#include "vocabulary_info.h"
#include "elements/types.h"


namespace dlplan::core {

ConceptDenotation::ConceptDenotation(int num_objects, utils::BitsetView data)
    : m_num_objects(num_objects), m_data(std::move(data)) { }

ConceptDenotation::~ConceptDenotation() { }

std::vector<int> ConceptDenotation::to_vector() const {
    std::vector<int> result;
    result.reserve(m_num_objects);
    for (int i = 0; i < m_num_objects; ++i) {
        if (m_data.test(i)) result.push_back(i);
    }
    result.shrink_to_fit();
    return result;
}

int ConceptDenotation::get_num_objects() const {
    return m_num_objects;
}

utils::BitsetView& ConceptDenotation::get_data() {
    return m_data;
}

const utils::BitsetView& ConceptDenotation::get_data() const {
    return m_data;
}

RoleDenotation::RoleDenotation(int num_objects, utils::BitsetView data)
    : m_num_objects(num_objects), m_data(std::move(data)) { }

RoleDenotation::~RoleDenotation() { }

std::vector<std::pair<int, int>> RoleDenotation::to_vector() const {
    std::vector<std::pair<int, int>> result;
    result.reserve(m_num_objects * m_num_objects);
    for (int i = 0; i < m_num_objects; ++i) {
        for (int j = 0; j < m_num_objects; ++j) {
            if (m_data.test(i * m_num_objects + j)) {
                result.emplace_back(i, j);
            }
        }
    }
    result.shrink_to_fit();
    return result;
}

int RoleDenotation::get_num_objects() const {
    return m_num_objects;
}

utils::BitsetView& RoleDenotation::get_data() {
    return m_data;
}

const utils::BitsetView& RoleDenotation::get_data() const {
    return m_data;
}


InstanceInfo::InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info) : m_pImpl(InstanceInfoImpl(vocabulary_info)) { }

InstanceInfo::InstanceInfo(const InstanceInfo& other)
    : m_pImpl(*other.m_pImpl) {}

InstanceInfo& InstanceInfo::operator=(const InstanceInfo& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

InstanceInfo::~InstanceInfo() = default;

const Atom& InstanceInfo::add_atom(const std::string &name, const Name_Vec &object_names, bool negated) {
    return m_pImpl->add_atom(name, object_names, negated);
}

const Atom& InstanceInfo::add_static_atom(const std::string &name, const Name_Vec &object_names) {
    return m_pImpl->add_static_atom(name, object_names);
}

bool InstanceInfo::exists_atom(const Atom& atom) const {
    return m_pImpl->exists_atom(atom);
}

const std::vector<Atom>& InstanceInfo::get_atoms() const {
    return m_pImpl->get_atoms();
}

const Atom& InstanceInfo::get_atom(int index) const {
    return m_pImpl->get_atom(index);
}

int InstanceInfo::get_atom_idx(const std::string& name) const {
    return m_pImpl->get_atom_idx(name);
}

bool InstanceInfo::exists_object(const Object& object) const {
    return m_pImpl->exists_object(object);
}

bool InstanceInfo::exists_object(const std::string name) const {
    return m_pImpl->exists_object(name);
}

const std::vector<Object>& InstanceInfo::get_objects() const {
    return m_pImpl->get_objects();
}

const Object& InstanceInfo::get_object(int index) const {
    return m_pImpl->get_object(index);
}

int InstanceInfo::get_object_idx(const std::string& name) const {
    return m_pImpl->get_object_idx(name);
}

int InstanceInfo::get_num_objects() const {
    return m_pImpl->get_num_objects();
}

std::shared_ptr<const VocabularyInfo> InstanceInfo::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}

const Index_Vec& InstanceInfo::get_static_atom_idxs() const {
    return m_pImpl->get_static_atom_idxs();
}

const phmap::flat_hash_map<int, std::vector<int>>& InstanceInfo::get_per_predicate_idx_static_atom_idxs() const {
    return m_pImpl->get_per_predicate_idx_static_atom_idxs();
}


VocabularyInfo::VocabularyInfo() : m_pImpl(VocabularyInfoImpl()) { }

VocabularyInfo::VocabularyInfo(const VocabularyInfo& other) : m_pImpl(*other.m_pImpl) { }

VocabularyInfo& VocabularyInfo::operator=(const VocabularyInfo& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

VocabularyInfo::~VocabularyInfo() = default;

const Predicate& VocabularyInfo::add_predicate(const std::string &name, int arity) {
    return m_pImpl->add_predicate(name, arity);
}

const Constant& VocabularyInfo::add_constant(const std::string& name) {
    return m_pImpl->add_constant(name);
}

bool VocabularyInfo::exists_predicate(const Predicate& predicate) const {
    return m_pImpl->exists_predicate(predicate);
}

bool VocabularyInfo::exists_predicate_name(const std::string& name) const {
    return m_pImpl->exists_predicate_name(name);
}

const std::vector<Predicate>& VocabularyInfo::get_predicates() const {
    return m_pImpl->get_predicates();
}

int VocabularyInfo::get_predicate_idx(const std::string& name) const {
    return m_pImpl->get_predicate_idx(name);
}

const Predicate& VocabularyInfo::get_predicate(int index) const {
    return m_pImpl->get_predicate(index);
}

bool VocabularyInfo::exists_constant(const Constant& constant) const {
    return m_pImpl->exists_constant(constant);
}

bool VocabularyInfo::exists_constant_name(const std::string& name) const {
    return m_pImpl->exists_constant_name(name);
}

int VocabularyInfo::get_constant_idx(const std::string& name) const {
    return m_pImpl->get_constant_idx(name);
}

const Constant& VocabularyInfo::get_constant(int index) const {
    return m_pImpl->get_constant(index);
}

const std::vector<Constant>& VocabularyInfo::get_constants() const {
    return m_pImpl->get_constants();
}

size_t VocabularyInfo::compute_hash() const {
    return m_pImpl->compute_hash();
}


Concept::Concept(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Concept>&& concept)
    : Element<ConceptDenotation>(vocabulary_info), m_element(concept) {
    if (!m_element) {
        throw std::runtime_error("Concept::Concept - tried to construct Concept from nullptr");
    }
}

Concept::~Concept() = default;

ConceptDenotation Concept::evaluate(PerElementEvaluationContext& context) const {
    // TODO: move duplicate code into element.evaluate?
    if (context.state->get_instance_info()->get_vocabulary_info() != get_vocabulary_info()) {
        throw std::runtime_error("Concept::evaluate - mismatched vocabularies of Concept and State.");
    }
    context.cache.clear_if_state_changed(context.state);
    return m_element->evaluate(context);
}

int Concept::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Concept::compute_repr() const {
    return m_element->compute_repr();
}

std::shared_ptr<const element::Concept> Concept::get_element() const {
    return m_element;
}


Role::Role(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Role>&& role)
    : Element<RoleDenotation>(vocabulary_info), m_element(role) {
    if (!m_element) {
        throw std::runtime_error("Role::Role - tried to construct Role from nullptr");
    }
}

Role::~Role() = default;

RoleDenotation Role::evaluate(PerElementEvaluationContext& context) const {
    if (context.state->get_instance_info()->get_vocabulary_info() != get_vocabulary_info()) {
        throw std::runtime_error("Role::evaluate - mismatched vocabularies of Role and State.");
    }
    context.cache.clear_if_state_changed(context.state);
    return m_element->evaluate(context);
}

int Role::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Role::compute_repr() const {
    return m_element->compute_repr();
}

std::shared_ptr<const element::Role> Role::get_element() const {
    return m_element;
}


Numerical::Numerical(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Numerical>&& numerical)
    : Element<int>(vocabulary_info), m_element(numerical) {
    if (!m_element) {
        throw std::runtime_error("Numerical::Numerical - tried to construct Numerical from nullptr");
    }
}

Numerical::~Numerical() = default;

int Numerical::evaluate(PerElementEvaluationContext& context) const {
    if (context.state->get_instance_info()->get_vocabulary_info() != get_vocabulary_info()) {
        throw std::runtime_error("Numerical::evaluate - mismatched vocabularies of Numerical and State.");
    }
    context.cache.clear_if_state_changed(context.state);
    return m_element->evaluate(context);
}

int Numerical::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Numerical::compute_repr() const {
    return m_element->compute_repr();
}

std::shared_ptr<const element::Numerical> Numerical::get_element() const {
    return m_element;
}


Boolean::Boolean(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Boolean>&& boolean)
    : Element<bool>(vocabulary_info), m_element(boolean) {
    if (!m_element) {
        throw std::runtime_error("Boolean::Boolean - tried to construct Boolean from nullptr");
    }
}

Boolean::~Boolean() = default;

bool Boolean::evaluate(PerElementEvaluationContext& context) const {
    if (context.state->get_instance_info()->get_vocabulary_info() != get_vocabulary_info()) {
        throw std::runtime_error("Boolean::evaluate - mismatched vocabularies of Boolean and State.");
    }
    context.cache.clear_if_state_changed(context.state);
    return m_element->evaluate(context);
}

int Boolean::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Boolean::compute_repr() const {
    return m_element->compute_repr();
}

std::shared_ptr<const element::Boolean> Boolean::get_element() const {
    return m_element;
}


SyntacticElementFactory::SyntacticElementFactory(std::shared_ptr<const VocabularyInfo> vocabulary_info) : m_pImpl(SyntacticElementFactoryImpl(vocabulary_info)) { }

SyntacticElementFactory::SyntacticElementFactory(const SyntacticElementFactory& other) : m_pImpl(*other.m_pImpl) { }

SyntacticElementFactory& SyntacticElementFactory::operator=(const SyntacticElementFactory& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

SyntacticElementFactory::~SyntacticElementFactory() = default;

const VocabularyInfo* SyntacticElementFactory::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}

Concept SyntacticElementFactory::parse_concept(const std::string &description) {
    return m_pImpl->parse_concept(description);
}

Role SyntacticElementFactory::parse_role(const std::string &description) {
    return m_pImpl->parse_role(description);
}

Numerical SyntacticElementFactory::parse_numerical(const std::string &description) {
    return m_pImpl->parse_numerical(description);
}

Boolean SyntacticElementFactory::parse_boolean(const std::string &description) {
    return m_pImpl->parse_boolean(description);
}


Boolean SyntacticElementFactory::make_empty_boolean(const Concept& concept) {
    return m_pImpl->make_empty_boolean(concept);
}

Boolean SyntacticElementFactory::make_empty_boolean(const Role& role) {
    return m_pImpl->make_empty_boolean(role);
}

Boolean SyntacticElementFactory::make_concept_inclusion_boolean(const Concept& concept_left, const Concept& concept_right) {
    return m_pImpl->make_concept_inclusion_boolean(concept_left, concept_right);
}

Boolean SyntacticElementFactory::make_role_inclusion_boolean(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_role_inclusion_boolean(role_left, role_right);
}

Boolean SyntacticElementFactory::make_nullary_boolean(const Predicate& predicate) {
    return m_pImpl->make_nullary_boolean(predicate);
}

Concept SyntacticElementFactory::make_all_concept(const Role& role, const Concept& concept) {
    return m_pImpl->make_all_concept(role, concept);
}

Concept SyntacticElementFactory::make_and_concept(const Concept& concept_left, const Concept& concept_right) {
    return m_pImpl->make_and_concept(concept_left, concept_right);
}

Concept SyntacticElementFactory::make_bot_concept() {
    return m_pImpl->make_bot_concept();
}

Concept SyntacticElementFactory::make_diff_concept(const Concept& concept_left, const Concept& concept_right) {
    return m_pImpl->make_diff_concept(concept_left, concept_right);
}

Concept SyntacticElementFactory::make_equal_concept(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_equal_concept(role_left, role_right);
}

Concept SyntacticElementFactory::make_not_concept(const Concept& concept) {
    return m_pImpl->make_not_concept(concept);
}

Concept SyntacticElementFactory::make_one_of_concept(const Constant& constant) {
    return m_pImpl->make_one_of_concept(constant);
}

Concept SyntacticElementFactory::make_or_concept(const Concept& concept_left, const Concept& concept_right) {
    return m_pImpl->make_or_concept(concept_left, concept_right);
}

Concept SyntacticElementFactory::make_projection_concept(const Role& role, int pos) {
    return m_pImpl->make_projection_concept(role, pos);
}

Concept SyntacticElementFactory::make_primitive_concept(const Predicate& predicate, int pos) {
    return m_pImpl->make_primitive_concept(predicate, pos);
}

Concept SyntacticElementFactory::make_some_concept(const Role& role, const Concept& concept) {
    return m_pImpl->make_some_concept(role, concept);
}

Concept SyntacticElementFactory::make_subset_concept(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_subset_concept(role_left, role_right);
}

Concept SyntacticElementFactory::make_top_concept() {
    return m_pImpl->make_top_concept();
}

Numerical SyntacticElementFactory::make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return m_pImpl->make_concept_distance(concept_from, role, concept_to);
}

Numerical SyntacticElementFactory::make_count(const Concept& concept) {
    return m_pImpl->make_count(concept);
}

Numerical SyntacticElementFactory::make_count(const Role& role) {
    return m_pImpl->make_count(role);
}

Numerical SyntacticElementFactory::make_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    return m_pImpl->make_role_distance(role_from, role, role_to);
}

Numerical SyntacticElementFactory::make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return m_pImpl->make_sum_concept_distance(concept_from, role, concept_to);
}

Numerical SyntacticElementFactory::make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    return m_pImpl->make_sum_role_distance(role_from, role, role_to);
}

Role SyntacticElementFactory::make_and_role(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_and_role(role_left, role_right);
}

Role SyntacticElementFactory::make_compose_role(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_compose_role(role_left, role_right);
}

Role SyntacticElementFactory::make_diff_role(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_diff_role(role_left, role_right);
}

Role SyntacticElementFactory::make_identity_role(const Concept& concept) {
    return m_pImpl->make_identity_role(concept);
}

Role SyntacticElementFactory::make_inverse_role(const Role& role) {
    return m_pImpl->make_inverse_role(role);
}

Role SyntacticElementFactory::make_not_role(const Role& role) {
    return m_pImpl->make_not_role(role);
}

Role SyntacticElementFactory::make_or_role(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_or_role(role_left, role_right);
}

Role SyntacticElementFactory::make_primitive_role(const Predicate& predicate, int pos_1, int pos_2) {
    return m_pImpl->make_primitive_role(predicate, pos_1, pos_2);
}

Role SyntacticElementFactory::make_restrict_role(const Role& role, const Concept& concept) {
    return m_pImpl->make_restrict_role(role, concept);
}

Role SyntacticElementFactory::make_top_role() {
    return m_pImpl->make_top_role();
}

Role SyntacticElementFactory::make_transitive_closure(const Role& role) {
    return m_pImpl->make_transitive_closure(role);
}

Role SyntacticElementFactory::make_transitive_reflexive_closure(const Role& role) {
    return m_pImpl->make_transitive_reflexive_closure(role);
}

}
