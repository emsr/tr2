
// /home/ed/bin/bin/g++ -g -std=c++1y -o test_filesystem test_filesystem.cpp filesystem.cc unique_path.cc

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_filesystem

// LD_LIBRARY_PATH=/home/ed/bin/lib64:$LD_LIBRARY_PATH ./test_filesystem > test.txt 2> test.err


#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "filesystem"
#include "fstream.h"


std::string type[]{
    "status_error",
    "file_not_found",
    "regular_file",
    "directory_file",
    "symlink_file",
    "block_file",
    "character_file",
    "fifo_file",
    "socket_file",
    "type_unknown"
};


void
table(std::ostream& out, const std::tbd::filesystem::path &p)
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
  //out << '\t' << p.stem();
  //out << '\t' << p.extension();
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
main(int, char **)
{
  std::tbd::filesystem::path empty;
  std::cout << "empty: " << empty << '\n';
  std::cout << "path empty: " << empty.empty() << '\n';
  try
  {
    std::cout << "is_empty: " << std::tbd::filesystem::is_empty(empty) << '\n';
  }
  catch(std::tbd::filesystem::filesystem_error& fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }

  std::vector<char> charvec(26);
  std::iota(std::begin(charvec), std::end(charvec), 'a');
  std::tbd::filesystem::path vecpath(charvec);
  std::cout << "vecpath: " << vecpath << '\n';

  std::list<char> charlist(26);
  std::iota(std::begin(charlist), std::end(charlist), 'A');
  std::tbd::filesystem::path listpath(charlist);
  std::cout << "listpath: " << listpath << '\n';

  std::tbd::filesystem::path path(std::string("./filesystem"));
  std::cout << "path: " << path << '\n';
  std::tbd::filesystem::file_status path_stat = std::tbd::filesystem::status(path);
  std::cout << "path type: " << type[int(path_stat.type())] << '\n';
  std::tbd::filesystem::space_info sp = std::tbd::filesystem::space(path);
  std::cout << "space: " << sp.capacity << "  " << sp.free << "  " << sp.available << '\n';

  std::tbd::filesystem::path ccpath("./filesystem.cc");
  std::cout << "ccpath: " << ccpath << '\n';
  std::tbd::filesystem::file_status ccpath_stat = std::tbd::filesystem::status(ccpath);
  std::cout << "path type: " << type[int(ccpath_stat.type())] << '\n';
  std::tbd::filesystem::space_info ccsp = std::tbd::filesystem::space(ccpath);
  std::cout << "space: " << ccsp.capacity << "  " << ccsp.free << "  " << ccsp.available << '\n';

  std::tbd::filesystem::path link(path.string() + ".link");
  std::cout << "link: " << link << '\n';
  std::tbd::filesystem::create_symlink(path, link);
  path_stat = std::tbd::filesystem::status(link);
  std::cout << "path type: " << type[int(path_stat.type())] << '\n';
  std::tbd::filesystem::file_status link_stat = std::tbd::filesystem::symlink_status(link);
  std::cout << "link type: " << type[int(link_stat.type())] << '\n';

  std::tbd::filesystem::remove(link);
  try
  {
    link_stat = std::tbd::filesystem::symlink_status(link);
  }
  catch (std::tbd::filesystem::filesystem_error& fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }
  std::cout << "path type: " << type[int(link_stat.type())] << '\n';

  std::tbd::filesystem::path hl1(path.string() + ".hl1");
  std::cout << "hard link 1: " << hl1 << '\n';
  std::tbd::filesystem::create_hard_link(path, hl1);
  std::tbd::filesystem::file_status hl1_stat = std::tbd::filesystem::status(hl1);
  std::cout << "hard link 1 type: " << type[int(hl1_stat.type())] << '\n';
  std::cout << "hard link count: " << hard_link_count(path) << '\n';
  std::cout << "hard link count: " << hard_link_count(hl1) << '\n';

  std::tbd::filesystem::remove(hl1);
  try
  {
    hl1_stat = std::tbd::filesystem::status(hl1);
  }
  catch (std::tbd::filesystem::filesystem_error& fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }
  std::cout << "hard link 1 type: " << type[int(hl1_stat.type())] << '\n';
  std::cout << "hard link count: " << hard_link_count(path) << '\n';

  std::cout << "current_path: " << std::tbd::filesystem::current_path() << '\n';

  std::cout << "unique_path: " << std::tbd::filesystem::unique_path() << '\n';

  std::cout << "temp_directory_path: " << std::tbd::filesystem::temp_directory_path() << '\n';

  std::cout << '\n';
  std::tbd::filesystem::path var("/var");
  std::cout << var << '\n';
  std::cout << var / "/spool" << '\n';

  std::cout << '\n';
  std::tbd::filesystem::path walk1("//net/usr/local/winonas/big/brown/beaver.dat");
  std::cout << walk1 << '\n';
  for (const auto& c : walk1)
    std::cout << " " << c;
  std::cout << '\n';

  std::cout << '\n' << '\n';
  std::tbd::filesystem::path walk2("/usr/bin");
  std::cout << walk2 << '\n';
  for (const auto& c : walk2)
    std::cout << " " << c;
  std::cout << '\n';

  std::cout << '\n';
  std::tbd::filesystem::path walk3("/uranus/hertz/");
  std::cout << walk3 << '\n';
  for (const auto& c : walk3)
    std::cout << " " << c;
  std::cout << '\n';

  std::cout << '\n';
  std::tbd::filesystem::path walk4("///A////B/////C//////");
  std::cout << walk4 << '\n';
  for (const auto& c : walk4)
    std::cout << " " << c;
  std::cout << '\n';

  std::cout << '\n';
  std::tbd::filesystem::path wbbb("//net/usr/local/winonas/big/brown/beaver.dat");
  std::cout << "wbbb: " << wbbb << '\n';
  std::cout << "wbbb.string(): " << wbbb.string() << '\n';
  ////std::wcout << "wbbb.string<std::wstring>(): " << wbbb.string<std::wstring>() << '\n';
  std::tbd::filesystem::path::iterator it = wbbb.begin();
  std::cout << "*it: " << *it << '\n';
  std::cout << "*it++: " << *it++ << '\n';
  std::cout << '\n';
  std::cout << "*wbbb.begin(): " << *wbbb.begin() << '\n';

  for (auto f : wbbb)
    std::cout << " " << f;
  std::cout << '\n';

  std::tbd::filesystem::path sluggo("sluggo");
  std::tbd::filesystem::create_directory(sluggo);
  std::tbd::filesystem::remove(sluggo);

  std::tbd::filesystem::path lwbbb("./winonas/big/brown/beaver.dat");
  std::cout << '\n' << "create_directories(" << lwbbb << ")" << '\n';
  std::cout << lwbbb << '\n';
  for (auto f : lwbbb)
    std::cout << " " << f;
  std::cout << '\n';
  std::tbd::filesystem::create_directories(lwbbb);
  std::cout << "is_directory(" << lwbbb << "): " << std::tbd::filesystem::is_directory(lwbbb) << '\n';

  std::cout << '\n' << "remove_all(" << lwbbb << ")" << '\n';
  std::tbd::filesystem::remove_all(lwbbb);
  std::cout << "is_directory(" << lwbbb << "): " << std::tbd::filesystem::is_directory(lwbbb) << '\n';
  std::cout << "is_directory(" << lwbbb.parent_path() << "): " << std::tbd::filesystem::is_directory(lwbbb.parent_path()) << '\n';
  std::cout << "remove_all(" << lwbbb.parent_path() << ")" << '\n';
  std::tbd::filesystem::remove_all(lwbbb.parent_path());
  std::cout << "is_directory(" << lwbbb.parent_path() << "): " << std::tbd::filesystem::is_directory(lwbbb.parent_path()) << '\n';

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
    std::tbd::filesystem::recursive_directory_iterator db2ls("/usr/local/bin/db2ls");
    for(; db2ls != std::tbd::filesystem::recursive_directory_iterator(); ++db2ls)
      std::cout << tab(db2ls.level()) << db2ls->path().filename() << '\n';
  }
  catch (std::tbd::filesystem::filesystem_error & fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }

  std::cout << '\n' << "Iterate over \".\"" << '\n';
  std::tbd::filesystem::directory_iterator di(".");
  for(; di != std::tbd::filesystem::directory_iterator(); ++di)
    std::cout << di->path().filename() << '\n';

  std::cout << '\n' << "Recurse into \".\"" << '\n';
  std::tbd::filesystem::recursive_directory_iterator rdi(".");
  for(; rdi != std::tbd::filesystem::recursive_directory_iterator(); ++rdi)
    std::cout << tab(rdi.level()) << rdi->path().filename() << '\n';

  std::cout << '\n' << "Iterate over \"/usr/local\"" << '\n';
  std::tbd::filesystem::directory_iterator diul("/usr/local");
  for(; diul != std::tbd::filesystem::directory_iterator(); ++diul)
    std::cout << diul->path().filename() << '\n';

  try
  {
    std::cout << '\n' << "Recurse into \"/usr/local\"" << '\n';
    std::tbd::filesystem::recursive_directory_iterator rdiul("/usr/local");
    for(; rdiul != std::tbd::filesystem::recursive_directory_iterator(); ++rdiul)
      std::cout << tab(rdiul.level()) << rdiul->path().filename() << '\n';
  }
  catch (std::tbd::filesystem::filesystem_error & fe)
  {
    std::cerr << "error: " << fe.what() << '\n';
    std::cerr << "path : " << fe.path1() << ", " << fe.path2() << '\n';
  }

  std::tbd::filesystem::path newpath;
  std::istringstream strn("\" &\" uranus &\" hertz &\" \"");
  std::cout << "Input path: " << strn.str() << '\n';
  strn >> newpath;
  std::cout << "Output path: " << newpath << '\n';

  //  This is bad: It shouldn't compile.  It picks the template<Source> ctor.
  std::tbd::filesystem::path badpath(std::tbd::filesystem::file_status());

  std::tbd::filesystem::ofstream ofwbbb(wbbb);
  ofwbbb << "Primus Sucks!!!\n";

  std::tbd::filesystem::ifstream ifwbbb(wbbb);
  std::string line;
  std::getline(ifwbbb, line);

  std::tbd::filesystem::create_file("new file");

  //auto path16 = path.string<std::u16string>();

  return 0;
}

// br 'std::tbd::filesystem::recursive_directory_iterator::increment(std::error_code&)'
