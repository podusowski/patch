#include <elf/elf++.hh>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>

namespace patch
{

namespace detail
{

auto create_elf_reader()
{
    // no leak here, elf closes fd by itself
    int fd = open("/proc/self/exe", O_RDONLY);
    return elf::elf{elf::create_mmap_loader(fd)};
}

} // namespace detail

struct live_process
{
    // Z6foobarv
    auto patch(const std::string& f)
    {
        print_maps();
        std::cout << "patching: " << f << std::endl;
        auto s = find_function(f);
        std::cout << "addr: " << std::hex << s.get_data().value << std::endl;
    }

    template<class T>
    auto patch_function(T&& f)
    {
        std::cout << "patching function: " << std::hex << reinterpret_cast<std::intptr_t>(f) << std::endl;
    }

private:
    void print_maps() const
    {
        std::ifstream f{"/proc/self/maps"};
        std::string line;
        while (std::getline(f, line))
        {
            std::cout << line << std::endl;
        }
    }

    elf::sym find_function(const std::string& name)
    {
        for (auto &sec : _elf.sections()) {
            if (sec.get_hdr().type != elf::sht::symtab && sec.get_hdr().type != elf::sht::dynsym)
                    continue;

            for (auto sym : sec.as_symtab())
            {
                //std::cout << "sym: '" << sym.get_name() << "'" << std::endl;

                if (sym.get_name().empty())
                    continue;

                // for some reason I get " symbol" here
                auto real_name = sym.get_name().substr(1);

                if (real_name == name)
                {
                    auto &d = sym.get_data();
                    //std::cout << 
                    //        d.value << d.size
                    //        << to_string(d.type()).c_str()
                    //        << to_string(d.binding()).c_str()
                    //        << to_string(d.shnxd).c_str()
                    //        << sym.get_name().c_str() << std::endl;
                    return sym;
                }
            }
        }

        throw std::runtime_error("no symbol found");
    }

    elf::elf _elf{detail::create_elf_reader()};
};

}