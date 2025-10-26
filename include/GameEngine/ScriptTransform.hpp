#pragma once

#include <fstream>
#include <iostream>
#include <cctype>
#include <string>
#include <vector>

namespace sas
{

    enum class TokenType
    {
        IDENTIFIER,
        NUMBER,
        OPERATOR,
        OPEN_PAREN,
        CLOSE_PAREN,
        END_OF_FILE,
        EQUAL,
        OPEN_CURLY,
        CLOSE_CURLY,
        FUNCTION_IDENTIFIER,
        NEW_LINE,
        DELTA_TIME,
        GREATER_THAN,
        GREATER_EQUAL_THEN,
        PLUS,
        PLUS_EUQALS,

        UNKNOWN
    };

    struct Token
    {
        TokenType type;
        std::string value;
    };

    class Tokenizer
    {
    private:
        std::istream &file;
        std::string line;
        char currentChar = 0;
        size_t index = 0;

        void getNextLine() noexcept;

        void advance() noexcept;

        void skipWhitespace() noexcept;

        Token number() noexcept;
        
        Token identifier() noexcept;

    public:
        Tokenizer(std::istream &input) noexcept;

        Token getNextToken() noexcept;
    };

    std::vector<Token> getAllTokens(const std::string& path) noexcept;
    void generateCPPClass(std::vector<Token> tokens, const std::string &fn, std::string_view name) noexcept;
}