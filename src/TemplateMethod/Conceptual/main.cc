#include <iostream>

/**
 * EN: Template Method Design Pattern
 *
 * Intent: Defines the skeleton of an algorithm in the superclass but lets
 * subclasses override specific steps of the algorithm without changing its
 * structure.
 *
 * RU: Паттерн Шаблонный метод
 *
 * Назначение: Определяет общую схему алгоритма, перекладывая реализацию
 * некоторых шагов на подклассы. Шаблонный метод позволяет подклассам
 * переопределять отдельные шаги алгоритма без изменения структуры алгоритма.
 */
/**
 * EN: The Abstract Class defines a template method that contains a skeleton of
 * some algorithm, composed of calls to (usually) abstract primitive operations.
 *
 * Concrete subclasses should implement these operations, but leave the template
 * method itself intact.
 *
 * RU: Абстрактный Класс определяет шаблонный метод, содержащий скелет
 * некоторого алгоритма, состоящего из вызовов (обычно) абстрактных примитивных
 * операций.
 *
 * Конкретные подклассы должны реализовать эти операции, но оставить сам
 * шаблонный метод без изменений.
 */
class AbstractClass {
  /**
     * EN: The template method defines the skeleton of an algorithm.
     *
     * RU: Шаблонный метод определяет скелет алгоритма.
     */
 public:
  void TemplateMethod() const {
    this->BaseOperation1();
    this->RequiredOperations1();
    this->BaseOperation2();
    this->Hook1();
    this->RequiredOperation2();
    this->BaseOperation3();
    this->Hook2();
  }
  /**
     * EN: These operations already have implementations.
     *
     * RU: Эти операции уже имеют реализации.
     */
 protected:
  void BaseOperation1() const {
    std::cout << "AbstractClass says: I am doing the bulk of the work\n";
  }
  void BaseOperation2() const {
    std::cout << "AbstractClass says: But I let subclasses override some operations\n";
  }
  void BaseOperation3() const {
    std::cout << "AbstractClass says: But I am doing the bulk of the work anyway\n";
  }
  /**
     * EN: These operations have to be implemented in subclasses.
     *
     * RU: А эти операции должны быть реализованы в подклассах.
     */
  virtual void RequiredOperations1() const = 0;
  virtual void RequiredOperation2() const = 0;
  /**
     * EN: These are "hooks." Subclasses may override them, but it's not
     * mandatory since the hooks already have default (but empty)
     * implementation. Hooks provide additional extension points in some crucial
     * places of the algorithm.
     *
     * RU: Это «хуки». Подклассы могут переопределять их, но это не обязательно,
     * поскольку у хуков уже есть стандартная (но пустая) реализация. Хуки
     * предоставляют дополнительные точки расширения в некоторых критических
     * местах алгоритма.
     */
  virtual void Hook1() const {}
  virtual void Hook2() const {}
};
/**
 * EN: Concrete classes have to implement all abstract operations of the base
 * class. They can also override some operations with a default implementation.
 *
 * RU: Конкретные классы должны реализовать все абстрактные операции базового
 * класса. Они также могут переопределить некоторые операции с реализацией по
 * умолчанию.
 */
class ConcreteClass1 : public AbstractClass {
 protected:
  void RequiredOperations1() const override {
    std::cout << "ConcreteClass1 says: Implemented Operation1\n";
  }
  void RequiredOperation2() const override {
    std::cout << "ConcreteClass1 says: Implemented Operation2\n";
  }
};
/**
 * EN: Usually, concrete classes override only a fraction of base class'
 * operations.
 *
 * RU: Обычно конкретные классы переопределяют только часть операций базового
 * класса.
 */
class ConcreteClass2 : public AbstractClass {
 protected:
  void RequiredOperations1() const override {
    std::cout << "ConcreteClass2 says: Implemented Operation1\n";
  }
  void RequiredOperation2() const override {
    std::cout << "ConcreteClass2 says: Implemented Operation2\n";
  }
  void Hook1() const override {
    std::cout << "ConcreteClass2 says: Overridden Hook1\n";
  }
};
/**
 * EN: The client code calls the template method to execute the algorithm.
 * Client code does not have to know the concrete class of an object it works
 * with, as long as it works with objects through the interface of their base
 * class.
 *
 * RU: Клиентский код вызывает шаблонный метод для выполнения алгоритма.
 * Клиентский код не должен знать конкретный класс объекта, с которым работает,
 * при условии, что он работает с объектами через интерфейс их базового класса.
 */
void ClientCode(AbstractClass *class_) {
  // ...
  class_->TemplateMethod();
  // ...
}

int main() {
  std::cout << "Same client code can work with different subclasses:\n";
  ConcreteClass1 *concreteClass1 = new ConcreteClass1;
  ClientCode(concreteClass1);
  std::cout << "\n";
  std::cout << "Same client code can work with different subclasses:\n";
  ConcreteClass2 *concreteClass2 = new ConcreteClass2;
  ClientCode(concreteClass2);
  delete concreteClass1;
  delete concreteClass2;
  return 0;
}
