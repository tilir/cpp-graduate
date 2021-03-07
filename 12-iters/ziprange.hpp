//-----------------------------------------------------------------------------
//
// Example simplified from Arthur O'Dwyers zip range to illustrate key concept
// of proxy classes for operator->() and overall iterator development
//
// original: https://quuxplusone.github.io/blog/2019/02/06/arrow-proxy/
//
//-----------------------------------------------------------------------------

#include <iterator>
#include <utility>

namespace itertools {

//-----------------------------------------------------------------------------
//
// Consider a zip_range that refers to two other containers and lets you
// iterate them in parallel:
//
// std::vector<int> keys;
// std:vector<double> values;
// for (auto &&both : itertools::make_zip_range(keys, values))
//   std::cout << both.first << " " << both.second << "\n";
//
// This type is iterator to be returned by zip_range type's begin
//
//-----------------------------------------------------------------------------
template <typename KeyIt, typename ValueIt> class zip_iterator_t {
  KeyIt Kit_;
  ValueIt Vit_;
  using KeyType = typename std::iterator_traits<KeyIt>::value_type;
  using ValueType = typename std::iterator_traits<ValueIt>::value_type;
  using KeyDiff = typename std::iterator_traits<KeyIt>::difference_type;
  using ValueDiff = typename std::iterator_traits<ValueIt>::difference_type;
  using KeyReference = typename std::iterator_traits<KeyIt>::reference;
  using ValueReference = typename std::iterator_traits<ValueIt>::reference;
  using KeyCat = typename std::iterator_traits<KeyIt>::iterator_category;
  using ValueCat = typename std::iterator_traits<KeyIt>::iterator_category;

  // some checks to use this intereface
private:
  static_assert(std::is_base_of<std::forward_iterator_tag, KeyCat>::value,
                "Key shall be at least forward iterable to use this wrapper");
  static_assert(std::is_base_of<std::forward_iterator_tag, ValueCat>::value,
                "Value shall be at least forward iterable to use this wrapper");

  // five mandatory definitions:
  // * iterator_category
  // * difference_type
  // * value_type
  // * reference
  // * pointer
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::pair<KeyDiff, ValueDiff>;
  using value_type = std::pair<KeyType, ValueType>;
  using reference = std::pair<KeyReference, ValueReference>;
  // using pointer = ??? (see below)

  // iterator interface
public:
  zip_iterator_t(KeyIt Kit, ValueIt Vit) : Kit_(Kit), Vit_(Vit) {}

  zip_iterator_t &operator++() {
    ++Kit_;
    ++Vit_;
    return *this;
  }

  zip_iterator_t &operator++(int) {
    auto temp{*this};
    operator++();
    return temp;
  }

  bool equals(const zip_iterator_t &Rhs) const {
    return (Kit_ == Rhs.Kit_) && (Vit_ == Rhs.Vit_);
  }

  // first interesting part: operator*
public:
  reference operator*() const { return {*Kit_, *Vit_}; }

  // array reference details
private:
  template <typename Reference> struct arrow_proxy {
    Reference R;
    Reference *operator->() { return &R; }
  };
  using pointer = arrow_proxy<reference>;

  // second interesting part: operator ->
public:
  pointer operator->() const { return pointer{{*Kit_, *Vit_}}; }
};

// iterator interface: out of class
template <typename KeyIt, typename ValueIt>
bool operator==(const zip_iterator_t<KeyIt, ValueIt> &Lhs,
                const zip_iterator_t<KeyIt, ValueIt> &Rhs) {
  return Lhs.equals(Rhs);
}

template <typename KeyIt, typename ValueIt>
bool operator!=(const zip_iterator_t<KeyIt, ValueIt> &Lhs,
                const zip_iterator_t<KeyIt, ValueIt> &Rhs) {
  return !Lhs.equals(Rhs);
}

template <typename KeyIt, typename ValueIt>
auto make_zip_iterator(KeyIt K, ValueIt V) {
  return zip_iterator_t<KeyIt, ValueIt>{K, V};
}

//-----------------------------------------------------------------------------
//
// zip_range_t is leightweight helper to keep range references
//
//-----------------------------------------------------------------------------
template <typename Keys, typename Values> class zip_range_t {
  Keys &K_;
  Values &V_;
  using KIter = typename std::remove_reference_t<Keys>::iterator;
  using VIter = typename std::remove_reference_t<Values>::iterator;

public:
  zip_range_t(Keys &K, Values &V) : K_(K), V_(V) {}

  // begin and end interface
public:
  zip_iterator_t<KIter, VIter> begin() {
    return make_zip_iterator(std::begin(K_), std::begin(V_));
  }

  zip_iterator_t<KIter, VIter> end() {
    return make_zip_iterator(std::end(K_), std::end(V_));
  }
};

//-----------------------------------------------------------------------------
//
// make_this range helper interface to actually zip ranges
//
//-----------------------------------------------------------------------------
template <typename Keys, typename Values>
auto make_zip_range(Keys &K, Values &V) {
  return zip_range_t<Keys, Values>{K, V};
}

} // namespace itertools