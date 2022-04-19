#include <string>
#include "targets/type_checker.h"
#include "targets/symbol.h"
#include "ast/all.h"  // automatically generated

#include "m19_parser.tab.h"

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void m19::type_checker::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
}

//---------------------------------------------------------------------------

void m19::type_checker::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void m19::type_checker::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void m19::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}
void m19::type_checker::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
  else if (node->argument()->type()->name() == basic_type::TYPE_UNSPEC) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  } else
    throw std::string("wrong type in unary logical expression");
}
void m19::type_checker::do_and_node(cdk::and_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_or_node(cdk::or_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void m19::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void m19::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

void m19::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in argument of unary expression");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void m19::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void m19::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  else if (node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_POINTER)
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
  else if (node->left()->type()->name() == basic_type::TYPE_UNSPEC && node->right()->type()->name() == basic_type::TYPE_UNSPEC) {
// for @ in @ + @
    node->type(new basic_type(4, basic_type::TYPE_INT));
    node->left()->type(new basic_type(4, basic_type::TYPE_INT));
    node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  } else
    throw std::string("wrong types in binary expression");
}

void m19::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void m19::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void m19::type_checker::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id;
  }
}

void m19::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void m19::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {  
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 4);
  node->rvalue()->accept(this, lvl + 4);

  if (node->lvalue()->type()->name() == basic_type::TYPE_INT) {
    if (node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      node->type(new basic_type(4, basic_type::TYPE_INT));
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC) {
      node->type(new basic_type(4, basic_type::TYPE_INT));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_INT));
    } else
      throw std::string("wrong assignment to integer");
  } else if (node->lvalue()->type()->name() == basic_type::TYPE_POINTER) {

    if (node->rvalue()->type()->name() == basic_type::TYPE_POINTER) {
      node->type(new basic_type(4, basic_type::TYPE_POINTER));
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      node->type(new basic_type(4, basic_type::TYPE_POINTER));
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC) {
      node->type(new basic_type(4, basic_type::TYPE_ERROR));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_ERROR));
    } else
      throw std::string("wrong assignment to pointer");

  } else if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE) {

    if (node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE || node->rvalue()->type()->name() == basic_type::TYPE_INT) {
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC) {
      node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      node->rvalue()->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    } else
      throw std::string("wrong assignment to real");

  } else if (node->lvalue()->type()->name() == basic_type::TYPE_STRING) {

    if (node->rvalue()->type()->name() == basic_type::TYPE_STRING) {
      node->type(new basic_type(4, basic_type::TYPE_STRING));
    } else if (node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC) {
      node->type(new basic_type(4, basic_type::TYPE_STRING));
      node->rvalue()->type(new basic_type(4, basic_type::TYPE_STRING));
    } else
      throw std::string("wrong assignment to string");

  } else {
    throw std::string("wrong types in assignment");
  }
}

//---------------------------------------------------------------------------

void m19::type_checker::do_evaluation_node(m19::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void m19::type_checker::do_print_node(m19::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_VOID) {
    throw std::string("wrong type in print argument");
  }
}

//---------------------------------------------------------------------------

void m19::type_checker::do_read_node(m19::read_node * const node, int lvl) {
  try {
    node->argument()->accept(this, lvl);
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

//---------------------------------------------------------------------------

/*void m19::type_checker::do_while_node(m19::while_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}*/


//---------------------------------------------------------------------------

void m19::type_checker::do_for_node(m19::for_node * const node, int lvl) {
  node->initialization()->accept(this, lvl + 4);
  node->condition()->accept(this, lvl + 4);
  node->incrementation()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void m19::type_checker::do_if_node(m19::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void m19::type_checker::do_if_else_node(m19::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}



//----------------------------------------------------------------------------

void m19::type_checker::do_function_call_node(m19::function_call_node * const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->identifier();
  std::shared_ptr<m19::symbol> symbol = _symtab.find(id);

  if (symbol == nullptr) throw std::string("symbol '" + id + "' is undeclared.");

  if (!symbol->isFunction()) throw std::string("symbol '" + id + "' is not a function.");

  node->type(symbol->type());

  if (node->arguments()) {
    node->arguments()->accept(this, lvl + 4);
  }
}

//----------------------------------------------------------------------------

void m19::type_checker::do_function_declaration_node(m19::function_declaration_node * const node, int lvl) {
  std::string id;

  // "fix" naming issues...
  if (node->identifier() == "m19")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  // remember symbol so that args know
  std::shared_ptr<m19::symbol> function = std::make_shared < m19::symbol
      > (false, node->qualifier(), node->type(), id, false, true, true);

  std::shared_ptr<m19::symbol> previous = _symtab.find(function->name());
  if (previous) {
      throw std::string("conflicting declaration for '" + function->name() + "'");
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}

//----------------------------------------------------------------------------

void m19::type_checker::do_function_definition_node(m19::function_definition_node * const node, int lvl) {
  std::string id;
    
  // "fix" naming issues...
  if (node->identifier() == "m19")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  // remember symbol so that args know
  std::shared_ptr<m19::symbol> function = std::make_shared < m19::symbol
      > (false, node->qualifier(), node->type(), id, false, true);
  function->set_offset(-node->type()->size());

  std::shared_ptr<m19::symbol> previous = _symtab.find(function->name());
  if (previous) {
    if (previous->forward()
        && ((previous->qualifier() == tPUBLIC && node->qualifier() == tPUBLIC)
            || (previous->qualifier() == tPRIVATE && node->qualifier() == tPRIVATE))) {
      _symtab.replace(function->name(), function);
      _parent->set_new_symbol(function);
    } else {
      throw std::string("conflicting definition for '" + function->name() + "'");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}

//-----------------------------------------------------------------------------

void m19::type_checker::do_block_node(m19::block_node *const node, int lvl) {
  // EMPTY
}

//--------------------------------------------------------------------------------

void m19::type_checker::do_variable_declaration_node(m19::variable_declaration_node * const node, int lvl) {
  if (node->initializer() != nullptr) {
    node->initializer()->accept(this, lvl + 2);

    if (node->varType()->name() == basic_type::TYPE_INT) {
      if (node->initializer()->type()->name() != basic_type::TYPE_INT) throw std::string(
          "wrong type for initializer (integer expected).");
    } else if (node->varType()->name() == basic_type::TYPE_DOUBLE) {
      if (node->initializer()->type()->name() != basic_type::TYPE_INT
          && node->initializer()->type()->name() != basic_type::TYPE_DOUBLE) throw std::string(
          "wrong type for initializer (integer or double expected).");
    } else if (node->varType()->name() == basic_type::TYPE_STRING) {
      if (node->initializer()->type()->name() != basic_type::TYPE_STRING) throw std::string(
          "wrong type for initializer (string expected).");
    } else if (node->varType()->name() == basic_type::TYPE_POINTER) {
      if (node->initializer()->type()->name() != basic_type::TYPE_POINTER) throw std::string(
          "wrong type for initializer (pointer expected).");
    } else {
      throw std::string("unknown type for initializer.");
    }
  }
  
  const std::string &id = node->identifier();
  std::shared_ptr<m19::symbol> symbol = std::make_shared < m19::symbol > (node->constant(), // is it a constant?
  node->qualifier(), // qualifiers: public, forward, "private" (i.e., none)
  node->varType(), // type (type id + type size)
  id, // identifier
  (bool)node->initializer(), // initialized?
  false); // is it a function?
  if (_symtab.insert(id, symbol)) {
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
  } else {
    throw std::string("variable '" + id + "' redeclared");
  }
}

//---------------------------------------------------------------------------

void m19::type_checker::do_return_node(m19::return_node *const node, int lvl) {
    if (_function->type()->name() == basic_type::TYPE_VOID) throw std::string("initializer specified for void function.");

    std::cout << "FUNCT TYPE" << _function->type()->name()  << std::endl;
}

//-------------------------------------------------------------------------

void m19::type_checker::do_continue_node(m19::continue_node *const node, int lvl) {
    //EMPTY
}

//-------------------------------------------------------------------------

void m19::type_checker::do_break_node(m19::break_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void m19::type_checker::do_address_of_node(m19::address_of_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  } else {
    throw std::string("wrong type in unary logical expression");
  }
}

//---------------------------------------------------------------------------

void m19::type_checker::do_stack_alloc_node(m19::stack_alloc_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT) throw std::string(
      "integer expression expected in allocation expression");
  auto mytype = new basic_type(4, basic_type::TYPE_POINTER);
  mytype->_subtype = new basic_type(8, basic_type::TYPE_DOUBLE);
  node->type(mytype);
}

//---------------------------------------------------------------------------

void m19::type_checker::do_left_index_node(m19::left_index_node * const node, int lvl) {
  ASSERT_UNSPEC;
  if (node->pointer()) {
    node->pointer()->accept(this, lvl + 2);
    if (node->pointer()->type()->name() != basic_type::TYPE_POINTER) throw std::string(
        "pointer expression expected in index left-value");
  } else {
    if (_function->type()->name() != basic_type::TYPE_POINTER) throw std::string(
        "return pointer expression expected in index left-value");
  }
  node->index()->accept(this, lvl + 2);
  if (node->index()->type()->name() != basic_type::TYPE_INT) throw std::string("integer expression expected in left-value index");
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}

//---------------------------------------------------------------------------

void m19::type_checker::do_section_node(m19::section_node * const node, int lvl) {
    //EMPTY
}

void m19::type_checker::do_initial_section_node(m19::initial_section_node * const node, int lvl) {
    //EMPTY
}

void m19::type_checker::do_final_section_node(m19::final_section_node * const node, int lvl) {
    //EMPTY
}

void m19::type_checker::do_identity_node(m19::identity_node * const node, int lvl) {
    //EMPTY
}

void m19::type_checker::do_body_node(m19::body_node * const node, int lvl) {
    //EMPTY
}






