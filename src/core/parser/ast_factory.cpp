#include "ast_factory.h"

#include <algorithm>
#include <iostream>

#include "expressions/concepts/all.h"
#include "expressions/concepts/and.h"
#include "expressions/concepts/bot.h"
#include "expressions/concepts/or.h"
#include "expressions/concepts/primitive.h"
#include "expressions/concepts/some.h"
#include "expressions/concepts/top.h"
#include "expressions/roles/primitive.h"
#include "expressions/numericals/count.h"

namespace dlp {
namespace core {
namespace parser {


Expression_Ptr AST_Factory::make_ast(const VocabularyInfoImpl& vocabulary_info, const std::string &name, std::vector<Expression_Ptr> &&children) {
    // case 1: name is in alphabet of elements
    if (vocabulary_info.exists_element_name(name)) {
        EXPRESSION_TYPE expression_type = vocabulary_info.element_name_to_expression_type(name);
        switch (expression_type) {
            case C_ALL: {
                return std::make_unique<AllConcept>(AllConcept(name, std::move(children)));
            }
            case C_AND: {
                return std::make_unique<AndConcept>(AndConcept(name, std::move(children)));
            }
            case C_BOT: {
                return std::make_unique<BotConcept>(BotConcept(name, std::move(children)));
            }
            case C_OR: {
                return std::make_unique<OrConcept>(OrConcept(name, std::move(children)));
            }
            case C_SOME: {
                return std::make_unique<SomeConcept>(SomeConcept(name, std::move(children)));
            }
            case C_TOP: {
                return std::make_unique<TopConcept>(TopConcept(name, std::move(children)));
            }
            case N_COUNT: {
                return std::make_unique<CountNumerical>(CountNumerical(name, std::move(children)));
            }
        }
    }
    // case 2: name is in alphabet of VocabularyInfo
    else if (vocabulary_info.exists_predicate_name(name)) {
        if (children.size() == 2) {
            return std::make_unique<PrimitiveRole>(PrimitiveRole(name, std::move(children)));
        } else if (children.size() == 1) {
            return std::make_unique<PrimitiveConcept>(PrimitiveConcept(name, std::move(children)));
        } else if (children.size() == 0) {
            throw std::runtime_error("AST_Factory::make_ast - unspecified projection for predicate ("s + name + "). Is this supposed to be a concept or a role?");
        }
    }
    // case 3: name is an integer indicating projection
    else if (is_number(name)) {
        return std::make_unique<Expression>(Expression(name, std::move(children)));
    }
    // case 4: wrong syntax
    throw std::runtime_error("AST_Factory::make_ast - missing implementation for word ("s + name + ").");
}


}
}
}
