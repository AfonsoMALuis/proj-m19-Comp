#ifndef __M19_FUNCTIONDEFINITIONNODE_H__
#define __M19_FUNCTIONDEFINITIONNODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "ast/body_node.h"

namespace m19 {

  /**
   * Class for describing function definitions.
   */
  class function_definition_node: public cdk::basic_node {
    basic_type *_type;
    std::string _identifier;
    int _qualifier;
    cdk::sequence_node *_arguments;
    m19::body_node *_body;

  public:
    function_definition_node(int lineno, const std::string &identifier, int qualifier, cdk::sequence_node *arguments, m19::body_node *body) :
        cdk::basic_node(lineno), _type(new basic_type(0, basic_type::TYPE_VOID)), _identifier(identifier), _qualifier(qualifier), _arguments(arguments), _body(body) {
    }

    function_definition_node(int lineno, basic_type *type, const std::string &identifier, int qualifier, cdk::sequence_node *arguments, m19::body_node *body) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(qualifier), _arguments(arguments), _body(body) {
    }  
    
    /*inline function_definition_node(int lineno, basic_type *type, const std::string &identifier, m19::body_node *body) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(false), _arguments(new cdk::sequence_node(lineno)), _body(body) {
    }

    inline function_definition_node(int lineno, basic_type *type, const std::string &identifier, cdk::sequence_node *arguments, m19::body_node *body) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(false), _arguments(arguments), _body(body) {
    }
    
    inline function_definition_node(int lineno, basic_type *type, const std::string &identifier, bool qualifier, m19::body_node *body) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(true), _arguments(new cdk::sequence_node(lineno)), _body(body) {
    }
    
    inline function_definition_node(int lineno, basic_type *type, const std::string &identifier, bool qualifier, cdk::sequence_node *arguments, m19::body_node *body) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(true), _arguments(arguments), _body(body) {
    }*/

  public:
    inline basic_type *type() {
      return _type;
    }
    inline const std::string &identifier() const {
      return _identifier;
    }
    inline int qualifier(){
      return _qualifier;
    }
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }
    inline m19::body_node *body() {
      return _body;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // m19

#endif 
