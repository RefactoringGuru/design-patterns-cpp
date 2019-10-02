#include <iostream>

/**
 * EN: Singleton Design Pattern
 *
 * Intent: Lets you ensure that a class has only one instance, while providing a
 * global access point to this instance.
 *
 * RU: Паттерн Одиночка
 *
 * Назначение: Гарантирует, что у класса есть только один экземпляр, и
 * предоставляет к нему глобальную точку доступа.
 */
/**
 * EN: The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 *
 * RU: Класс Одиночка предоставляет метод `GetInstance`, который ведёт себя как
 * альтернативный конструктор и позволяет клиентам получать один и тот же
 * экземпляр класса при каждом вызове.
 */
class Singleton
{

    /**
     * EN: The Singleton's constructor should always be private to prevent
     * direct construction calls with the `new` operator.
     *
     * RU: Конструктор Одиночки всегда должен быть скрытым, чтобы предотвратить
     * создание объекта через оператор new.
     */

protected:
    Singleton()
    {
    }

public:
    /**
     * EN: Singletons should not be cloneable.
     *
     * RU: Одиночки не должны быть клонируемыми.
     */
    Singleton(Singleton &other) = delete;
    /**
     * EN: Singletons should not be assignable.
     *
     * RU:
     */
    void operator=(const Singleton &) = delete;
    /**
     * EN: This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     *
     * This implementation lets you subclass the Singleton class while keeping
     * just one instance of each subclass around.
     *
     * RU: Это статический метод, управляющий доступом к экземпляру одиночки.
     * При первом запуске, он создаёт экземпляр одиночки и помещает его в
     * статическое поле. При последующих запусках, он возвращает клиенту объект,
     * хранящийся в статическом поле.
     *
     * Эта реализация позволяет вам расширять класс Одиночки, сохраняя повсюду
     * только один экземпляр каждого подкласса.
     */

    static Singleton *GetInstance();
    /**
     * EN: Finally, any singleton should define some business logic, which can
     * be executed on its instance.
     *
     * RU: Наконец, любой одиночка должен содержать некоторую бизнес-логику,
     * которая может быть выполнена на его экземпляре.
     */
    void SomeBusinessLogic()
    {
        // ...
    }
};

/**
     * EN: Static methods should be defined outside the class.
     *
     * RU: 
     */
Singleton *Singleton::GetInstance()
{
    /**
     * EN: This is a safer way to create an instance. instance = new Singleton is dangeruous 
     * in case two instance threads wants to access at the same time
     *
     * RU: 
     */
    static Singleton singleton;
    return &singleton;
}

/**
 * EN: The client code.
 *
 * RU: Клиентский код.
 */
void ClientCode()
{
    Singleton *s1 = Singleton::GetInstance();
    Singleton *s2 = Singleton::GetInstance();
    if (s1 == s2)
    {
        std::cout << "Singleton works, both variables contain the same instance.\n";
    }
    else
    {
        std::cout << "Singleton failed, variables contain different instances.\n";
    }
    delete s1;
}

int main()
{
    ClientCode();
    return 0;
}