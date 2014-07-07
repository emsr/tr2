// /home/ed/bin/bin/g++ -std=c++14 -o test_pr61663 test_pr61445.cpp

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=61445

template<typename FactoryT>
  void newFrontendActionFactory(FactoryT *, int * = 0);

int a;

template<typename FactoryT>
  void
  newFrontendActionFactory(FactoryT *, int *)
  {
    class A
    {
      void
      m_fn1()
      {
        B(0, 0);
      }

      class B
      {
      public:
        B(FactoryT, int);
      };
    };

    newFrontendActionFactory(&a);
  }
