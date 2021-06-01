#include "element_factory.h"
#include "parser/parser.h"
#include "parser/expressions/expression.h"
#include "elements/concepts/all.h"
#include "elements/concepts/and.h"
#include "elements/concepts/or.h"
#include "elements/concepts/primitive.h"
#include "elements/concepts/some.h"
#include "elements/numericals/count.h"
#include "elements/roles/primitive.h"


namespace dlp {
namespace core {

SyntacticElementFactoryImpl::SyntacticElementFactoryImpl(std::shared_ptr<VocabularyInfoImpl> vocabulary_info)
    : m_vocabulary_info(vocabulary_info) {
}

element::Concept_Ptr SyntacticElementFactoryImpl::parse_concept(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_concept(*m_vocabulary_info, m_cache);
}

element::Role_Ptr SyntacticElementFactoryImpl::parse_role(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_role(*m_vocabulary_info, m_cache);
}

element::Numerical_Ptr SyntacticElementFactoryImpl::parse_numerical(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_numerical(*m_vocabulary_info, m_cache);
}

element::Boolean_Ptr SyntacticElementFactoryImpl::parse_boolean(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_boolean(*m_vocabulary_info, m_cache);
}

element::Boolean_Ptr SyntacticElementFactoryImpl::make_empty_boolean(element::Concept_Ptr concept) {
}

element::Boolean_Ptr SyntacticElementFactoryImpl::make_empty_boolean(element::Role_Ptr role) {

}

element::Concept_Ptr SyntacticElementFactoryImpl::make_all_concept(element::Role_Ptr role, element::Concept_Ptr concept) {
    element::Concept_Ptr value = std::make_shared<element::AllConcept>(*m_vocabulary_info, role, concept);
    return m_cache.concept_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::Concept_Ptr SyntacticElementFactoryImpl::make_and_concept(element::Concept_Ptr concept_1, element::Concept_Ptr concept_2) {
    element::Concept_Ptr value = std::make_shared<element::AndConcept>(*m_vocabulary_info, concept_1, concept_2);
    return m_cache.concept_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::Concept_Ptr SyntacticElementFactoryImpl::make_bot_concept() {

}

element::Concept_Ptr SyntacticElementFactoryImpl::make_diff_concept(element::Concept_Ptr concept_left, element::Concept_Ptr concept_right) {

}

element::Concept_Ptr SyntacticElementFactoryImpl::make_not_concept(element::Concept_Ptr concept) {

}

element::Concept_Ptr SyntacticElementFactoryImpl::make_one_of_concept(unsigned pos) {

}

element::Concept_Ptr SyntacticElementFactoryImpl::make_or_concept(element::Concept_Ptr concept_1, element::Concept_Ptr concept_2) {
    element::Concept_Ptr value = std::make_shared<element::OrConcept>(*m_vocabulary_info, concept_1, concept_2);
    return m_cache.concept_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::Concept_Ptr SyntacticElementFactoryImpl::make_primitive_concept(const std::string& name, unsigned pos) {
    element::Concept_Ptr value = std::make_shared<element::PrimitiveConcept>(*m_vocabulary_info, name, pos);
    return m_cache.concept_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::Concept_Ptr SyntacticElementFactoryImpl::make_some_concept(element::Role_Ptr role, element::Concept_Ptr concept) {
    element::Concept_Ptr value = std::make_shared<element::SomeConcept>(*m_vocabulary_info, role, concept);
    return m_cache.concept_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::Concept_Ptr SyntacticElementFactoryImpl::make_subset_concept(element::Role_Ptr role_left, element::Role_Ptr role_right) {

}

element::Concept_Ptr SyntacticElementFactoryImpl::make_top_concept() {

}

element::Numerical_Ptr SyntacticElementFactoryImpl::make_concept_distance(element::Concept_Ptr concept_from, element::Role_Ptr role, element::Concept_Ptr concept_to) {

}

element::Numerical_Ptr SyntacticElementFactoryImpl::make_count(element::Concept_Ptr element) {
    element::Numerical_Ptr value = std::make_shared<element::CountNumerical<element::Concept_Ptr>>(*m_vocabulary_info, element);
    return m_cache.numerical_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::Numerical_Ptr SyntacticElementFactoryImpl::make_count(element::Role_Ptr element) {
    element::Numerical_Ptr value = std::make_shared<element::CountNumerical<element::Role_Ptr>>(*m_vocabulary_info, element);
    return m_cache.numerical_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::Numerical_Ptr SyntacticElementFactoryImpl::make_role_distance(element::Role_Ptr role_from, element::Role_Ptr role, element::Role_Ptr role_to) {

}

element::Numerical_Ptr SyntacticElementFactoryImpl::make_sum_concept_distance(element::Concept_Ptr concept_from, element::Role_Ptr role, element::Concept_Ptr concept_to) {

}

element::Numerical_Ptr SyntacticElementFactoryImpl::make_sum_role_distance(element::Role_Ptr role_from, element::Role_Ptr role, element::Role_Ptr role_to) {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_and_role(element::Role_Ptr role_left, element::Role_Ptr role_right) {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_compose_role(element::Role_Ptr role_left, element::Role_Ptr role_right) {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_diff_role(element::Role_Ptr role_left, element::Role_Ptr role_right) {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_identity_role(element::Concept_Ptr concept) {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_inverse_role(element::Role_Ptr role) {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_not_role(element::Role_Ptr role) {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_or_role(element::Role_Ptr role_left, element::Role_Ptr role_right) {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_primitive_role(const std::string& name, unsigned pos_1, unsigned pos_2) {
    element::Role_Ptr value = std::make_shared<element::PrimitiveRole>(*m_vocabulary_info, name, pos_1, pos_2);
    return m_cache.role_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::Role_Ptr SyntacticElementFactoryImpl::make_restrict_role(element::Role_Ptr role, element::Concept_Ptr concept) {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_top_role() {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_transitive_closure(element::Role_Ptr role) {

}

element::Role_Ptr SyntacticElementFactoryImpl::make_transitive_reflexive_closure(element::Role_Ptr role) {

}

}
}
