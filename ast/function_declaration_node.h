#ifndef __M19_FUNCTIONDECLARATIONNODE_H__
#define __M19_FUNCTIONDECLARATIONNODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>

namespace m19 {

  /**
   * Class for describing function declarations.
   * False qualifier means private.
   */
  class function_declaration_node: public cdk::basic_node {
    basic_type *_type;
    std::string _identifier;
    int _qualifier;
    cdk::sequence_node *_arguments;

  public:
    inline function_declaration_node(int lineno, const std::string &identifier, int qualifier, cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _type(new basic_type(0, basic_type::TYPE_VOID)), _identifier(identifier), _qualifier(qualifier), _arguments(arguments) {
    }

    
    inline function_declaration_node(int lineno, basic_type *type, const std::string &identifier, int qualifier, cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(qualifier), _arguments(arguments) {
    }
    
    /*inline function_declaration_node(int lineno, basic_type *type, const std::string &identifier) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(false), _arguments(new cdk::sequence_node(lineno)) {
    }
    
    inline function_declaration_node(int lineno, basic_type *type, const std::string &identifier, bool qualifier, cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(true), _arguments(arguments) {
    }

    inline function_declaration_node(int lineno, basic_type *type, const std::string &identifier, bool qualifier) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _qualifier(true), _arguments(new cdk::sequence_node(lineno)) {
    }*/

  public:
    inline basic_type *type() {
      return _type;
    }
    inline const std::string &identifier() const {
      return _identifier;
    }
    inline int qualifier() {
      return _qualifier;
    }
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // m19

#endif
