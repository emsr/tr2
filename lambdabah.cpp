// /home/ed/bin/bin/g++ -std=c++1y -o lambdabah lambdabah.cpp

// 'Identity' is a lambda that accepts an argument of any type and returns the value of its parameter.

auto Identity = [](auto a) { return a; };

int three = Identity(3);

char const* hello = Identity("hello");

 // Conversion to function pointer for capture-less lambdas

int (*fpi)(int) = Identity;

char (*fpc)(char) = Identity;

auto glambda = [](int i, auto a) { return i; }; // OK: a generic lambda

// Gnu extension to allow auto in regular functions...
auto
boogerz(auto a)
{
  return a + decltype(a){1};
}

//---------------------------------------------------------------
//  From http://isocpp.org/files/papers/N3649.html

auto glambda2 = [](auto a, auto&& b) { return a < b; };

bool b = glambda2(3, 3.14);       // OK

auto
vglambda = [](auto printer)
           {
             return [=](auto&& ... ts)
                    {   // OK: ts is a  function parameter pack
                      printer(std::forward<decltype(ts)>(ts)...);
                      return [=](){ printer(ts ...); };
                    };
           };

auto p = vglambda([](auto v1, auto v2, auto v3){ std::cout << v1 << v2 << v3; });

auto q = p(1, 'a', 3.14);  // OK: outputs 1a3.14

q();                       // OK: outputs 1a3.14

//---------------------------------------------------------------

void f1(int (*)(int)) { }
void f2(char (*)(int)) { }

void g(int (*)(int)) { }   // #1
void g(char (*)(char)) { } // #2

void h(int (*)(int)) { }   // #3
void h(char (*)(int)) { }  // #4

auto glambda3 = [](auto a) { return a; };

f1(glambda3); // OK
f2(glambda3); // error: ID is not convertible

g(glambda3); // error: ambiguous
h(glambda3); // OK: calls #3 since it is convertible from ID

int& (*fpi)(int*) = [](auto* a) -> auto& { return *a; }; // OK

//---------------------------------------------------------------

void f(int, const int (&)[2] = {}) { }   // #1

void f(const int&, const int (&)[1]) { }   // #2

void
test()
{
  const int x = 17;

  auto g = [](auto a){ f(x); }; // OK: calls #1, does not capture x

  auto g2 = [=](auto a)
            {
              int selector[sizeof(a) == 1 ? 1 : 2]{};
              f(x, selector);  // OK: is a dependent expression, so captures x
            };
}

//---------------------------------------------------------------

int
main()
{
}
