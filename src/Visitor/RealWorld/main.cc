/**
 * EN: Real World Example of the Visitor Design Pattern (Modern C++17 Standard)
 *
 * Need: Consider a restaurant \c Menu represented as a heterogeneous \c Item
 * collection of different \c Food and \c Drink items, which must be
 * (homogeneously) serialised into RFC 8259 JSON for some external API usage.
 *
 * Solution: A modern C++17 standard \c Serialiser Visitor be easily implemented
 * using the built-in utilities found within the \c <variant> header, namely,
 * the type-safe union \c std::variant to represent different menu items and the
 * functor \c std::visit to apply a callable \c Serialiser visitor.
 *
 * This simpler ("KISS") and boilerplate-free implementation of the Visitor
 * Design Pattern surpasses the classical object-oriented programming Visitor
 * that often requires maintaining two separate, but cyclically interdependent,
 * class hierarchies:
 *
 *     \c Item<-Food/Drink and \c Visitor<-FoodVisitor/DrinkVisitor
 *
 * and suffers from performance penalties associated with the virtual function
 * calls in the double dispatch.
 *
 * In this contemporary take on the Visitor Design Pattern here, the (SOLID)
 * Open-Closed Principle is more expressively fulfilled because the \c Food and
 * \c Drink classes do not need to be derived from some base \c Item class and
 * also do not need to be updated with \c AcceptVisitor methods. The absence of
 * any intrusive polymorphism provides greater flexibility; this means that new
 * types (i.e. new \c Item types such as \c Snack ) and new visitors (i.e. ) are
 * more straightforward to incorporate.
 *
 * For such a \e procedural Visitor Design Pattern, performances gains can be
 * expected if the \c std::variant uses value semantics rather than reference
 * semantics, and if the collection storage is continguous, that is, instead of
 * the memory-scattering pointer indirections of the traditional Visitor Design
 * Pattern applied to multiple types.
 */

#include <iostream>
#include <string>
#include <variant>
#include <vector>

/**
 * EN: Stable Low-Lying Data Structures for Food, Drink,...
 *
 * Respecting the Open-Closed Principle, there is no need to modify these
 * classes to accept the visitors that are to be introduced later. Observe that
 * these \c Item classes are not part of an inheritance hierarchy and so there
 * is flexibility to create more such \c Item classes.
 *
 * However, note that these classes require a complete definition here in lieu
 * of their upcoming role within the \c std::variant union, that is, a forward
 * declaration of these classes is not sufficient. The public API consists of an
 * explicit constructor and the necessary access methods that are required by
 * the \c Serialiser Visitor.
 */
class Food {
public:
  enum Label : unsigned { meat, fish, vegetarian, vegan };

public:
  explicit Food(std::string name, std::size_t calories, Label label)
      : name_{name}, calories_{calories}, label_{label} {}

  auto name() const noexcept { return name_; }
  auto calories() const noexcept { return calories_; }
  auto label() const noexcept {
    switch (label_) {
    case Label::meat:
      return "meat";
    case Label::fish:
      return "fish";
    case Label::vegetarian:
      return "vegetarian";
    case Label::vegan:
      return "vegan";
    default:
      return "unknown";
    }
  }

private:
  std::string name_;
  std::size_t calories_;
  Label label_;
};

class Drink {
public:
  enum Label : unsigned { alcoholic, hot, cold };

public:
  explicit Drink(std::string name, std::size_t volume, Label label)
      : name_{name}, volume_{volume}, label_{label} {}

  auto name() const noexcept { return name_; }
  auto volume() const noexcept { return volume_; }
  auto label() const noexcept {
    switch (label_) {
    case Label::alcoholic:
      return "alcholic";
    case Label::hot:
      return "hot";
    case Label::cold:
      return "cold";
    default:
      return "unknown";
    }
  }

private:
  std::string name_;
  std::size_t volume_;
  Label label_;
};

/* ... */

/**
 * EN: Variant Union of the Item and Menu as an Item Collection
 *
 * The \c Item and \c Menu aliases carve out an architectural boundary
 * separating the low-lying data structures (above) and the client-facing
 * visitor (below), the former being more established in the codebase and the
 * latter being perhaps newer and often more changeable. Also note the value
 * semantics, which means there is no need for manual dynamic memory allocation
 * or management (e.g. via smart pointers) and hence lower overall complexity
 * when it comes to implementing the Visitor Design Pattern.
 *
 * For best performance, it is recommended to use \c Item types of similar, if
 * not identical, sizes so that the memory layout can be optimised. (If there
 * are considerable differences in the class sizes, then it may be sensible to
 * use the Proxy Design Pattern to wrap around larger-sized classes or even the
 * Bridge Design Pattern/"pimpl" idiom.) The memory layout of the members within
 * each of the \c Item classes themselves may also be of importance to overall
 * performance (c.f. padding) in this implementation as the \c std::visit method
 * will be applied to each \c Item element by iterating over the \c Menu
 * container.
 */
using Item = std::variant<Food, Drink /* ... */>;
using Menu = std::vector<Item>;

/**
 * EN: Serialiser Visitor Functor
 *
 * This basic \c Serialiser class has non-canonical operator() overloads
 * which take the different \c Item types as input arguments, define lambdas to
 * perform a rudimentary conversion of the data to compressed/minified JSON
 * using the public API of the classes, and then print out the converted result
 * to some \c std::ostream by invoking the lambdas. Each \c Item has its own
 * unique overloaded operator() definition, which makes this class a prime
 * candidate for the Strategy Design Pattern e.g. different JSON specifications.
 */
class Serialiser {
public:
  explicit Serialiser(std::ostream &os = std::cout) : os_{os} {}

public:
  auto operator()(Food const &food) const {

    auto to_json = [&](auto f) {
      return R"({"item":"food","name":")" + f.name() + R"(","calories":")" +
             std::to_string(f.calories()) + R"(kcal","label":")" + f.label() +
             R"("})";
    };

    os_ << to_json(food);
  }
  auto operator()(Drink const &drink) const {
    auto to_json = [&](auto d) {
      return R"({"item":"drink","name":")" + d.name() + R"(","volume":")" +
             std::to_string(d.volume()) + R"(ml","label":")" + d.label() +
             R"("})";
    };
    os_ << to_json(drink);
  }
  /* ... */

private:
  std::ostream &os_{std::cout};
};

/* ... */

/**
 * EN: Applied Visitor for Menu (Item Collection) Serialisation
 *
 * The callable/invokable \c Serialiser Visitor can now be applied to each of
 * the \c Item elements in the \c Menu via the \c std::visit utility method, the
 * internal machinery of which could somewhat vary between different compilers
 * (e.g. GCC, Clang, MSVC, etc.) and their versions. Nevertheless, as a staple
 * part of the standard library from C++17 onwards, \c std::visit reliably and
 * conveniently automates the required boilerplate code and thereby reduces the
 * implementational friction that accompanies the traditional object-oriented
 * Visitor Design Pattern.
 *
 * Accordingly, it is now possible to perform a simple range-based for loop over
 * the \c Menu collection and apply visitor on each \c Item element in turn,
 * which has the best possible performance if the \c Item elements are stored
 * contiguously as values in memory.
 */
void serialise(Menu const &menu, std::ostream &os = std::cout) {
  bool first{true};
  os << R"({"menu":[)";
  for (auto const &item : menu) {
    if (!first)
      os << ",";
    else
      first = false;
    std::visit(Serialiser{os}, item);
  }
  os << R"(]})";
}

/* ... */

/**
 * EN: Client Code: Variant Visitor
 *
 * The declaration of the \c Menu collection is clean and hassle-free, and the
 * addition of the \c Item elements in form of \c Food and \c Drink class
 * instances is also drastically simplified by the value semantics. Finally, the
 * neat \c serialise method can be called with the \c Menu input argument to
 * demonstrate Modern C++17 Visitor Design Pattern in action.
 */
int main() {

  Menu menu;
  menu.reserve(8);

  menu.emplace_back(Food{"Borscht", 160, Food::Label::meat});
  menu.emplace_back(Food{"Samosa", 250, Food::Label::vegetarian});
  menu.emplace_back(Food{"Sushi", 300, Food::Label::fish});
  menu.emplace_back(Food{"Quinoa", 350, Food::Label::vegan});
  menu.emplace_back(Drink{"Vodka", 25, Drink::Label::alcoholic});
  menu.emplace_back(Drink{"Chai", 120, Drink::Label::hot});
  menu.emplace_back(Drink{"Sake", 180, Drink::Label::alcoholic});
  menu.emplace_back(Drink{"Kola", 355, Drink::Label::cold});
  /* ... */

  serialise(menu);
}