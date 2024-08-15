#include <numeric>
#include <iostream>

namespace MetaFunctions {
//Counting Sqrt
template<
    ssize_t Num,
    ssize_t InfRoot = 0,
    ssize_t SupRoot = (Num + 1) / 2,
    bool Found = (SupRoot - InfRoot) <= 1>
struct Sqrt {
  enum class Bounds : ssize_t {
    MidRoot = std::midpoint(InfRoot, SupRoot),
    NewInf = MidRoot > Num / MidRoot ? InfRoot : MidRoot,
    NewSup = MidRoot > Num / MidRoot ? MidRoot : SupRoot
  };
  static constexpr ssize_t val =
      Sqrt<Num, static_cast<ssize_t>(Bounds::NewInf), static_cast<ssize_t>(Bounds::NewSup)>::val;
};

template<ssize_t Num, ssize_t InfRoot, ssize_t SupRoot>
struct Sqrt<Num, InfRoot, SupRoot, true> {
  static constexpr ssize_t val = (SupRoot * SupRoot <= Num) ? SupRoot : InfRoot;
};

template<ssize_t Num>
inline constexpr ssize_t Sqrt_v = Sqrt<Num>::val;

//Checking if Number IsPrime
template<ssize_t Num, ssize_t Div>
struct IsPrimeHelper {
  static constexpr ssize_t val = Num % Div != 0 && IsPrimeHelper<Num, Div - 1>::val;
};
//Base
template<ssize_t Num>
struct IsPrimeHelper<Num, 1> {
  static constexpr ssize_t val = true;
};
//Main Meta-function
template<ssize_t Num>
struct IsPrime {
  static constexpr ssize_t val = IsPrimeHelper<Num, Sqrt<Num>::val>::val;
};
//Exception
template<>
struct IsPrime<1> {
  static constexpr ssize_t val = false;
};
}
