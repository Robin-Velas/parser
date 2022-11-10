#ifndef OP_PARSER_H_
#define OP_PARSER_H_

#include <iostream>
#include <vector>

#include "Option.h"

namespace op {
    class Parser {
    public:

        /**
         * Parse command line made of a command options
         */
        void parseCommandLine(int argc, const char* const argv[]);

        /**
         * Print help : options, value and mandatory or not
         */
        void printHelp(std::ostream& stream = std::cout) const;

        /**
         * Tell number of positional arguments parsed
         */
        std::size_t getPositionalArgumentCount() const;

        /**
         * Access the option
         */
        Option& operator()(const std::string& name);

        /**
         * Access to positional arguments
         */
        const std::string& operator[](std::size_t i) const;

        /**
         * Tell if the given if an option or an alias of an option
         */
        bool isOption(const std::string& name);

        /**
         * Return option with the name given (can be the alias)
         */
        Option& getOption(const std::string& name);

    private:
        std::vector<op::Option> options;
        std::vector<std::string> positionalArguments;
    };
}

#endif // OP_PARSER_H_
