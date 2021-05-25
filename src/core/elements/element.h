#ifndef DLP_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLP_SRC_CORE_ELEMENTS_ELEMENT_H_

#include "types.h"
#include "../types.h"
#include "../cache.h"
#include "../../../include/dlp/core.h"


namespace dlp {
namespace core {
namespace element {

template<typename T>
class Element {
protected:
    bool m_goal;
    T m_result;
    const State *m_state;
    // TODO(dominik): we might want cache goal evaluations of different instances

protected:
    virtual T evaluate_impl(const State& state) = 0;

public:
    Element(bool goal) : m_goal(goal) { }

    /**
     * Evaluate and cache the last result.
     */
    virtual T evaluate(const InstanceInfoImpl& info, const State& state) {
        if (m_state != &state) {
            m_state = &state;
            m_result = evaluate_impl(state);
        }
        return m_result;
    }

    /**
     * Returns the complexity of the element.
     */
    virtual unsigned complexity() const = 0;

    /**
     * Getters.
     */
    virtual bool goal() const {
        return m_goal;
    }
};

}
}
}

#endif
