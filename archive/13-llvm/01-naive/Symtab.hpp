//------------------------------------------------------------------------------
//
// Symtab.hpp -- symbol table
//
//------------------------------------------------------------------------------

#include <string>
#include <unordered_map>

#include "INode.hpp"

class Symtab_local {
  std::unordered_map<std::string, INode *> symbols_;

public:
  void add(std::string symbol, INode *n) { symbols_[symbol] = n; }

  INode *find(std::string symbol) {
    auto it = symbols_.find(symbol);
    if (it == symbols_.end())
      return nullptr;
    return it->second;
  }

  void free() {
    for (auto &elt : symbols_)
      delete elt.second;
    symbols_.clear();
  }
};

class Symtab {
  std::unordered_map<IScope *, Symtab_local> tables;

public:
  void add(IScope *s, std::string symbol, INode *n) {
    auto &tab = tables[s];
    tab.add(symbol, n);
  }

  INode *exists(IScope *s, std::string symbol) {
    auto &tab = tables[s];
    return tab.find(symbol);
  }

  void free(IScope *s) {
    auto &tab = tables[s];
    tab.free();
  }
};

extern Symtab globalTable;
