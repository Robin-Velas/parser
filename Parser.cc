#include "Parser.h"

#include <cstring>
#include <algorithm>
#include <stdexcept>


namespace op {

    void Parser::parseCommandLine(int argc, const char* const argv[]) {
        if(argc > 0)
            if(strlen(argv[0]) == 0)
                throw std::runtime_error("empty argv");

        for(int i=1; i<argc; ++i){
            // research option help
            if( (strcmp(argv[i],"--help") == 0) || (strcmp(argv[i],"-h") == 0) ){
                printHelp(std::cout);
                exit(1);
            }

            size_t size = strlen(argv[i]);
            if(size == 0) {
                throw std::runtime_error("empty argv");

            } else if(argv[i][0] == '-') {
                //option
                if(size == 1){
                    throw std::runtime_error("empty option");
                } else {
                    int start = 1;
                    if(argv[i][1] == '-') {
                        start = 2;
                    }

                    std::string argv_str = argv[i];
                    std::string option_name = argv_str.substr(start, size-start);

                    /* Search option, check if value expected */
                    if(!isOption(option_name)){
                        throw std::runtime_error("option not valid");
                    } else {
                        Option &option = Parser::getOption(option_name);
                        option.parsed();
                        if(option.expectValue()){

                            if( (i+1 >= argc) || (strlen(argv[i+1]) == 0) ){
                                throw std::runtime_error("value of option not found");
                            }

                            std::string value = argv[i+1];
                            std::cout << "value of option : " << value << std::endl;
                            option = value;
                            ++i;
                        }
                    }
                }

            } else {
                //not an option and not his value, so positional argument
                Parser::positionalArguments.emplace_back(argv[i]);
            }
        }

        // Check if mandatory option not read
        for(const Option& opt : Parser::options){
            if( (!opt) && (opt.isMandatory()) ){
                throw std::runtime_error("mandatory option not read");
            }
        }
    }

    void Parser::printHelp(std::ostream& stream) const {
        std::string result;
        for (const auto& current_option : options) {
            auto vec = current_option.getNames();
            bool mandatory = current_option.isMandatory();
            bool expect = current_option.expectValue();
            std::string defaultval;

            if (current_option.hasDefault()) {
                defaultval = current_option.getValue();
            }

            std::string ret;
            auto size = vec.size();
            for (size_t j = 0; j < size; ++j) {

                ret = "";

                if (j == size-1 && j == 0) { // case : without any alias
                    std::string complement;
                    if (expect)
                        complement.append(" value");
                    if (mandatory)
                        complement.append(" MANDATORY");
                    if (!defaultval.empty())
                        complement.append (" (default value : <" + defaultval + ">)");

                    ret.append("\t --" + vec[j] + complement +" \n");
                } else { // case : with alias
                    if (j == 0) { // case : first name
                        ret.append("\t --" + vec[j] + " |");
                    } else {
                        if (j == size-1) { //case : last alias
                            std::string complement;
                            if (expect)
                                complement.append(" value");
                            if (mandatory)
                                complement.append(" MANDATORY");
                            if (!defaultval.empty())
                                complement.append(" (default value : <" + defaultval + ">)");

                            ret.append(" -" + vec[j] + complement + " \n");
                        } else { // case : middle alias
                            ret.append(" -" + vec[j] + " |");
                        }
                    }
                }
            }
            result.append(ret);
        }
        stream << "Usage :\n" << result;
    }

    std::size_t Parser::getPositionalArgumentCount() const {
        return positionalArguments.size();
    }

    Option& Parser::operator()(const std::string& name) {
        if (name == "help" || name == "h") {
            throw std::runtime_error("help and h cannot be redefined");
        }
        for(auto& option : Parser::options){
            std::vector<std::string> names = option.getNames();
            if (std::find(names.begin(), names.end(), name) != names.end()){
                return option;
            }
        }

        // Option not stored -> add
        op::Option opt(name);
        Parser::options.push_back(opt);
        return Parser::options[options.size()-1];
    }

    const std::string& Parser::operator[](std::size_t i) const {
        auto count = Parser::getPositionalArgumentCount();
        if(i >= count){
            throw std::out_of_range("operator[]");
        }
        return positionalArguments[i];
    }

    bool Parser::isOption(const std::string& name){
        for(auto& option : Parser::options){
            std::vector<std::string> names = option.getNames();
            if (std::find(names.begin(), names.end(), name) != names.end()){
                return true;
            }
        }
        return false;
    }

    Option& Parser::getOption(const std::string& name){
        for(auto& option : Parser::options){
            std::vector<std::string> names = option.getNames();
            if (std::find(names.begin(), names.end(), name) != names.end()){
                return option;
            }
        }
        throw std::runtime_error("no option with this name");
    }

}
