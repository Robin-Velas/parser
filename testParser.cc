#include <gtest/gtest.h>

#include "Option.h"
#include "Parser.h"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/**
 * Option
 */

TEST(option_constructor, init) {
    op::Option opt("name");
    EXPECT_FALSE(opt);
    EXPECT_FALSE(opt.expectValue());
    EXPECT_FALSE(opt.isMandatory());
}

TEST(option_constructor, badFormat) {
    ASSERT_THROW(op::Option opt("8"), std::runtime_error);
    ASSERT_THROW(op::Option opt("8fe"), std::runtime_error);
    ASSERT_THROW(op::Option opt("fk$"), std::runtime_error);
    op::Option opt("Bilout-e_");
}
TEST(option_getNames, oneName) {
    op::Option opt("hell");
    std::vector<std::string> compare;
    compare.push_back("hell");
    EXPECT_EQ(compare, opt.getNames());
}
TEST(option_getNames, oneNameMultiplesAlias) {
    op::Option opt("hell");
    opt.addAlias("hel");
    opt.addAlias("he");
    opt.addAlias("hl");
    std::vector<std::string> compare;
    compare.push_back("hell");
    compare.push_back("hel");
    compare.push_back("he");
    compare.push_back("hl");
    EXPECT_EQ(compare, opt.getNames());
}

TEST(option_parsed, read) {
    op::Option opt("option");
    EXPECT_FALSE(opt);
    opt.parsed();
    EXPECT_TRUE(opt);
}

TEST(option_getValue, noValue) {
    op::Option opt("option");
    opt.hasValue();
    ASSERT_THROW(opt.getValue(), std::runtime_error);
}
TEST(option_getValue, noValueExpected) {
    op::Option opt("option");
    ASSERT_THROW(opt.getValue(), std::runtime_error);
}
TEST(option_getValue, value) {
    op::Option opt("option");
    opt.hasValue();
    opt = "the_value";
    std::string value = "the_value";
    EXPECT_EQ(value, opt.getValue());
}


TEST(option_setDefaultValue, value) {
    op::Option opt("option");
    opt.setDefaultValue("the_value");
    std::string value = "the_value";
    EXPECT_EQ(value, opt.getValue());
}

TEST(option_mandatory, no) {
    op::Option opt("option");
    EXPECT_FALSE(opt.isMandatory());
}
TEST(option_mandatory, yes) {
    op::Option opt("option");
    opt.setMandatory();
    EXPECT_TRUE(opt.isMandatory());
}

TEST(option_equality, equal) {
    op::Option opt("option");
    std::string name = "option";
    EXPECT_TRUE(opt == name);
}
TEST(option_equality, equalAlias) {
    op::Option opt("option");
    opt.addAlias("op");
    std::string name = "op";
    EXPECT_TRUE(opt == name);
}
TEST(option_equality, notEqual) {
    op::Option opt("option");
    opt.addAlias("op");
    std::string name = "pop";
    EXPECT_FALSE(opt == name);
}

TEST(option_inequality, unequal) {
    op::Option opt("option");
    std::string name = "opti";
    EXPECT_TRUE(opt != name);
}
TEST(option_inequality, unequalAlias) {
    op::Option opt("option");
    opt.addAlias("op");
    opt.addAlias("pop");
    std::string name = "opti";
    EXPECT_TRUE(opt != name);
}
TEST(option_inequality, equal) {
    op::Option opt("option");
    std::string name = "option";
    EXPECT_FALSE(opt != name);
}

TEST(option_addValue, valueExpected) {
    op::Option opt("option");
    opt.hasValue();
    opt = "value";
    EXPECT_EQ("value", opt.getValue());
}
TEST(option_addValue, noValueExpected) {
    op::Option opt("option");
    ASSERT_THROW(opt = "value", std::runtime_error);
}

TEST(option_addAlias, alreadyAdded) {
    op::Option opt("option");
    opt.addAlias("op");
    ASSERT_THROW(opt.addAlias("op"), std::runtime_error);
}
/**
 * Parser
 */

TEST(parser_fonctor, first) {
    op::Parser parser;
    EXPECT_NO_FATAL_FAILURE(parser("hello"));
}

TEST(parser_fonctor, help) {
    op::Parser parser;
    ASSERT_THROW(parser("help"), std::runtime_error);
    ASSERT_THROW(parser("h"), std::runtime_error);
}

TEST(parser_fonctor, badFormat) {
    op::Parser parser;
    ASSERT_THROW(parser("9"), std::runtime_error);
    ASSERT_THROW(parser("--"), std::runtime_error);
    ASSERT_THROW(parser("-"), std::runtime_error);
    ASSERT_THROW(parser("fjeig*"), std::runtime_error);
    ASSERT_THROW(parser(" A"), std::runtime_error);
}

TEST(getPositionalArgumentCount, empty) {
    const char *args[2] = {"command", "--option"};
    op::Parser parser;
    parser("option");
    parser.parseCommandLine(std::size(args), args);
    EXPECT_EQ(0u, parser.getPositionalArgumentCount());
}

TEST(getPositionalArgumentCount, two){
    const char *args[4] = {"command", "--option", "pos1", "pos2"};
    op::Parser parser;
    parser("option");
    parser.parseCommandLine(std::size(args), args);
    EXPECT_EQ(2u, parser.getPositionalArgumentCount());
}

TEST(getPositionalArgumentCount, one){
    const char *args[4] = {"command", "--option", "pos1", "pos2"};
    op::Parser parser;
    parser("option");
    parser.parseCommandLine(std::size(args), args);
    EXPECT_EQ(2u, parser.getPositionalArgumentCount());
}

TEST(getPositionalArgumentCount, multiple_positions){
    const char *args[8] = {"command", "pos1", "pos2", "--option1", "val1", "--option2", "val2", "pos3"};
    op::Parser parser;
    parser("option1").hasValue();
    parser("option2").hasValue();
    parser.parseCommandLine(std::size(args), args);
    EXPECT_EQ(3u, parser.getPositionalArgumentCount());
}

TEST(accessor, multiple_positional_arguments){
    const char *args[8] = {"command", "pos1", "pos2", "--option1", "val1", "--option2", "val2", "pos3"};
    op::Parser parser;
    parser("option1").hasValue();
    parser("option2").hasValue();
    parser.parseCommandLine(std::size(args), args);
    EXPECT_EQ("pos1", parser[0]);
    EXPECT_EQ("pos2", parser[1]);
    EXPECT_EQ("pos3", parser[2]);
}
TEST(accessor, throw_out_of_bound) {
    const char *args[8] = {"command", "pos1", "pos2", "--option1", "val1", "--option2", "val2", "pos3"};
    op::Parser parser;
    parser("option1").hasValue();
    parser("option2").hasValue();
    parser.parseCommandLine(std::size(args), args);
    ASSERT_THROW(parser[5], std::out_of_range);
}


TEST(parser_options, addDefaultValue) {
    op::Parser parser;
    parser("option").setDefaultValue("foo");
}

TEST(parser_parseCommandLine, help_exit) {
    const char *args[4] = {"command", "--option", "--help", "pos2"};
    op::Parser parser;
    parser("option");
    EXPECT_EXIT(parser.parseCommandLine(std::size(args), args), testing::ExitedWithCode(1), "");
}

TEST(parser_parseCommandLine, modified_value) {
    const char* args[6] = {"command", "--option", "bar", "-a", "pos1", "pos2"};

    op::Parser parser;
    parser("option").setDefaultValue("foo");
    parser("alias").addAlias("a");
    parser.parseCommandLine(std::size(args), args);
    EXPECT_EQ("bar", parser("option").getValue());
}

TEST(parser_parseCommandLine, double_redifinition_by_alias) {
    const char* args[7] = {"command", "--option", "bar", "-o", "fools", "pos1", "pos2"};

    op::Parser parser;
    parser("option").setDefaultValue("foo").addAlias("o");
    parser("alias").addAlias("a");
    parser.parseCommandLine(std::size(args), args);
    EXPECT_EQ("fools", parser("option").getValue());
}

TEST(parser_parseCommandLine, value_with_dash) {
    const char* args[3] = {"command", "--option", "-hel"};

    op::Parser parser;
    parser("option").setDefaultValue("foo");
    parser.parseCommandLine(std::size(args), args);
    EXPECT_EQ("-hel", parser("option").getValue());
}
TEST(parser_parseCommandLine, mandatoryOptionNotRead) {
    const char *args[4] = {"command", "pos1", "pos2", "pos3"};

    op::Parser parser;
    parser("option").setMandatory();
    ASSERT_THROW(parser.parseCommandLine(std::size(args), args), std::runtime_error);
}
TEST(parser_parseCommandLine, not_mandatoryOptionNotRead) {
    const char *args[4] = {"command", "pos1", "pos2", "pos3"};

    op::Parser parser;
    parser("option");
    parser.parseCommandLine(std::size(args), args);
}
