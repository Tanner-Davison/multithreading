#include <print>
// use Meyers Singleton
/* just create Lazy initialization like
 *
 * static LazyInit ptest;
 *
 * */
class LazyInit {
  public:
    LazyInit() {
        std::println("===Test Constructor called===\n");
    }
    void func() {};
};
