#include <iostream>
#include <thread>
#include <vector>

class Person {
public:
  std::string name;
  Person(std::string _name) : name(_name) {}

  void operator()(int arg) const {
    std::cout << this->name << " says " << arg << "\n";
  };

  void operator()(std::string arg) const {
    std::cout << this->name << " says " << arg << "\n";
  };
};

struct Greeter {
public:
  void hello() { std::cout << "saying hello" << std::endl; };
};

void thread_one(std::vector<Person> &&persons) {
  int count{1};

  while (count <= 20) {
    for (auto &person : persons) {
      if (count % 3 == 0 && count % 5 == 0) {
        person("FizzBuzz!");
      } else if (count % 3 == 0) {
        person("Fizz!");

      } else if (count % 5 == 0) {
        person("Buzz!");
      } else {
        person(count);
      }
      ++count;
    }
  }
}

int main() {
  Person abdul{"Abdul"}, bart{"Bart"}, claudia{"Claudia"}, divya{"Divya"};
  std::vector<Person> persons{abdul, bart, claudia, divya};
  std::thread thr1(thread_one, std::move(persons));
  thr1.join();

  Greeter greet;
  std::thread thr(&Greeter::hello, &greet);
  thr.join();

  int i = 1;
  std::thread thr2([&i] { i *= 2; });
  thr2.join();
  std::cout << "\nint i was 1 and is now = " << i << "\n";

  std::thread thr3(
      [](int a, int b) {
        std::cout << "the sum of " << a << " + " << b << " is " << a + b
                  << "\n";
      },
      4, 19);
  thr3.join();
  return 0;
}
