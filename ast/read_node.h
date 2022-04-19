// $Id: read_node.h,v 1.3 2019/05/20 20:20:01 ist424723 Exp $ -*- c++ -*-
#ifndef __M19_READNODE_H__
#define __M19_READNODE_H__

#include <cdk/ast/unary_expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace m19 {

  /**
   * Class for describing read nodes.
   */
  class read_node: public cdk::expression_node {
    cdk::lvalue_node *_argument;

  public:
    inline read_node(int lineno, cdk::lvalue_node *argument) :
        cdk::expression_node(lineno), _argument(argument) {
    }
    inline read_node(int lineno) :
        cdk::expression_node(lineno) {
    }

  public:
    inline cdk::lvalue_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_read_node(this, level);
    }

  };

} // m19

#endif
