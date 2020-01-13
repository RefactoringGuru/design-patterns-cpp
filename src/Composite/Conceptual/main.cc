
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
/**
 * EN: Composite Design Pattern
 *
 * Intent: Lets you compose objects into tree structures and then work with
 * these structures as if they were individual objects.
 *
 * RU: Паттерн Компоновщик
 *
 * Назначение: Позволяет сгруппировать объекты в древовидную структуру, а затем
 * работать с ними так, как будто это единичный объект.
 */
/**
 * EN: The base Component class declares common operations for both simple and
 * complex objects of a composition.
 *
 * RU: Базовый класс Компонент объявляет общие операции как для простых, так и
 * для сложных объектов структуры.
 */
class Component {
  /**
     * @var Component
     */
 protected:
  Component *parent_;
  /**
     * EN: Optionally, the base Component can declare an interface for setting
     * and accessing a parent of the component in a tree structure. It can also
     * provide some default implementation for these methods.
     *
     * RU: При необходимости базовый Компонент может объявить интерфейс для
     * установки и получения родителя компонента в древовидной структуре. Он
     * также может предоставить некоторую реализацию по умолчанию для этих
     * методов.
     */
 public:
  virtual ~Component() {}
  void SetParent(Component *parent) {
    this->parent_ = parent;
  }
  Component *GetParent() const {
    return this->parent_;
  }
  /**
     * EN: In some cases, it would be beneficial to define the child-management
     * operations right in the base Component class. This way, you won't need to
     * expose any concrete component classes to the client code, even during the
     * object tree assembly. The downside is that these methods will be empty
     * for the leaf-level components.
     *
     * RU: В некоторых случаях целесообразно определить операции управления
     * потомками прямо в базовом классе Компонент. Таким образом, вам не нужно
     * будет предоставлять конкретные классы компонентов клиентскому коду, даже
     * во время сборки дерева объектов. Недостаток такого подхода в том, что эти
     * методы будут пустыми для компонентов уровня листа.
     */
  virtual void Add(Component *component) {}
  virtual void Remove(Component *component) {}
  /**
     * EN: You can provide a method that lets the client code figure out whether
     * a component can bear children.
     *
     * RU: Вы можете предоставить метод, который позволит клиентскому коду
     * понять, может ли компонент иметь вложенные объекты.
     */
  virtual bool IsComposite() const {
    return false;
  }
  /**
     * EN: The base Component may implement some default behavior or leave it to
     * concrete classes (by declaring the method containing the behavior as
     * "abstract").
     *
     * RU: Базовый Компонент может сам реализовать некоторое поведение по
     * умолчанию или поручить это конкретным классам, объявив метод, содержащий
     * поведение абстрактным.
     */
  virtual std::string Operation() const = 0;
};
/**
 * EN: The Leaf class represents the end objects of a composition. A leaf can't
 * have any children.
 *
 * Usually, it's the Leaf objects that do the actual work, whereas Composite
 * objects only delegate to their sub-components.
 *
 * RU: Класс Лист представляет собой конечные объекты структуры. Лист не может
 * иметь вложенных компонентов.
 *
 * Обычно объекты Листьев выполняют фактическую работу, тогда как объекты
 * Контейнера лишь делегируют работу своим подкомпонентам.
 */
class Leaf : public Component {
 public:
  std::string Operation() const override {
    return "Leaf";
  }
};
/**
 * EN: The Composite class represents the complex components that may have
 * children. Usually, the Composite objects delegate the actual work to their
 * children and then "sum-up" the result.
 *
 * RU: Класс Контейнер содержит сложные компоненты, которые могут иметь
 * вложенные компоненты. Обычно объекты Контейнеры делегируют фактическую работу
 * своим детям, а затем «суммируют» результат.
 */
class Composite : public Component {
  /**
     * @var \SplObjectStorage
     */
 protected:
  std::list<Component *> children_;

 public:
  /**
     * EN: A composite object can add or remove other components (both simple or
     * complex) to or from its child list.
     *
     * RU: Объект контейнера может как добавлять компоненты в свой список
     * вложенных компонентов, так и удалять их, как простые, так и сложные.
     */
  void Add(Component *component) override {
    this->children_.push_back(component);
    component->SetParent(this);
  }
  /**
     * EN: Have in mind that this method removes the pointer to the list but doesn't frees the 
     *     memory, you should do it manually or better use smart pointers.
     *
     * RU: 
     */
  void Remove(Component *component) override {
    children_.remove(component);
    component->SetParent(nullptr);
  }
  bool IsComposite() const override {
    return true;
  }
  /**
     * EN: The Composite executes its primary logic in a particular way. It
     * traverses recursively through all its children, collecting and summing
     * their results. Since the composite's children pass these calls to their
     * children and so forth, the whole object tree is traversed as a result.
     *
     * RU: Контейнер выполняет свою основную логику особым образом. Он проходит
     * рекурсивно через всех своих детей, собирая и суммируя их результаты.
     * Поскольку потомки контейнера передают эти вызовы своим потомкам и так
     * далее, в результате обходится всё дерево объектов.
     */
  std::string Operation() const override {
    std::string result;
    for (const Component *c : children_) {
      if (c == children_.back()) {
        result += c->Operation();
      } else {
        result += c->Operation() + "+";
      }
    }
    return "Branch(" + result + ")";
  }
};
/**
 * EN: The client code works with all of the components via the base interface.
 *
 * RU: Клиентский код работает со всеми компонентами через базовый интерфейс.
 */
void ClientCode(Component *component) {
  // ...
  std::cout << "RESULT: " << component->Operation();
  // ...
}

/**
 * EN: Thanks to the fact that the child-management operations are declared in
 * the base Component class, the client code can work with any component, simple
 * or complex, without depending on their concrete classes.
 *
 * RU: Благодаря тому, что операции управления потомками объявлены в базовом
 * классе Компонента, клиентский код может работать как с простыми, так и со
 * сложными компонентами, вне зависимости от их конкретных классов.
 */
void ClientCode2(Component *component1, Component *component2) {
  // ...
  if (component1->IsComposite()) {
    component1->Add(component2);
  }
  std::cout << "RESULT: " << component1->Operation();
  // ...
}

/**
 * EN: This way the client code can support the simple leaf components...
 *
 * RU: Таким образом, клиентский код может поддерживать простые
 * компоненты-листья...
 */

int main() {
  Component *simple = new Leaf;
  std::cout << "Client: I've got a simple component:\n";
  ClientCode(simple);
  std::cout << "\n\n";
  /**
     * EN: ...as well as the complex composites.
     *
     * RU: ...а также сложные контейнеры.
     */

  Component *tree = new Composite;
  Component *branch1 = new Composite;

  Component *leaf_1 = new Leaf;
  Component *leaf_2 = new Leaf;
  Component *leaf_3 = new Leaf;
  branch1->Add(leaf_1);
  branch1->Add(leaf_2);
  Component *branch2 = new Composite;
  branch2->Add(leaf_3);
  tree->Add(branch1);
  tree->Add(branch2);
  std::cout << "Client: Now I've got a composite tree:\n";
  ClientCode(tree);
  std::cout << "\n\n";

  std::cout << "Client: I don't need to check the components classes even when managing the tree:\n";
  ClientCode2(tree, simple);
  std::cout << "\n";

  delete simple;
  delete tree;
  delete branch1;
  delete branch2;
  delete leaf_1;
  delete leaf_2;
  delete leaf_3;

  return 0;
}
