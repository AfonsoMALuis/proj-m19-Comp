#ifndef __M19_IDENTITYNODE_H__
#define __M19_IDENTITYNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class identity_node: public cdk::expression_node {
    /*int _argumentInt;
    double _argumentDouble;*/
    cdk::expression_node *_expr;

  public:
    /*inline identity_node(int lineno, int argumentInt) :
        cdk::expression_node(lineno), _argumentInt(argumentInt) {
    }
    
    inline identity_node(int lineno, double argumentDouble) :
        cdk::expression_node(lineno), _argumentDouble(argumentDouble) {
    }*/
    
    inline identity_node(int lineno, cdk::expression_node *expr) :
        cdk::expression_node(lineno), _expr(expr) {
    }

  public:
    /*inline int argumentInt() {
      return _argumentInt;
    }
    inline double argumentDouble() {
      return _argumentDouble;
    }*/
    inline cdk::expression_node *expr() {
      return _expr;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

} // m19

#endif 
