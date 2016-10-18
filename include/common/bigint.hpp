#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

template <std::size_t size,
          typename intType = std::uint64_t,
          std::size_t intTypeSize = 64>
//std::enable_if<std::is_unsigned<intType>::value>
class BigInt {
 public:
  //static_assert(std::is_unsigned<intType>::value);
  using BigIntType = BigInt<size, intType, intTypeSize>;
  BigInt() = default;
  BigInt(const BigIntType &other) = default;
  BigInt(BigIntType &&other) = default;
  BigIntType &operator=(const BigIntType &other) = default;
  BigIntType &operator=(BigIntType &&other) = default;
  // binary operators
  BigIntType operator+(const BigIntType &other) const;
  BigIntType operator-(const BigIntType &other) const;
  BigIntType operator*(const BigIntType &other) const;
  BigIntType operator/(const BigIntType &other) const;
  BigIntType operator%(const BigIntType &other) const;
  // unary operators
  BigIntType operator+() const;
  BigIntType operator-() const;
  BigIntType &operator++();  //++a
  BigIntType operator++(int);// a++
  BigIntType &operator--();  //--a
  BigIntType operator--(int);// a--
  // comparison operators
  bool operator==(const BigIntType &other) const;
  bool operator!=(const BigIntType &other) const;
  bool operator>=(const BigIntType &other) const;
  bool operator<=(const BigIntType &other) const;
  bool operator>(const BigIntType &other) const;
  bool operator<(const BigIntType &other) const;
  // bit operators
  BigIntType operator~() const;
  BigIntType operator|(const BigIntType &other) const;
  BigIntType operator&(const BigIntType &other) const;
  BigIntType operator^(const BigIntType &other) const;
  BigIntType operator<<(const BigIntType &other) const;
  BigIntType operator>>(const BigIntType &other) const;
  // compund operators
  BigIntType &operator+=(const BigIntType &other);
  BigIntType &operator-=(const BigIntType &other);
  BigIntType &operator*=(const BigIntType &other);
  BigIntType &operator/=(const BigIntType &other);
  BigIntType &operator%=(const BigIntType &other);
  BigIntType &operator&=(const BigIntType &other);
  BigIntType &operator|=(const BigIntType &other);
  BigIntType &operator^=(const BigIntType &other);
  BigIntType &operator<<=(const BigIntType &other);
  BigIntType &operator>>=(const BigIntType &other);
  // cross int support, this might actually be quite hard D:
  // it actually might be a pain D:
  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, BigIntType>::type &
  operator+=(T &other);
  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, BigIntType>::type &
  operator-=(T &other);
  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, BigIntType>::type &
  operator*=(T &other);
  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, BigIntType>::type &
  operator/=(T &other);
  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, BigIntType>::type &
  operator%=(T &other);
  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, BigIntType>::type &
  operator<<=(T shift);
  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, BigIntType>::type &
  operator>>=(T shift);
  BigIntType operator<<(std::size_t shift) const;
  BigIntType operator>>(std::size_t shift) const;

  operator bool();
  // sar, sal, ror, rol
  // stream stuff

 private:
  static constexpr std::size_t intTypeCount =
      (size + (intTypeSize - 1)) / intTypeSize;
  // std::vector<intType> _data(intTypeCount, static_cast<intType>(0));
  std::vector<intType> _data();

  intType getByte(std::size_t index);

  std::pair<BigIntType, BigIntType> mul(const BigIntType &other) const;
  std::pair<BigIntType, BigIntType> div(const BigIntType &other) const;
  std::vector<BigIntType> pow(const BigIntType &other) const;
};
