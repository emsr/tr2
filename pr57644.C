
struct foo
{
  unsigned i: 32;
};

int
main()
{
  foo f {};
  return (f.i);
}
