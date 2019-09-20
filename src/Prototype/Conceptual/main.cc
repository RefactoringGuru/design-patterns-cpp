#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>

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


enum BulletType {
	SIMPLE=0,
	GOOD
};

/**
 * EN: The example class that has cloning ability. We'll see how the values of
 * field with different types will be cloned.
 *
 * RU: Пример класса, имеющего возможность клонирования. Мы посмотрим как
 * происходит клонирование значений полей разных типов.
 */


class Bullet {

protected:
	string bullet_name_;
	float speed_;
	float fire_power_;
	float direction_;
	
public:

	Bullet() {}
	Bullet(string bullet_name, float speed,float fire_power)
		:bullet_name_(bullet_name),speed_(speed),fire_power_(fire_power){
	}
	virtual ~Bullet() {}
	virtual Bullet* Clone() const = 0;
	virtual void Fire(float direction)= 0;
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

class SimpleBullet: public Bullet {
    private:
    float damage_power_;
public:
	SimpleBullet(string bullet_name, float speed, float fire_power, float damage_power) 
        :Bullet(bullet_name, speed/2, fire_power),damage_power_(damage_power) {
	
	}

    /**
     * EN: Notice that Clone method return a Pointer to a new Bullet replica. so, the client
     * (who call the clone method) has the responsability to free that memory. I you have
     * smart pointer knowledge you may prefer to use unique_pointer here.
     *
     * RU: 
     */
	Bullet* Clone() const override {
		return new SimpleBullet(*this);
	}

    void Fire(float direction) override {
		this->direction_ = direction;
		std::cout << "fire Simple bullet with direction : " << direction<<std::endl;
	}
};

class GoodBullet : public Bullet {
private:
float damage_power_;
float damage_area_;
public:
	GoodBullet(string bullet_name, float speed, float fire_power, float damage_power,float damage_area) 
        :Bullet(bullet_name, speed, fire_power),damage_power_(damage_power),damage_area_(damage_area) {

	}
	Bullet* Clone() const override {
		return new GoodBullet(*this);
	}

    void Fire(float direction) override {
		this->direction_ = direction;
		std::cout << "fire Good Bullet with direction :"<< direction<<std::endl;
	}
};

/**
 * EN: In Bullet Factory you have two protorypes of each bullet, so each time 
 * you want to create a bullet , you can use the existing ones and clone those.
 *
 * RU: 
 */

class BulletFactory {

private: 
	std::unordered_map<BulletType, Bullet* ,std::hash<int> >bullets_;
public:
	BulletFactory() {
	
		bullets_[BulletType::SIMPLE] = new SimpleBullet("Simple Bullet",50.f,75.f,75.f);
		bullets_[BulletType::GOOD] = new GoodBullet ("Good Bullet", 60.f, 95.f, 95.f,200.f);
	}

    /**
     * EN: Be carefull of free all memory allocated. Again, if you have smart pointers knowelege
     * will be better to use it here.
     *
     * RU: 
     */

    ~BulletFactory(){
        delete bullets_[BulletType::SIMPLE];
        delete bullets_[BulletType::GOOD];
    }

    /**
     * EN: Notice here that you just need to specify the type of the bullet you want and the method
     * will create from the object with this type.
     *
     * RU: 
     */
	Bullet* CreateBullet(BulletType bulletType) {
		return bullets_[bulletType]->Clone();
	}
};

void Client(BulletFactory& bullet_factory){
    
    std::cout <<"Let's create a simple bullet\n";
    
    Bullet* bullet = bullet_factory.CreateBullet(BulletType::SIMPLE);
	bullet->Fire(90);
    delete bullet;

    std::cout << "\n";

    std::cout <<"Let's create a Good bullet\n";

	bullet = bullet_factory.CreateBullet(BulletType::GOOD);
	bullet->Fire(10);

    delete bullet;
}


int main(){
    BulletFactory* bullet_factory= new BulletFactory();
    Client(*bullet_factory);
    delete bullet_factory;
	
    return 0;
}