#ifndef __M19_SEMANTICS_SYMBOL_H__
#define __M19_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace m19 {

  class symbol {
    bool _constant;
    int _qualifier;
    long _value; // hack!
    basic_type *_type;
    std::string _name;
    bool _initialized;
    long _offset = 0;
    bool _function; // false for variables
    bool _forward = false;

  public:
    symbol(bool constant, int qualifier, basic_type *type, const std::string &name, bool initialized, bool function, bool forward =
               false) :
        _constant(constant), _qualifier(qualifier), _value(0), _type(type), _name(name), _initialized(initialized), _function(
            function), _forward(forward) {
    }

    virtual ~symbol() {
      delete _type;
    }

    int qualifier() const {
      return _qualifier;
    }
    basic_type *type() const {
      return _type;
    }
    const std::string &name() const {
      return _name;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }
    long offset() const {
      return _offset;
    }
    long offset(long v) {
      return _offset = v;
    }
    void set_offset(int offset) {
      _offset = offset;
    }
    bool global() const {
      return _offset == 0;
    }
    bool forward() const {
      return _forward;
    }
    bool isFunction() const {
      return _function;
    }
  };

} // m19

#endif
