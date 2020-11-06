#include <fstream>

int main(int argc, char ** argv)
{
    std::ofstream ofs;
    ofs.open("testfile");
    ofs << argv[1];
    ofs.close();
}
