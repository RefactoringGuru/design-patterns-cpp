#include <iostream>
#include <string>
#include <vector>

/**
 * EN: Chain of Responsibility Design Pattern
 *
 * Intent: Lets you pass requests along a chain of handlers. Upon receiving a
 * request, each handler decides either to process the request or to pass it to
 * the next handler in the chain.
 *
 * RU: Паттерн Цепочка обязанностей
 *
 * Назначение: Позволяет передавать запросы последовательно по цепочке
 * обработчиков. Каждый последующий обработчик решает, может ли он обработать
 * запрос сам и стоит ли передавать запрос дальше по цепи.
 */
/**
 * EN: The Handler interface declares a method for building the chain of
 * handlers. It also declares a method for executing a request.
 *
 * RU: Интерфейс Обработчика объявляет метод построения цепочки обработчиков. Он
 * также объявляет метод для выполнения запроса.
 */
class Handler {
 public:
  virtual Handler *SetNext(Handler *handler) = 0;
  virtual std::string Handle(std::string request) = 0;
};
/**
 * EN: The default chaining behavior can be implemented inside a base handler
 * class.
 *
 * RU: Поведение цепочки по умолчанию может быть реализовано внутри базового
 * класса обработчика.
 */
class AbstractHandler : public Handler {
  /**
     * @var Handler
     */
 private:
  Handler *next_handler_;

 public:
  AbstractHandler() : next_handler_(nullptr) {
  }
  Handler *SetNext(Handler *handler) override {
    this->next_handler_ = handler;
    // EN: Returning a handler from here will let us link handlers in a
    // convenient way like this:
    // $monkey->setNext($squirrel)->setNext($dog);
    //
    // RU: Возврат обработчика отсюда позволит связать обработчики простым
    // способом, вот так:
    // $monkey->setNext($squirrel)->setNext($dog);
    return handler;
  }
  std::string Handle(std::string request) override {
    if (this->next_handler_) {
      return this->next_handler_->Handle(request);
    }

    return {};
  }
};
/**
 * EN: All Concrete Handlers either handle a request or pass it to the next
 * handler in the chain.
 *
 * RU: Все Конкретные Обработчики либо обрабатывают запрос, либо передают его
 * следующему обработчику в цепочке.
 */
class MonkeyHandler : public AbstractHandler {
 public:
  std::string Handle(std::string request) override {
    if (request == "Banana") {
      return "Monkey: I'll eat the " + request + ".\n";
    } else {
      return AbstractHandler::Handle(request);
    }
  }
};
class SquirrelHandler : public AbstractHandler {
 public:
  std::string Handle(std::string request) override {
    if (request == "Nut") {
      return "Squirrel: I'll eat the " + request + ".\n";
    } else {
      return AbstractHandler::Handle(request);
    }
  }
};
class DogHandler : public AbstractHandler {
 public:
  std::string Handle(std::string request) override {
    if (request == "MeatBall") {
      return "Dog: I'll eat the " + request + ".\n";
    } else {
      return AbstractHandler::Handle(request);
    }
  }
};
/**
 * EN: The client code is usually suited to work with a single handler. In most
 * cases, it is not even aware that the handler is part of a chain.
 *
 * RU: Обычно клиентский код приспособлен для работы с единственным
 * обработчиком. В большинстве случаев клиенту даже неизвестно, что этот
 * обработчик является частью цепочки.
 */
void ClientCode(Handler &handler) {
  std::vector<std::string> food = {"Nut", "Banana", "Cup of coffee"};
  for (const std::string &f : food) {
    std::cout << "Client: Who wants a " << f << "?\n";
    const std::string result = handler.Handle(f);
    if (!result.empty()) {
      std::cout << "  " << result;
    } else {
      std::cout << "  " << f << " was left untouched.\n";
    }
  }
}
/**
 * EN: The other part of the client code constructs the actual chain.
 *
 * RU: Другая часть клиентского кода создает саму цепочку.
 */
int main() {
  MonkeyHandler *monkey = new MonkeyHandler;
  SquirrelHandler *squirrel = new SquirrelHandler;
  DogHandler *dog = new DogHandler;
  monkey->SetNext(squirrel)->SetNext(dog);

  /**
     * EN: The client should be able to send a request to any handler, not just the
     * first one in the chain.
     *
     * RU: Клиент должен иметь возможность отправлять запрос любому обработчику, а
     * не только первому в цепочке.
     */
  std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
  ClientCode(*monkey);
  std::cout << "\n";
  std::cout << "Subchain: Squirrel > Dog\n\n";
  ClientCode(*squirrel);

  delete monkey;
  delete squirrel;
  delete dog;

  return 0;
}
