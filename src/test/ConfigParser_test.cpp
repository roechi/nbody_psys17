//
// Created by Richard Remus on 10.05.17.
//

#include "ConfigParser_test.h"
#include "gtest/gtest.h"
#include "../main/ConfigParser.h"
#include "../main/Body.h"



TEST(ConfigParserTests, testOneLine) {
    ConfigParser *parser = new ConfigParser();
    
    const Body &body = parser->parseLine("1000.0 1.0 2.0 3.0 4.0\n");

    ASSERT_EQ(body.m, 1000.0f);
    ASSERT_EQ(body.rx, 1.0f);
    ASSERT_EQ(body.ry, 2.0f);
    ASSERT_EQ(body.vx, 3.0f);
    ASSERT_EQ(body.vy, 4.0f);
}

TEST(ConfigParserTests, testParseFile) {
    ConfigParser *parser = new ConfigParser();

    try {
        std::list<Body> bodies = parser->parseFile("../../../resources/test/example.txt");
        ASSERT_EQ(bodies.size(), 5);
    } catch (std::string exception) {
        std::cout << exception << "\n";
        FAIL();
    }
}


TEST(ConfigParserTest, testParseFileWithException) {
    ConfigParser *parser = new ConfigParser();

    try {
        parser->parseFile("path/to/nonexisting/file");
        FAIL();
    } catch (std::string exception) {
        std::cout << exception << "\n";
    }
}
