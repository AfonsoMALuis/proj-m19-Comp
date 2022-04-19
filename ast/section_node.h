#ifndef __M19_SECTIONNODE_H__
#define __M19_SECTIONNODE_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class section_node: public cdk::basic_node {
    cdk::expression_node *_expression;
    m19::block_node *_block;

  public:
    inline section_node(int lineno, m19::block_node *block) :
        cdk::basic_node(lineno), _block(block) {
    }
    
    inline section_node(int lineno, cdk::expression_node *expression, m19::block_node *block) :
        cdk::basic_node(lineno), _expression(expression), _block(block) {
    }

  public:
    inline cdk::expression_node *expression() {
      return _expression;
    }
    inline m19::block_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_section_node(this, level);
    }

  };

} // m19

#endif 
