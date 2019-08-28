#include "include/dwarf.hpp"
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/types.h>



extern "C" int breakpoint_enable(pid_t, long);


dwarf::dwarf 	__dwarf;
elf::elf	__elf;

int db_inform_init(char* progname)
{
	int fd = open(progname, O_RDONLY);

	__elf = elf::elf(elf::create_mmap_loader(fd));
	__dwarf = dwarf::dwarf(dwarf::elf::create_loader(__elf));
}


dwarf::die get_function_from_pc(long long pc)
{
	for (auto &cu : __dwarf.compilation_units())
	{
		if (die_pc_range(cu.root()).contains(pc))
		{
			for(const auto& die : cu.root())
			{
				if( die.tag == dwarf::DW_TAG::subprogram)
				{
					if(die_pc_range(die).contains(pc))
					{
						return die;
					}
				}
			}
		}
	}
	fprintf(stderr, "CAN'T FIND FUNCTION");
}


dwarf::line_table::iterator get_line_entry_from_pc(long long pc) {
    for (auto &cu : __dwarf.compilation_units()) {
        if (die_pc_range(cu.root()).contains(pc)) {
            auto &lt = cu.get_line_table();
            auto it = lt.find_address(pc);
            if (it == lt.end()) {
                throw std::out_of_range{"Cannot find line entry"};
            }
            else {
                return it;
            }
        }
    }

    throw std::out_of_range{"Cannot find line entry"};
}


void set_breakpoint_at_function(int pid, const std::string& name) {
    for (const auto& cu : __dwarf.compilation_units()) {
        for (const auto& die : cu.root()) {
            if (die.has(dwarf::DW_AT::name) && at_name(die) == name) {
                auto low_pc = at_low_pc(die);
                auto entry = get_line_entry_from_pc(low_pc);
                ++entry; //skip prologue
                breakpoint_enable(pid, entry->address);
            }
        }
    }
}

void print_source(const std::string& file_name, unsigned line, unsigned n_lines_context) {
    std::ifstream file {file_name};

    //Work out a window around the desired line
    auto start_line = line <= n_lines_context ? 1 : line - n_lines_context;
    auto end_line = line + n_lines_context + (line < n_lines_context ? n_lines_context - line : 0) + 1;

    char c{};
    auto current_line = 1u;
    //Skip lines up until start_line
    while (current_line != start_line && file.get(c)) {
        if (c == '\n') {
            ++current_line;
        }
    }

    //Output cursor if we're at the current line
    std::cout << (current_line==line ? "> " : "  ");

    //Write lines up until end_line
    while (current_line <= end_line && file.get(c)) {
        std::cout << c;
        if (c == '\n') {
            ++current_line;
            //Output cursor if we're at the current line
            std::cout << (current_line==line ? "> " : "  ");
        }
    }

    //Write newline and make sure that the stream is flushed properly
    std::cout << std::endl;
}
