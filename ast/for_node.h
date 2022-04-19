// $Id: for_node.h,v 1.5 2019/05/20 20:20:01 ist424723 Exp $ -*- c++ -*-
#ifndef __M19_FORNODE_H__
#define __M19_FORNODE_H__

#include <cdk/ast/unary_expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace m19 {

  /**
   * Class for describing for-cycle nodes.
   */
  class for_node: public cdk::expression_node {
    cdk::sequence_node *_initialization, *_condition, *_incrementation;
    cdk::basic_node *_block;

  public:
    inline for_node(int lineno, cdk::sequence_node *initialization, cdk::sequence_node *condition, cdk::sequence_node *incrementation, cdk::basic_node *block) :
        expression_node(lineno), _initialization(initialization), _condition(condition), _incrementation(incrementation), _block(block) {
    }

  public:
    inline cdk::sequence_node *initialization() {
      return _initialization;
    }
    inline cdk::sequence_node *condition() {
      return _condition;
    }
    inline cdk::sequence_node *incrementation() {
      return _incrementation;
    }
    inline cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_node(this, level);
    }

  };

} // m19

#endif
