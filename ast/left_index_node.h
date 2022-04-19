#ifndef __M19_LEFTINDEXNODE_H__
#define __M19_LEFTINDEXNODE_H__

#include <cdk/ast/lvalue_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class left_index_node: public cdk::lvalue_node {
    cdk::expression_node *_pointer;
    cdk::expression_node *_index;

  public:
    left_index_node(int lineno, cdk::expression_node *pointer, cdk::expression_node *index) :
        cdk::lvalue_node(lineno), _pointer(pointer), _index(index) {
    }

  public:
    cdk::expression_node *pointer() {
      return _pointer;
    }
    cdk::expression_node *index() {
      return _index;
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_left_index_node(this, level);
    }

  };

} // m19

#endif
