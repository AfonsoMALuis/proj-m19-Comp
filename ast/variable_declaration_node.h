#ifndef __M19_VARIABLEDECLARATIONNODE_H__
#define __M19_VARIABLEDECLARATIONNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>

namespace m19 {

  class variable_declaration_node: public cdk::basic_node {
    basic_type *_varType;
    std::string _identifier;
    int _qualifier;
    cdk::expression_node *_initializer;

  public:
    /*inline variable_declaration_node(int lineno, basic_type *varType, const std::string &identifier, cdk::expression_node *initializer) :
        cdk::basic_node(lineno), _varType(varType), _identifier(identifier), _qualifier(false), _initializer(initializer) {
    }*/
    
    inline variable_declaration_node(int lineno, basic_type *varType, const std::string &identifier, int qualifier, cdk::expression_node *initializer) :
        cdk::basic_node(lineno), _varType(varType), _identifier(identifier), _qualifier(true), _initializer(initializer) {
    }
    
    /*inline variable_declaration_node(int lineno, basic_type *varType, const std::string &identifier) :
        cdk::basic_node(lineno), _varType(varType), _identifier(identifier), _qualifier(false) {
    }
    
    inline variable_declaration_node(int lineno, basic_type *varType, const std::string &identifier, bool qualifier) :
        cdk::basic_node(lineno), _varType(varType), _identifier(identifier), _qualifier(true) {
    }*/

  public:
    bool constant() {
      return _varType == _varType->subtype(); // HACK!
    }
    basic_type *varType() {
      return _varType;
    }
    const std::string &identifier() const {
      return _identifier;
    }
    int qualifier(){
      return _qualifier;
    }
    cdk::expression_node *initializer() {
      return _initializer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_declaration_node(this, level);
    }

  };

} // m19

#endif
