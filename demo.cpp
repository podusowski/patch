#include <elf/elf++.hh>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

void foobar()
{
}

int main(int argc, char* argv[])
{
    int fd = open("/proc/self/exe", O_RDONLY);
    elf::elf f(elf::create_mmap_loader(fd));


    for (auto &sec : f.sections()) {
        if (sec.get_hdr().type != elf::sht::symtab && sec.get_hdr().type != elf::sht::dynsym)
                continue;

        printf("Symbol table '%s':\n", sec.get_name().c_str());
        printf("%6s: %-16s %-5s %-7s %-7s %-5s %s\n",
                "Num", "Value", "Size", "Type", "Binding", "Index",
                "Name");
        int i = 0;
        for (auto sym : sec.as_symtab()) {
                auto &d = sym.get_data();
                std::cout << 
                        d.value << d.size
                        << to_string(d.type()).c_str()
                        << to_string(d.binding()).c_str()
                        << to_string(d.shnxd).c_str()
                        << sym.get_name().c_str() << std::endl;
        }
    }
}