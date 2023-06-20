namespace meta_functions {
  //Counting Sqrt
  template<
    size_t Num,
    size_t InfRoot = 0,
    size_t SupRoot = (Num + 1) / 2,
    bool Found = (SupRoot - InfRoot) <= 1>
  struct Sqrt {
    enum {
      MidRoot = (InfRoot + SupRoot) / 2,
      NewInf = MidRoot > Num / MidRoot ? InfRoot : MidRoot,
      NewSup = MidRoot > Num / MidRoot ? MidRoot : SupRoot
    };
    static const size_t value = Sqrt<Num, NewInf, NewSup>::value;
  };
  template<size_t Num, size_t InfRoot, size_t SupRoot>
  struct Sqrt<Num, InfRoot, SupRoot, true> {
    static const size_t value = (SupRoot * SupRoot <= Num) ? SupRoot : InfRoot;
  };
  
  //Checking if Number IsPrime
  template<size_t Num, size_t Div>
  struct IsPrimeHelper {
    static const size_t
      value = Num % Div != 0 && IsPrimeHelper<Num, Div - 1>::value;
  };
  //Base
  template<size_t Num>
  struct IsPrimeHelper<Num, 1> {
    static const size_t value = true;
  };
  //Main Meta-function
  template<size_t Num>
  struct IsPrime {
    static const size_t value = IsPrimeHelper<Num, Sqrt<Num>::value>::value;
  };
  //Exception
  template<>
  struct IsPrime<1> {
    static const size_t value = false;
  };
}