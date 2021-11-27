//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Some details of noexcept expressions
//
//----------------------------------------------------------------------------

#include <iostream>
#include <type_traits>

struct DefaultCtor{};

struct ThrowingCtor{
  ThrowingCtor() {}; // =default will create noexcept one
  ThrowingCtor(const ThrowingCtor&) = default;
  ThrowingCtor(ThrowingCtor&&) = default;
};

struct Inherited{
  ThrowingCtor c;
};

void foo(int) noexcept;
void foo(DefaultCtor) noexcept;
void foo(ThrowingCtor) noexcept;

int main() {
  std::cout << std::boolalpha;
  std::cout << "noexcept(null pointer deref): "<< noexcept(*static_cast<int *>(nullptr)) << std::endl;

  std::cout << "foo(int): "<< noexcept(foo(1)) << std::endl;
  std::cout << "foo(Default): "<< noexcept(foo(DefaultCtor{})) << std::endl;
  std::cout << "foo(Throwing): " << noexcept(foo(ThrowingCtor{})) << std::endl;

  std::cout << "Default constr: " << std::is_nothrow_constructible<DefaultCtor>::value << std::endl;
  std::cout << "Default copy constr: " << std::is_nothrow_copy_constructible<DefaultCtor>::value << std::endl;
  std::cout << "Default move constr: " << std::is_nothrow_move_constructible<DefaultCtor>::value << std::endl;

  std::cout << "Inherited constr: " << std::is_nothrow_constructible<Inherited>::value << std::endl;
  std::cout << "Inherited copy constr: " << std::is_nothrow_copy_constructible<Inherited>::value << std::endl;
  std::cout << "Inherited move constr: " << std::is_nothrow_move_constructible<Inherited>::value << std::endl;
}
