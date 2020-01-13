#include <iostream>
#include <string>

/**
 * EN: Command Design Pattern
 *
 * Intent: Turns a request into a stand-alone object that contains all
 * information about the request. This transformation lets you parameterize
 * methods with different requests, delay or queue a request's execution, and
 * support undoable operations.
 *
 * RU: Паттерн Команда
 *
 * Назначение: Превращает запросы в объекты, позволяя передавать их как
 * аргументы при вызове методов, ставить запросы в очередь, логировать их, а
 * также поддерживать отмену операций.
 */
/**
 * EN: The Command interface declares a method for executing a command.
 *
 * RU: Интерфейс Команды объявляет метод для выполнения команд.
 */
class Command {
 public:
  virtual ~Command() {
  }
  virtual void Execute() const = 0;
};
/**
 * EN: Some commands can implement simple operations on their own.
 *
 * RU: Некоторые команды способны выполнять простые операции самостоятельно.
 */
class SimpleCommand : public Command {
 private:
  std::string pay_load_;

 public:
  explicit SimpleCommand(std::string pay_load) : pay_load_(pay_load) {
  }
  void Execute() const override {
    std::cout << "SimpleCommand: See, I can do simple things like printing (" << this->pay_load_ << ")\n";
  }
};

/**
 * EN: The Receiver classes contain some important business logic. They know how
 * to perform all kinds of operations, associated with carrying out a request.
 * In fact, any class may serve as a Receiver.
 *
 * RU: Классы Получателей содержат некую важную бизнес-логику. Они умеют
 * выполнять все виды операций, связанных с выполнением запроса. Фактически,
 * любой класс может выступать Получателем.
 */
class Receiver {
 public:
  void DoSomething(const std::string &a) {
    std::cout << "Receiver: Working on (" << a << ".)\n";
  }
  void DoSomethingElse(const std::string &b) {
    std::cout << "Receiver: Also working on (" << b << ".)\n";
  }
};

/**
 * EN: However, some commands can delegate more complex operations to other
 * objects, called "receivers."
 *
 * RU: Но есть и команды, которые делегируют более сложные операции другим
 * объектам, называемым «получателями».
 */
class ComplexCommand : public Command {
  /**
     * @var Receiver
     */
 private:
  Receiver *receiver_;
  /**
     * EN: Context data, required for launching the receiver's methods.
     *
     * RU: Данные о контексте, необходимые для запуска методов получателя.
     */
  std::string a_;
  std::string b_;
  /**
     * EN: Complex commands can accept one or several receiver objects along
     * with any context data via the constructor.
     *
     * RU: Сложные команды могут принимать один или несколько
     * объектов-получателей вместе с любыми данными о контексте через
     * конструктор.
     */
 public:
  ComplexCommand(Receiver *receiver, std::string a, std::string b) : receiver_(receiver), a_(a), b_(b) {
  }
  /**
     * EN: Commands can delegate to any methods of a receiver.
     *
     * RU: Команды могут делегировать выполнение любым методам получателя.
     */
  void Execute() const override {
    std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
    this->receiver_->DoSomething(this->a_);
    this->receiver_->DoSomethingElse(this->b_);
  }
};

/**
 * EN: The Invoker is associated with one or several commands. It sends a
 * request to the command.
 *
 * RU: Отправитель связан с одной или несколькими командами. Он отправляет
 * запрос команде.
 */
class Invoker {
  /**
     * @var Command
     */
 private:
  Command *on_start_;
  /**
     * @var Command
     */
  Command *on_finish_;
  /**
     * EN: Initialize commands.
     *
     * RU: Инициализация команд.
     */
 public:
  ~Invoker() {
    delete on_start_;
    delete on_finish_;
  }

  void SetOnStart(Command *command) {
    this->on_start_ = command;
  }
  void SetOnFinish(Command *command) {
    this->on_finish_ = command;
  }
  /**
     * EN: The Invoker does not depend on concrete command or receiver classes.
     * The Invoker passes a request to a receiver indirectly, by executing a
     * command.
     *
     * RU: Отправитель не зависит от классов конкретных команд и получателей.
     * Отправитель передаёт запрос получателю косвенно, выполняя команду.
     */
  void DoSomethingImportant() {
    std::cout << "Invoker: Does anybody want something done before I begin?\n";
    if (this->on_start_) {
      this->on_start_->Execute();
    }
    std::cout << "Invoker: ...doing something really important...\n";
    std::cout << "Invoker: Does anybody want something done after I finish?\n";
    if (this->on_finish_) {
      this->on_finish_->Execute();
    }
  }
};
/**
 * EN: The client code can parameterize an invoker with any commands.
 *
 * RU: Клиентский код может параметризовать отправителя любыми командами.
 */

int main() {
  Invoker *invoker = new Invoker;
  invoker->SetOnStart(new SimpleCommand("Say Hi!"));
  Receiver *receiver = new Receiver;
  invoker->SetOnFinish(new ComplexCommand(receiver, "Send email", "Save report"));
  invoker->DoSomethingImportant();

  delete invoker;
  delete receiver;

  return 0;
}
