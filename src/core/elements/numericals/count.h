#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_

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
template<typename T>
class CountNumerical;
}


namespace boost::serialization {
    template<typename Archive, typename T>
    void serialize(Archive& ar, dlplan::core::CountNumerical<T>& t, const unsigned int version);
    template<class Archive, typename T>
    void save_construct_data(Archive& ar, const dlplan::core::CountNumerical<T>* t, const unsigned int version);
    template<class Archive, typename T>
    void load_construct_data(Archive& ar, dlplan::core::CountNumerical<T>* t, const unsigned int version);

    template<typename Archive, typename T>
    void serialize(Archive& ar, std::pair<const dlplan::core::CountNumerical<T>, std::weak_ptr<dlplan::core::CountNumerical<T>>>& t, const unsigned int version);
    template<class Archive, typename T>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::CountNumerical<T>, std::weak_ptr<dlplan::core::CountNumerical<T>>>* t, const unsigned int version);
    template<class Archive, typename T>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::CountNumerical<T>, std::weak_ptr<dlplan::core::CountNumerical<T>>>* t, const unsigned int version);
}

namespace dlplan::core {

template<typename T>
class CountNumerical : public Numerical {
private:
    const std::shared_ptr<const T> m_element;

    template<typename DENOTATION_TYPE>
    void compute_result(const DENOTATION_TYPE& denot, int& result) const {
        result = denot.size();
    }

    int evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        int denotation;
        compute_result(
            *m_element->evaluate(state, caches),
            denotation);
        return denotation;
    }

    NumericalDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        NumericalDenotations denotations;
        denotations.reserve(states.size());
        auto element_denotations = m_element->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            int denotation;
            compute_result(
                *(*element_denotations)[i],
                denotation);
            denotations.push_back(denotation);
        }
        return denotations;
    }

    CountNumerical(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const T> element)
        : Numerical(vocabulary_info, index, element->is_static()), m_element(element) { }

    template<typename Archive, typename T_>
    friend void boost::serialization::serialize(Archive& ar, CountNumerical<T_>& t, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::save_construct_data(Archive& ar, const CountNumerical<T_>* t, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::load_construct_data(Archive& ar, CountNumerical<T_>* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Numerical& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const CountNumerical&>(other);
            return m_is_static == other_derived.m_is_static
                && m_element == other_derived.m_element;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_element);
    }

    int evaluate(const State& state) const override {
        int result;
        compute_result(
            m_element->evaluate(state),
            result);
        return result;
    }

    int compute_complexity() const override {
        return m_element->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "n_count" << "(";
        m_element->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        int score = m_element->compute_evaluate_time_score();
        if (std::is_same<T, Concept>::value) {
            score += SCORE_LINEAR;
        } else if (std::is_same<T, Role>::value) {
            score += SCORE_QUADRATIC;
        } else {
            throw std::runtime_error("Inclusion::compute_evaluate_time_score - unknown template parameter.");
        }
        return score;
    }
};

}


namespace boost::serialization {
template<typename Archive, typename T>
void serialize(Archive& /* ar */ , dlplan::core::CountNumerical<T>& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Numerical>(t);
}

template<class Archive, typename T>
void save_construct_data(Archive & ar, const dlplan::core::CountNumerical<T>* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_element;
}

template<class Archive, typename T>
void load_construct_data(Archive & ar, dlplan::core::CountNumerical<T>* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const T> element;
    ar >> vocabulary;
    ar >> index;
    ar >> element;
    ::new(t)dlplan::core::CountNumerical<T>(index, vocabulary, element);
}


template<typename Archive, typename T>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::CountNumerical<T>, std::weak_ptr<dlplan::core::CountNumerical<T>>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive, typename T>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::CountNumerical<T>, std::weak_ptr<dlplan::core::CountNumerical<T>>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive, typename T>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::CountNumerical<T>, std::weak_ptr<dlplan::core::CountNumerical<T>>>* t, const unsigned int /*version*/) {
    dlplan::core::CountNumerical<T>* first;
    std::weak_ptr<dlplan::core::CountNumerical<T>>* second;
    ar >> const_cast<dlplan::core::CountNumerical<T>&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::CountNumerical<T>, std::weak_ptr<dlplan::core::CountNumerical<T>>>(*first, *second);
    delete first;
    delete second;
}



}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::CountNumerical<dlplan::core::Concept>, "dlplan::core::CountNumerical<dlplan::core::Concept>")
BOOST_CLASS_EXPORT_KEY2(dlplan::core::CountNumerical<dlplan::core::Role>, "dlplan::core::CountNumerical<dlplan::core::Role>")


namespace std {
    template<typename T>
    struct less<std::shared_ptr<const dlplan::core::CountNumerical<T>>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::CountNumerical<T>>& left_numerical,
            const std::shared_ptr<const dlplan::core::CountNumerical<T>>& right_numerical) const {
            return *left_numerical < *right_numerical;
        }
    };

    template<typename T>
    struct hash<dlplan::core::CountNumerical<T>>
    {
        std::size_t operator()(const dlplan::core::CountNumerical<T>& numerical) const {
            return numerical.hash();
        }
    };
}

#endif