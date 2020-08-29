struct S {
  S *retme() { return this; }
};

S *retfirst(S *ps) { return ps; }

S g;

S *foo() { return g.retme(); }
