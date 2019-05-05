#include <elf/elf++.hh>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

#include "patch.hpp"

void foobar()
{
}

void bar()
{
}

struct foo
{
    void bar()
    {
    }
};

int main(int argc, char* argv[])
{
    patch::live_process p;
    std::cout << "real addr: " << std::hex
              << "foobar: " << reinterpret_cast<std::intptr_t>(&foobar)
              << ", bar: " << reinterpret_cast<std::intptr_t>(&bar) << std::endl;
    p.patch("Z6foobarv");

    p.patch_function(&foobar);
    p.patch_function(&foo::bar);
}