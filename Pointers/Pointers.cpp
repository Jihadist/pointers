// Pointers.cpp: определяет точку входа для приложения.
//

#include "Pointers.h"
#include "Shared.h"
#include <memory>

struct Foo {
	Foo() { std::cout << "Foo...\n"; }
	~Foo() { std::cout << "~Foo...\n"; }
};

struct D {
	void operator()(Foo* p) const {
		std::cout << "Call delete for Foo object...\n";
		delete p;
	}
};

int main()
{
	// constructor with no managed object
	std::shared_ptr<Foo> sh1;

	// constructor with object
	std::shared_ptr<Foo> sh2(new Foo);
	std::shared_ptr<Foo> sh3(sh2);
	std::cout << sh2.use_count() << '\n';
	std::cout << sh3.use_count() << '\n';

	//constructor with object and deleter
	std::shared_ptr<Foo> sh4(new Foo, D());
	//Shared_ptr<int> p1;
	//Shared_ptr<int> p2;
	//p1.swap(p2);
	//Shared_ptr<float> p2(13);
		// constructor with no managed object
	Shared_ptr<int> sH1{ new int(42) };
	Shared_ptr<int> sH2 (new int);
	auto  sH3(sH1);
	// constructor with object
	//std::cout << sH2.use_count() << '\n';
	//std::cout << sH3.use_count() << '\n';

	//constructor with object and deleter
//	Shared_ptr<Foo> sH4(new Foo, D());
}
