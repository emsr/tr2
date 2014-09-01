
// /home/ed/bin/bin/g++ -g -std=c++14 -fdiagnostics-color=always -o test_filesystem test_filesystem.cpp path.cc directory_iterator.cc recursive_directory_iterator.cc filesystem.cc

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_filesystem

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_filesystem > test.txt 2> test.err

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_filesystem > test.new.txt 2> test.new.err

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_filesystem > test.new2.txt 2> test.new2.err


#include <iostream>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>
#include "filesystem"
#include "fstream.h"


std::string type[]
{
    "not_found", // -1
    "none",
    "regular",
    "directory",
    "symlink",
    "block",
    "character",
    "fifo",
    "socket",
    "unknown"
};


void
table(std::ostream& out, const std::experimental::filesystem::path &p)
{
  out << p.string();
  out << '\t';
  int i = 0;
  for (const auto& c : p)
    out << (i++ > 0 ? ',' : ' ') << c.string();
  out << '\t' << p.string();
  out << '\t' << p.generic_string();
  out << '\t' << p.root_path().string();
  out << '\t' << p.root_name().string();
  out << '\t' << p.root_directory().string();
  out << '\t' << p.relative_path().string();
  out << '\t' << p.parent_path().string();
  out << '\t' << p.filename().string();
  out << '\t' << p.stem();
  out << '\t' << p.extension();
  out << '\n';
}


std::string
tab(int level)
{
  std::string tabs;
  for (int i = 0; i < level; ++i)
    tabs += "\t";
  return tabs;
}


int
main()
{
  std::experimental::filesystem::path empty;
  std::cout << "empty: " << empty << '\n';
  std::cout << "path empty: " << empty.empty() << '\n';
  try
  {
    std::cout << "is_empty: " << std::experimental::filesystem::is_empty(empty) << '\n';
  }
  catch(std::experimental::filesystem::filesystem_error& fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }

  std::array<char, 8 + 1 + 3> chararr;
  std::iota(std::begin(chararr), std::end(chararr), 'a');
  std::experimental::filesystem::path arrpath(chararr);
  std::cout << "arrpath: " << arrpath << '\n';

  std::vector<char> charvec(26);
  std::iota(std::begin(charvec), std::end(charvec), 'a');
  std::experimental::filesystem::path vecpath(charvec);
  std::cout << "vecpath: " << vecpath << '\n';

  std::list<char> charlist(26);
  std::iota(std::begin(charlist), std::end(charlist), 'A');
  std::experimental::filesystem::path listpath(charlist);
  std::cout << "listpath: " << listpath << '\n';

  std::string latin1_string = "Résumé";
  using cvt_t = std::codecvt_byname<wchar_t, char, std::mbstate_t>;
  try
  {
    std::locale latin1_locale{std::locale(),
                              new cvt_t{"fr_FR.ISO-8859-1"}}; // "fr_FR.UTF-8"
    std::experimental::filesystem::path latin1_path{latin1_string, latin1_locale};
    std::cout << "latin1_path.empty() = " << latin1_path.empty() << std::endl;
    std::cout << "latin1_path.string().length() = " << latin1_path.string().length() << std::endl;
    std::cout << "latin1_path.string() = " << latin1_path.string() << std::endl;
    std::cout << "latin1_path.wstring().length() = " << latin1_path.wstring().length() << std::endl;
    std::wcout << L"latin1_path: " << latin1_path.wstring() << std::endl;
    std::experimental::filesystem::create_directory(latin1_path);
  }
  catch(std::experimental::filesystem::filesystem_error& fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }
  catch(std::exception& ex)
  {
    std::cerr << "error: " << ex.what() << '\n';
  }
  catch(...)
  {
  }

  std::experimental::filesystem::path path(std::string("./filesystem"));
  std::cout << "path: " << path << '\n';
  std::experimental::filesystem::file_status path_stat = std::experimental::filesystem::status(path);
  std::cout << "path type: " << type[int(path_stat.type()) + 1] << '\n';
  std::experimental::filesystem::space_info sp = std::experimental::filesystem::space(path);
  std::cout << "space: " << sp.capacity << "  " << sp.free << "  " << sp.available << '\n';

  std::experimental::filesystem::path ccpath("./filesystem.cc");
  std::cout << "ccpath: " << ccpath << '\n';
  std::experimental::filesystem::file_status ccpath_stat = std::experimental::filesystem::status(ccpath);
  std::cout << "path type: " << type[int(ccpath_stat.type()) + 1] << '\n';
  std::experimental::filesystem::space_info ccsp = std::experimental::filesystem::space(ccpath);
  std::cout << "space: " << ccsp.capacity << "  " << ccsp.free << "  " << ccsp.available << '\n';

  std::experimental::filesystem::path link(path.string() + ".link");
  std::cout << "link: " << link << '\n';
  std::experimental::filesystem::create_symlink(path, link);
  path_stat = std::experimental::filesystem::status(link);
  std::cout << "path type: " << type[int(path_stat.type()) + 1] << '\n';
  std::experimental::filesystem::file_status link_stat = std::experimental::filesystem::symlink_status(link);
  std::cout << "link type: " << type[int(link_stat.type()) + 1] << '\n';

  std::experimental::filesystem::remove(link);
  try
  {
    link_stat = std::experimental::filesystem::symlink_status(link);
  }
  catch (std::experimental::filesystem::filesystem_error& fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }
  std::cout << "path type: " << type[int(link_stat.type()) + 1] << '\n';

  std::experimental::filesystem::path hl1(path.string() + ".hl1");
  std::cout << "hard link 1: " << hl1 << '\n';
  std::experimental::filesystem::create_hard_link(path, hl1);
  std::experimental::filesystem::file_status hl1_stat = std::experimental::filesystem::status(hl1);
  std::cout << "hard link 1 type: " << type[int(hl1_stat.type()) + 1] << '\n';
  std::cout << "hard link count: " << hard_link_count(path) << '\n';
  std::cout << "hard link count: " << hard_link_count(hl1) << '\n';

  std::experimental::filesystem::remove(hl1);
  try
  {
    hl1_stat = std::experimental::filesystem::status(hl1);
  }
  catch (std::experimental::filesystem::filesystem_error& fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }
  std::cout << "hard link 1 type: " << type[int(hl1_stat.type()) + 1] << '\n';
  std::cout << "hard link count: " << hard_link_count(path) << '\n';

  std::cout << "current_path: " << std::experimental::filesystem::current_path() << '\n';

  std::experimental::filesystem::path this_test{__FILE__};
  std::cout << "this_test: " << this_test << '\n';
  std::cout << "this_test.has_root_name(): " << this_test.has_root_name() << '\n';
  std::cout << "this_test.has_root_directory(): " << this_test.has_root_directory() << '\n';
  std::cout << "canonical(this_test): " << std::experimental::filesystem::canonical(this_test) << '\n';
  std::cout << "absolute(this_test): " << std::experimental::filesystem::absolute(this_test) << '\n';

  std::experimental::filesystem::file_status this_status = std::experimental::filesystem::status(this_test);
  std::error_code this_error;
  std::cout << "is_block_file: " << std::experimental::filesystem::is_block_file(this_status) << '\n';
  std::cout << "is_block_file: " << std::experimental::filesystem::is_block_file(this_test) << '\n';
  std::cout << "is_block_file: " << std::experimental::filesystem::is_block_file(this_test, this_error) << '\n';

  std::cout << "is_character_file: " << std::experimental::filesystem::is_character_file(this_status) << '\n';
  std::cout << "is_character_file: " << std::experimental::filesystem::is_character_file(this_test) << '\n';
  std::cout << "is_character_file: " << std::experimental::filesystem::is_character_file(this_test, this_error) << '\n';

  std::cout << "is_directory: " << std::experimental::filesystem::is_directory(this_status) << '\n';
  std::cout << "is_directory: " << std::experimental::filesystem::is_directory(this_test) << '\n';
  std::cout << "is_directory: " << std::experimental::filesystem::is_directory(this_test, this_error) << '\n';

  std::cout << "is_empty: " << std::experimental::filesystem::is_empty(this_test) << '\n';
  std::cout << "is_empty: " << std::experimental::filesystem::is_empty(this_test, this_error) << '\n';

  std::cout << "is_fifo: " << std::experimental::filesystem::is_fifo(this_status) << '\n';
  std::cout << "is_fifo: " << std::experimental::filesystem::is_fifo(this_test) << '\n';
  std::cout << "is_fifo: " << std::experimental::filesystem::is_fifo(this_test, this_error) << '\n';

  std::cout << "is_other: " << std::experimental::filesystem::is_other(this_status) << '\n';
  std::cout << "is_other: " << std::experimental::filesystem::is_other(this_test) << '\n';
  std::cout << "is_other: " << std::experimental::filesystem::is_other(this_test, this_error) << '\n';

  std::cout << "is_regular_file: " << std::experimental::filesystem::is_regular_file(this_status) << '\n'; 
  std::cout << "is_regular_file: " << std::experimental::filesystem::is_regular_file(this_test) << '\n';
  std::cout << "is_regular_file: " << std::experimental::filesystem::is_regular_file(this_test, this_error) << '\n';

  std::cout << "is_socket: " << std::experimental::filesystem::is_socket(this_status) << '\n';
  std::cout << "is_socket: " << std::experimental::filesystem::is_socket(this_test) << '\n';
  std::cout << "is_socket: " << std::experimental::filesystem::is_socket(this_test, this_error) << '\n';

  std::cout << "is_symlink: " << std::experimental::filesystem::is_symlink(this_status) << '\n';
  std::cout << "is_symlink: " << std::experimental::filesystem::is_symlink(this_test) << '\n';
  std::cout << "is_symlink: " << std::experimental::filesystem::is_symlink(this_test, this_error) << '\n';

  std::cout << "temp_directory_path: " << std::experimental::filesystem::temp_directory_path() << '\n';

  std::cout << '\n';
  std::experimental::filesystem::path var("/var");
  std::cout << var << '\n';
  std::cout << var / "/spool" << '\n';

  std::cout << '\n';
  std::experimental::filesystem::path walk1("//net/usr/local/winonas/big/brown/beaver.dat");
  std::cout << walk1 << '\n';
  for (const auto& c : walk1)
    std::cout << " " << c;
  std::cout << '\n';

  std::cout << '\n' << '\n';
  std::experimental::filesystem::path walk2("/usr/bin");
  std::cout << walk2 << '\n';
  for (const auto& c : walk2)
    std::cout << " " << c;
  std::cout << '\n';

  std::cout << '\n';
  std::experimental::filesystem::path walk3("/uranus/hertz/");
  std::cout << walk3 << '\n';
  for (const auto& c : walk3)
    std::cout << " " << c;
  std::cout << '\n';

  std::cout << '\n';
  std::experimental::filesystem::path walk4("///A////B/////C//////");
  std::cout << walk4 << '\n';
  for (const auto& c : walk4)
    std::cout << " " << c;
  std::cout << '\n';

  std::cout << '\n';
  std::experimental::filesystem::path wbbb("//net/usr/local/winonas/big/brown/beaver.dat");
  std::cout << "wbbb: " << wbbb << '\n';
  std::cout << "wbbb.string(): " << wbbb.string() << '\n';
  ////std::wcout << "wbbb.string<std::wstring>(): " << wbbb.string<std::wstring>() << '\n';
  std::experimental::filesystem::path::iterator it = wbbb.begin();
  std::cout << "*it: " << *it << '\n';
  std::cout << "*it++: " << *it++ << '\n';
  std::cout << '\n';
  std::cout << "*wbbb.begin(): " << *wbbb.begin() << '\n';

  for (auto f : wbbb)
    std::cout << " " << f;
  std::cout << '\n';

  std::experimental::filesystem::path sluggo("sluggo");
  std::experimental::filesystem::create_directory(sluggo);
  std::experimental::filesystem::remove(sluggo);

  std::error_code ec;
  std::experimental::filesystem::path lwbbb("./winonas/big/brown/beaver.dat");
  std::cout << '\n' << "create_directories(" << lwbbb << ")" << '\n';
  std::cout << lwbbb << '\n';
  for (auto f : lwbbb)
    std::cout << " " << f;
  std::cout << '\n';
  ////std::experimental::filesystem::create_directories(lwbbb);
  std::cout << "is_directory(" << lwbbb << "): " << std::experimental::filesystem::is_directory(lwbbb, ec) << '\n';

  std::cout << '\n' << "remove_all(" << lwbbb << ")" << '\n';
  std::experimental::filesystem::remove_all(lwbbb, ec);
  std::cout << "is_directory(" << lwbbb << "): " << std::experimental::filesystem::is_directory(lwbbb, ec) << '\n';
  std::cout << "is_directory(" << lwbbb.parent_path() << "): " << std::experimental::filesystem::is_directory(lwbbb.parent_path(), ec) << '\n';
  std::cout << "remove_all(" << lwbbb.parent_path() << ")" << '\n';
  std::experimental::filesystem::remove_all(lwbbb.parent_path(), ec);
  std::cout << "is_directory(" << lwbbb.parent_path() << "): " << std::experimental::filesystem::is_directory(lwbbb.parent_path(), ec) << '\n';

  std::cout << '\n' << '\n';
  table(std::cout, "");
  table(std::cout, ".");
  table(std::cout, "..");
  table(std::cout, "foo");
  table(std::cout, "/");
  table(std::cout, "/foo");
  table(std::cout, "foo/");
  table(std::cout, "/foo/");
  table(std::cout, "foo/bar");
  table(std::cout, "/foo/bar");
  table(std::cout, "//net");
  table(std::cout, "//net/foo");
  table(std::cout, "///foo///");
  table(std::cout, "///foo///bar");
  table(std::cout, "/.");
  table(std::cout, "./");
  table(std::cout, "/..");
  table(std::cout, "../");
  table(std::cout, "foo/.");
  table(std::cout, "foo/..");
  table(std::cout, "foo/./");
  table(std::cout, "foo/./bar");
  table(std::cout, "foo/..");
  table(std::cout, "foo/../");
  table(std::cout, "foo/../bar");
  table(std::cout, "c:");
  table(std::cout, "c:/");
  table(std::cout, "c:foo");
  table(std::cout, "c:/foo");
  table(std::cout, "c:foo/");
  table(std::cout, "c:/foo/");
  table(std::cout, "c:/foo/bar");
  table(std::cout, "prn:");
  table(std::cout, "c:\\");
  table(std::cout, "c:foo");
  table(std::cout, "c:\\foo");
  table(std::cout, "c:foo\\");
  table(std::cout, "c:\\foo\\");
  table(std::cout, "c:\\foo/");
  table(std::cout, "c:/foo\\bar");

  try
  {
    std::cout << '\n' << "Recurse into \"/usr/local/bin/db2ls\"" << '\n';
    std::experimental::filesystem::recursive_directory_iterator db2ls("/usr/local/bin/db2ls");
    for(; db2ls != std::experimental::filesystem::recursive_directory_iterator(); ++db2ls)
      std::cout << tab(db2ls.depth()) << db2ls->path().filename() << '\n';
  }
  catch (std::experimental::filesystem::filesystem_error & fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }

  std::cout << '\n' << "Iterate over \".\"" << '\n';
  std::experimental::filesystem::directory_iterator di(".");
  for(; di != std::experimental::filesystem::directory_iterator(); ++di)
    std::cout << di->path().filename() << '\n';

  std::cout << '\n' << "Recurse into \".\"" << '\n';
  std::experimental::filesystem::recursive_directory_iterator rdi(".");
  for(; rdi != std::experimental::filesystem::recursive_directory_iterator(); ++rdi)
    std::cout << tab(rdi.depth()) << rdi->path().filename() << '\n';

  std::cout << '\n' << "Iterate over \"/usr/local\"" << '\n';
  std::experimental::filesystem::directory_iterator diul("/usr/local");
  for(; diul != std::experimental::filesystem::directory_iterator(); ++diul)
    std::cout << diul->path().filename() << '\n';

  try
  {
    std::cout << '\n' << "Recurse into \"/usr/local\"" << '\n';
    std::experimental::filesystem::recursive_directory_iterator rdiul("/usr/local");
    for(; rdiul != std::experimental::filesystem::recursive_directory_iterator(); ++rdiul)
      std::cout << tab(rdiul.depth()) << rdiul->path().filename() << '\n';
  }
  catch (std::experimental::filesystem::filesystem_error & fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }

  std::experimental::filesystem::path newpath;
  std::istringstream strn("\" &\" uranus &\" hertz &\" \"");
  std::cout << "Input path: " << strn.str() << '\n';
  strn >> newpath;
  std::cout << "Output path: " << newpath << '\n';

  //  This is bad: It shouldn't compile.  It picks the template<Source> ctor.
  std::experimental::filesystem::path badpath(std::experimental::filesystem::file_status());

  std::experimental::filesystem::ofstream ofwbbb(wbbb);
  ofwbbb << "Primus Sucks!!!\n";

  std::experimental::filesystem::ifstream ifwbbb(wbbb);
  std::string line;
  std::getline(ifwbbb, line);

  //auto path16 = path.string<std::u16string>();

  return 0;
}

// br 'std::experimental::filesystem::recursive_directory_iterator::increment(std::error_code&)'
