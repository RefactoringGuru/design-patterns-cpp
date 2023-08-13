#include <iostream>
#include <string>
#include <vector>

/**
 * EN: Real World Example for the Builder Design Pattern (C++03/11 Evolution)
 *
 * Need: Consider a representation of the Document Object Model in which
 * each HTML element is a non-trivial graph (multi-way tree) structure
 * whose construction is complicated by the need to add an arbitrary number of
 * children to the root.
 *
 * Solution: A HTML ElementBuilder can be used for stepwise construction of an
 * Element using an implementational variant of the Builder Design Pattern
 * known as the \e Fluent Builder. Although modern C++17/20/23 provides the
 * neccesary built-in language mechanics (i.e. initializer_list and parameter
 * packs) for a Builder, this specific \e Fluent Builder is a class that can be
 * applied to legacy code relying on the older C++03/11 standards.
 */

#include <iostream>
#include <string>
#include <vector>

/**
 * EN: These preprocessor directives allow this standalone code to target both
 * pre- and post-C++11 standards when it comes to std::vector, in particular,
 * appending a new element as well as iterating over all of the elements. In
 * addition, unscoped C++03 and scoped C++11 enums are also handled using the
 * same technique. However, this approach is for only demonstration purposes in
 * order to show the subtle difference in the C++03- and C++11-subvariants of
 * the Fluent Builder as part of the evolution of the design pattern itself.
 */
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || \
    ((!defined(_MSVC_LANG)) && __cplusplus >= 201103L)

#define append_element(tag, content) emplace_back(tag, content)
#define ranged_for(children) for (auto const &child : children)

#define ENUMMERATION_TYPE() enum class
#define TAG_SCOPE() html::Tag

#else

#define append_element(tag, content) push_back(Element(tag, content))
#define ranged_for(children)                                       \
  for (std::vector<Element>::const_iterator it = children.begin(); \
       it != children.end(); ++it)
#define child *it

#define ENUMMERATION_TYPE() enum
#define TAG_SCOPE() html

#endif

/**
 * EN: The html namespace contains the core machinery of the Fluent Builder
 * Pattern, namely, the Element and ElementBuilder classes. To showcase the
 * versatility of the pattern in being able to extend the Element class with
 * different types of HTML elements (tags), a print method that relies on
 * various tags is provided to show the Fluent Builder in action.
 */
namespace html {

/**
 * EN: The forward declaration for the ElementBuilder is necessary as it is
 * a friend class of the Element class in this Fluent Builder implementation.
 */
class ElementBuilder;

/**
 * EN: Enumeration to represent different HTML elements. (Note that in C++11
 * the enumeration will be class-scoped.) There is also a naive helper function
 * to convert the names into strings, which is used inside of the print method.
 */
ENUMMERATION_TYPE() Tag{body, h1, h2, p, /* ... */};

std::string to_string(Tag tag) {
  switch (tag) {
    case TAG_SCOPE()::body:
      return "body";
    case TAG_SCOPE()::h1:
      return "h1";
    case TAG_SCOPE()::h2:
      return "h2";
    case TAG_SCOPE()::p:
      return "p";
    /* ... */
    default:
      return "tag";
  }
}

/**
 * EN: This client-facing Element class is essentially a tree node that
 * stores its children by value in a dynamic container. The Fluent Builder
 * provides a means to construct an instance of root Element node and then add
 * an arbitrary number of children Element nodes.
 */
class Element {
 public:
  Element(Tag tag, std::string const &content = std::string())
      : tag_(tag), content_(content) {}

  /**
   * EN: The print method generates markup. Note that the ranged-based for
   * loop over the children differs between the respective C++03 and C++11
   * standards.
   */
  friend std::ostream &operator<<(std::ostream &os, Element const &e) {
    os << "<" << to_string(e.tag_) << ">";
    if (!e.content_.empty()) {
      os << e.content_;
    } else {
      os << "\n";
    }
    ranged_for(e.children_) { os << child; }
    os << "</" << to_string(e.tag_) << ">\n";
    return os;
  }

 private:
  friend class ElementBuilder;

 private:
  Tag tag_;
  std::string content_;
  std::vector<Element> children_;
};

/**
 * EN: The Fluent Builder is named for its method chaining as the modifier
 * (setter) method add_child() returns the builder itself, and so it can be
 * repeatedly called to construct a complex Element with many Element children.
 *
 * Again note that that element addition operation on the vector of children
 * differs between the C++03 and C++11 standards; in the former case, the
 * Element constructor must be called explicitly whereas in the latter case, the
 * arguments are forwarded to the Element constructor.
 */
class ElementBuilder {
 public:
  explicit ElementBuilder(Tag tag, std::string const &content = std::string())
      : root_(Element(tag, content)) {}

  ElementBuilder &add_child(Tag tag,
                            std::string const &content = std::string()) {
    root_.children_.append_element(tag, content);
    return *this;
  }

  operator Element() const { return root_; }

 private:
  Element root_;
};

}  // namespace html

int main() {
  html::Element body =
      html::ElementBuilder(TAG_SCOPE()::body)
          .add_child(TAG_SCOPE()::h1, "Title of the Page")
          .add_child(TAG_SCOPE()::h2, "Subtitle A")
          .add_child(TAG_SCOPE()::p, "Lorem ipsum dolor sit amet, ...")
          .add_child(TAG_SCOPE()::h2, "Subtitle B")
          .add_child(TAG_SCOPE()::p, "... consectetur adipiscing elit.")
      /* ... */;

  std::cout << body;
}