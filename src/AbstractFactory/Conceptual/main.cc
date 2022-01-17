#include <iostream>
#include <string>

/**
 * EN: Abstract Factory Design Pattern
 *
 * Intent: Lets you produce families of related objects without specifying their
 * concrete classes.
 *
 * RU: Паттерн Абстрактная Фабрика
 *
 * Назначение: Предоставляет интерфейс для создания семейств связанных или
 * зависимых объектов без привязки к их конкретным классам.
 */

/**
 * EN: Each distinct product of a product family should have a base interface.
 * All variants of the product must implement this interface.
 *
 * RU: Каждый отдельный продукт семейства продуктов должен иметь базовый
 * интерфейс. Все вариации продукта должны реализовывать этот интерфейс.
*/
class AbstractProductA {
 public:
  virtual ~AbstractProductA(){};
  virtual std::string UsefulFunctionA() const = 0;
};

/**
 * EN: Concrete Products are created by corresponding Concrete Factories.
 *
 * RU: Конкретные продукты создаются соответствующими Конкретными Фабриками.
 */
class ConcreteProductA1 : public AbstractProductA {
 public:
  std::string UsefulFunctionA() const override {
    return "The result of the product A1.";
  }
};

class ConcreteProductA2 : public AbstractProductA {
  std::string UsefulFunctionA() const override {
    return "The result of the product A2.";
  }
};

/**
 * EN: Here's the the base interface of another product. All products can
 * interact with each other, but proper interaction is possible only between
 * products of the same concrete variant.
 *
 * RU: Базовый интерфейс другого продукта. Все продукты могут взаимодействовать
 * друг с другом, но правильное взаимодействие возможно только между продуктами
 * одной и той же конкретной вариации.
 */
class AbstractProductB {
  /**
   * EN: Product B is able to do its own thing...
   *
   * RU: Продукт B способен работать самостоятельно...
   */
 public:
  virtual ~AbstractProductB(){};
  virtual std::string UsefulFunctionB() const = 0;
  /**
   * EN: ...but it also can collaborate with the ProductA.
   *
   * The Abstract Factory makes sure that all products it creates are of the
   * same variant and thus, compatible.
   *
   * RU: ...а также взаимодействовать с Продуктами A той же вариации.
   *
   * Абстрактная Фабрика гарантирует, что все продукты, которые она создает,
   * имеют одинаковую вариацию и, следовательно, совместимы.
   */
  virtual std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const = 0;
};

/**
 * EN: Concrete Products are created by corresponding Concrete Factories.
 *
 * RU: Конкретные Продукты создаются соответствующими Конкретными Фабриками.
 */
class ConcreteProductB1 : public AbstractProductB {
 public:
  std::string UsefulFunctionB() const override {
    return "The result of the product B1.";
  }
  /**
   * EN: The variant, Product B1, is only able to work correctly with the
   * variant, Product A1. Nevertheless, it accepts any instance of
   * AbstractProductA as an argument.
   *
   * RU: Продукт B1 может корректно работать только с Продуктом A1. Тем не
   * менее, он принимает любой экземпляр Абстрактного Продукта А в качестве
   * аргумента.
   */
  std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const override {
    const std::string result = collaborator.UsefulFunctionA();
    return "The result of the B1 collaborating with ( " + result + " )";
  }
};

class ConcreteProductB2 : public AbstractProductB {
 public:
  std::string UsefulFunctionB() const override {
    return "The result of the product B2.";
  }
  /**
   * EN: The variant, Product B2, is only able to work correctly with the
   * variant, Product A2. Nevertheless, it accepts any instance of
   * AbstractProductA as an argument.
   *
   * RU: Продукт B2 может корректно работать только с Продуктом A2. Тем не
   * менее, он принимает любой экземпляр Абстрактного Продукта А в качестве
   * аргумента.
   */
  std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const override {
    const std::string result = collaborator.UsefulFunctionA();
    return "The result of the B2 collaborating with ( " + result + " )";
  }
};

/**
 * EN: The Abstract Factory interface declares a set of methods that return
 * different abstract products. These products are called a family and are
 * related by a high-level theme or concept. Products of one family are usually
 * able to collaborate among themselves. A family of products may have several
 * variants, but the products of one variant are incompatible with products of
 * another.
 *
 * RU: Интерфейс Абстрактной Фабрики объявляет набор методов, которые возвращают
 * различные абстрактные продукты. Эти продукты называются семейством и связаны
 * темой или концепцией высокого уровня. Продукты одного семейства обычно могут
 * взаимодействовать между собой. Семейство продуктов может иметь несколько
 * вариаций, но продукты одной вариации несовместимы с продуктами другой.
 */
class AbstractFactory {
 public:
  virtual ~AbstractFactory() = default;
  virtual AbstractProductA *CreateProductA() const = 0;
  virtual AbstractProductB *CreateProductB() const = 0;
};

/**
 * EN: Concrete Factories produce a family of products that belong to a single
 * variant. The factory guarantees that resulting products are compatible. Note
 * that signatures of the Concrete Factory's methods return an abstract product,
 * while inside the method a concrete product is instantiated.
 *
 * RU: Конкретная Фабрика производит семейство продуктов одной вариации. Фабрика
 * гарантирует совместимость полученных продуктов. Обратите внимание, что
 * сигнатуры методов Конкретной Фабрики возвращают абстрактный продукт, в то
 * время как внутри метода создается экземпляр конкретного продукта.
 */
class ConcreteFactory1 : public AbstractFactory {
 public:
  AbstractProductA *CreateProductA() const override {
    return new ConcreteProductA1();
  }
  AbstractProductB *CreateProductB() const override {
    return new ConcreteProductB1();
  }
};

/**
 * EN: Each Concrete Factory has a corresponding product variant.
 *
 * RU: Каждая Конкретная Фабрика имеет соответствующую вариацию продукта.
 */
class ConcreteFactory2 : public AbstractFactory {
 public:
  AbstractProductA *CreateProductA() const override {
    return new ConcreteProductA2();
  }
  AbstractProductB *CreateProductB() const override {
    return new ConcreteProductB2();
  }
};

/**
 * EN: The client code works with factories and products only through abstract
 * types: AbstractFactory and AbstractProduct. This lets you pass any factory or
 * product subclass to the client code without breaking it.
 *
 * RU: Клиентский код работает с фабриками и продуктами только через абстрактные
 * типы: Абстрактная Фабрика и Абстрактный Продукт. Это позволяет передавать
 * любой подкласс фабрики или продукта клиентскому коду, не нарушая его.
 */

void ClientCode(const AbstractFactory &factory) {
  const AbstractProductA *product_a = factory.CreateProductA();
  const AbstractProductB *product_b = factory.CreateProductB();
  std::cout << product_b->UsefulFunctionB() << "\n";
  std::cout << product_b->AnotherUsefulFunctionB(*product_a) << "\n";
  delete product_a;
  delete product_b;
}

int main() {
  std::cout << "Client: Testing client code with the first factory type:\n";
  ConcreteFactory1 *f1 = new ConcreteFactory1();
  ClientCode(*f1);
  delete f1;
  std::cout << std::endl;
  std::cout << "Client: Testing the same client code with the second factory type:\n";
  ConcreteFactory2 *f2 = new ConcreteFactory2();
  ClientCode(*f2);
  delete f2;
  return 0;
}