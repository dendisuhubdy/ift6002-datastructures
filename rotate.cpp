/* See http://blog.regehr.org/archives/1063
 */
#include <type_traits>
#include <cstdint>
#include <limits>
#include <typeinfo>

#ifndef ARGTYPE
#define ARGTYPE uint32_t
#endif /* ARGTYPE */

using argtype = ARGTYPE;

template <typename T>
T
rotl (T v, unsigned int b)
{
  static_assert(std::is_integral<T>::value, "rotate of non-integral type");
  static_assert(! std::is_signed<T>::value, "rotate of signed type");
  constexpr unsigned int num_bits {std::numeric_limits<T>::digits};
  static_assert(0 == (num_bits & (num_bits - 1)), "rotate value bit length not power of two");
  constexpr unsigned int count_mask {num_bits - 1};
  const unsigned int mb {b & count_mask};
  using promoted_type = typename std::common_type<int, T>::type;
  using unsigned_promoted_type = typename std::make_unsigned<promoted_type>::type;
  return ((unsigned_promoted_type{v} << mb)
          | (unsigned_promoted_type{v} >> (-mb & count_mask)));
}

argtype
rotl32d (argtype v,
         unsigned int b)
{
  return rotl(v, b);
}

#include <iostream>
#include <bitset>

int
main (int argc, char * argv[])
{
  using ostype = std::common_type<unsigned int, argtype>::type;
  argtype v {0x1};
  int b{8};

  if (1 < argc) {
    v = std::stoul(argv[1]);
  }
  if (2 < argc) {
    b = std::stoi(argv[2]);
  }
  std::cout << typeid(argtype).name()
            << " isint "
            << std::is_integral<argtype>::value
            << "\n";
  std::cout << std::hex
    << static_cast<ostype>(v) << " rotl " << b << " is "
    << static_cast<ostype>(rotl(v, b)) << "\n";
  return 0;
}
