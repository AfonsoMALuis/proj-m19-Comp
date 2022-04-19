#ifndef __M19_BODYNODE_H__
#define __M19_BODYNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include "targets/basic_ast_visitor.h"

namespace m19 {

  class body_node: public cdk::basic_node {
    m19::initial_section_node *_initialSection;
    cdk::sequence_node *_section;
    m19::final_section_node *_finalSection;

  public:
    inline body_node(int lineno, cdk::sequence_node *section) :
        cdk::basic_node(lineno), _section(section) {
    }
    
    inline body_node(int lineno, m19::initial_section_node *initialSection, cdk::sequence_node *section) :
        cdk::basic_node(lineno), _initialSection(initialSection), _section(section) {
    }
    
    inline body_node(int lineno, cdk::sequence_node *section, m19::final_section_node *finalSection) :
        cdk::basic_node(lineno), _section(section), _finalSection(finalSection) {
    }
    
    inline body_node(int lineno, m19::initial_section_node *initialSection, cdk::sequence_node *section, m19::final_section_node *finalSection) :
        cdk::basic_node(lineno), _initialSection(initialSection), _section(section), _finalSection(finalSection) {
    }

  public:
    inline m19::initial_section_node *initialSection() {
      return _initialSection;
    }
    inline cdk::sequence_node *section() {
      return _section;
    }
    inline m19::final_section_node *finalSection() {
      return _finalSection;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_body_node(this, level);
    }

  };

} // m19

#endif 
