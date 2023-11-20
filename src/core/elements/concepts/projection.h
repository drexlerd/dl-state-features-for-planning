#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PROJECTION_H_

#include "../utils.h"
#include "../../../../include/dlplan/core.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class ProjectionConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::ProjectionConcept& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::ProjectionConcept* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::ProjectionConcept* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::ProjectionConcept, std::weak_ptr<dlplan::core::ProjectionConcept>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::ProjectionConcept, std::weak_ptr<dlplan::core::ProjectionConcept>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::ProjectionConcept, std::weak_ptr<dlplan::core::ProjectionConcept>>* t, const unsigned int version);
}


namespace dlplan::core {
class ProjectionConcept : public Concept {
private:
    std::shared_ptr<const Role> m_role;
    int m_pos;

    void compute_result(const RoleDenotation& denot, ConceptDenotation& result) const {
        for (const auto& pair : denot.to_vector()) {
            if (m_pos == 0) result.insert(pair.first);
            else if (m_pos == 1) result.insert(pair.second);
        }
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role->evaluate(state, caches),
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        auto role_denotations = m_role->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_denotations)[i],
                denotation);
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
       return denotations;
    }

    ProjectionConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const std::shared_ptr<const Role>& role, int pos)
    : Concept(vocabulary_info, index, role->is_static()), m_role(role), m_pos(pos) {
        if (pos < 0 || pos > 1) {
            throw std::runtime_error("ProjectionConcept::ProjectionConcept - projection index out of range, should be 0 or 1 ("s + std::to_string(pos) + ")");
        }
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, ProjectionConcept& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const ProjectionConcept* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, ProjectionConcept* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const ProjectionConcept&>(other);
            return m_is_static == other_derived.m_is_static
                && m_role == other_derived.m_role
                && m_pos == other_derived.m_pos;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_role, m_pos);
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_projection" << "(";
        m_role->compute_repr(out);
        out << "," << std::to_string(m_pos) << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role->compute_evaluate_time_score() + SCORE_QUADRATIC;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::ProjectionConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::ProjectionConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
    ar << t->m_pos;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::ProjectionConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    int pos;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ar >> pos;
    ::new(t)dlplan::core::ProjectionConcept(index, vocabulary, role, pos);
}

template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::ProjectionConcept, std::weak_ptr<dlplan::core::ProjectionConcept>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::ProjectionConcept, std::weak_ptr<dlplan::core::ProjectionConcept>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::ProjectionConcept, std::weak_ptr<dlplan::core::ProjectionConcept>>* t, const unsigned int /*version*/) {
    dlplan::core::ProjectionConcept* first = nullptr;
    std::weak_ptr<dlplan::core::ProjectionConcept>* second = nullptr;
    ar >> const_cast<dlplan::core::ProjectionConcept&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::ProjectionConcept, std::weak_ptr<dlplan::core::ProjectionConcept>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::ProjectionConcept, "dlplan::core::ProjectionConcept")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::ProjectionConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::ProjectionConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::ProjectionConcept>& right_concept) const {
            return *left_concept < *right_concept;
        }
    };

    template<>
    struct hash<dlplan::core::ProjectionConcept>
    {
        std::size_t operator()(const dlplan::core::ProjectionConcept& concept_) const {
            return concept_.hash();
        }
    };
}

#endif
