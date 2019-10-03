#include <iostream>
#include <unordered_map>
#include <string>

using std::string;

// EN: Prototype Design Pattern
//
// Intent: Lets you copy existing objects without making your code dependent on
// their classes.
//
// RU: Паттерн Прототип
//
// Назначение: Позволяет копировать объекты, не вдаваясь в подробности их
// реализации.

enum Type
{
	SIMPLE = 0,
	GOOD
};

/**
 * EN: The example class that has cloning ability. We'll see how the values of
 * field with different types will be cloned.
 *
 * RU: Пример класса, имеющего возможность клонирования. Мы посмотрим как
 * происходит клонирование значений полей разных типов.
 */

class Prototype{

protected:
	string bullet_name_;
	float speed_;
	float fire_power_;
	float direction_;

public:
	Prototype() {}
	Prototype(string bullet_name, float speed, float fire_power)
		: bullet_name_(bullet_name), speed_(speed), fire_power_(fire_power)
	{
	}
	virtual ~Prototype() {}
	virtual Prototype *Clone() const = 0;
	virtual void Fire(float direction){
		this->direction_ = direction;
		std::cout << "fire "<< bullet_name_<<" with direction : " << direction << std::endl;
	}
};

/**
 * EN: Simple Bullet is a Sub-Class of Bullet and implement the Clone Method
 * In this example all data members of Bullet Class are in the Stack. If you
 * have pointers in your properties for ex: String* name_ ,you will need to
 * implement the Copy-Constructor to make sure you have a deep copy from the 
 * clone method
 *
 * RU: 
 */

class ConcretePrototype1 : public Prototype
{
private:
	float damage_power_;

public:
	ConcretePrototype1(string bullet_name, float speed, float fire_power, float damage_power)
		: Prototype(bullet_name, speed / 2, fire_power), damage_power_(damage_power)
	{
	}

	/**
     * EN: Notice that Clone method return a Pointer to a new Bullet replica. so, the client
     * (who call the clone method) has the responsability to free that memory. I you have
     * smart pointer knowledge you may prefer to use unique_pointer here.
     *
     * RU: 
     */
	Prototype *Clone() const override
	{
		return new ConcretePrototype1(*this);
	}
};

class ConcretePrototype2 : public Prototype
{
private:
	float damage_area_;

public:
	ConcretePrototype2(string bullet_name, float speed, float fire_power, float damage_power, float damage_area)
		: Prototype(bullet_name, speed, fire_power), damage_area_(damage_area)
	{
	}
	Prototype *Clone() const override
	{
		return new ConcretePrototype2(*this);
	}
};

/**
 * EN: In Bullet Factory you have two protorypes of each bullet, so each time 
 * you want to create a bullet , you can use the existing ones and clone those.
 *
 * RU: 
 */

class PrototypeFactory
{

private:
	std::unordered_map<Type, Prototype *, std::hash<int>> bullets_;

public:
	PrototypeFactory()
	{

		bullets_[Type::SIMPLE] = new ConcretePrototype1("Simple Bullet", 50.f, 75.f, 75.f);
		bullets_[Type::GOOD] = new ConcretePrototype2("Good Bullet", 60.f, 95.f, 95.f, 200.f);
	}

	/**
     * EN: Be carefull of free all memory allocated. Again, if you have smart pointers knowelege
     * will be better to use it here.
     *
     * RU: 
     */

	~PrototypeFactory()
	{
		delete bullets_[Type::SIMPLE];
		delete bullets_[Type::GOOD];
	}

	/**
     * EN: Notice here that you just need to specify the type of the bullet you want and the method
     * will create from the object with this type.
     *
     * RU: 
     */
	Prototype *CreateBullet(Type Type)
	{
		return bullets_[Type]->Clone();
	}
};

void Client(PrototypeFactory &bullet_factory)
{

	std::cout << "Let's create a simple bullet\n";

	Prototype *prototype = bullet_factory.CreateBullet(Type::SIMPLE);
	prototype->Fire(90);
	delete prototype;

	std::cout << "\n";

	std::cout << "Let's create a Good bullet\n";

	prototype = bullet_factory.CreateBullet(Type::GOOD);
	prototype->Fire(10);

	delete prototype;
}

int main()
{
	PrototypeFactory *bullet_factory = new PrototypeFactory();
	Client(*bullet_factory);
	delete bullet_factory;

	return 0;
}