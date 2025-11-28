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

void thread_one() {

  Person abdul{"Abdul"}, bart{"Bart"}, claudia{"Claudia"}, divya{"Divya"};
  std::vector<Person> persons{abdul, bart, claudia, divya};
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
  std::thread thr1(thread_one);
  thr1.join();
  return 0;
}
