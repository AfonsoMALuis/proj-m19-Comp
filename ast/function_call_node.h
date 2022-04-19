#ifndef __M19_FUNCTIONCALLNODE_H__
#define __M19_FUNCTIONCALLNODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/nil_node.h>
#include <cdk/ast/expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  /**
   * Class for describing function call nodes.
   *
   * If _arguments is null, them the node is either a call to a function with
   * no arguments (or in which none of the default arguments is present) or
   * an access to a variable.
   */
  class function_call_node: public cdk::expression_node {
    std::string _identifier;
    cdk::sequence_node *_arguments;
    //cdk::expression_node *_literal;

  public:
    /**
     * Constructor for a function call without arguments and literal.
     */
    /*inline function_call_node(int lineno, const std::string &identifier) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(new cdk::sequence_node(lineno)) {
    }*/
    
    /**
     * Constructor for a function call with literal and without arguments.
     */
    /*inline function_call_node(int lineno, const std::string &identifier, cdk::expression_node *literal) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(new cdk::sequence_node(lineno)), _literal(literal) {
    }*/
    
    /**
     * Constructor for a function call with arguments and literal.
     */
    /*inline function_call_node(int lineno, const std::string &identifier, cdk::sequence_node *arguments, cdk::expression_node *literal) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(new cdk::sequence_node(lineno)), _literal(literal) {
    }*/

    /**
     * Constructor for a function call with arguments and without literal.
     */
    inline function_call_node(int lineno, const std::string &identifier, cdk::sequence_node *arguments) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(arguments) {
    }

  public:
    inline const std::string &identifier() {
      return _identifier;
    }
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }
    /*inline cdk::expression_node *literal() {
      return _literal;
    }*/

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // m19

#endif 
