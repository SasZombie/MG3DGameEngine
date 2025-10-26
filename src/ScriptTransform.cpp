#include "ScriptTransform.hpp"

void sas::Tokenizer::getNextLine() noexcept
{
    std::getline(file, line);
}

void sas::Tokenizer::advance() noexcept
{
    if (index == line.size())
    {
        currentChar = '\n';
        index = 0;
        getNextLine();
        return;
    }

    currentChar = line[index];
    ++index;
}

void sas::Tokenizer::skipWhitespace() noexcept
{
    while (std::isspace(currentChar) && !file.eof())
        advance();
}

sas::Token sas::Tokenizer::number() noexcept
{
    std::string value;
    while (std::isdigit(currentChar) && !file.eof())
    {
        value += currentChar;
        advance();
    }
    return {TokenType::NUMBER, value};
}

sas::Token sas::Tokenizer::identifier() noexcept
{
    std::string value;
    while ((std::isalnum(currentChar) || currentChar == '_' || currentChar == '.') && !file.eof())
    {
        value += currentChar;
        advance();
    }
    if (value == "void")
    {
        return {TokenType::FUNCTION_IDENTIFIER, value};
    }

    if (value == "dt")
    {
        return {TokenType::DELTA_TIME, value};
    }
    return {TokenType::IDENTIFIER, value};
}

sas::Tokenizer::Tokenizer(std::istream &input) noexcept
    : file(input)
{
    getNextLine();
    advance();
}

sas::Token sas::Tokenizer::getNextToken() noexcept
{
    if (file.eof())
        return {TokenType::END_OF_FILE, ""};
    if (currentChar == '\n')
    {
        advance();
        return {TokenType::NEW_LINE, "\n"};
    }

    skipWhitespace();

    if (std::isdigit(currentChar))
        return number();
    if (std::isalpha(currentChar))
        return identifier();
    if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/' || currentChar == '>' || currentChar == '<')
    {
        char op = currentChar;
        advance();
        return {TokenType::OPERATOR, std::string(1, op)};
    }
    if (currentChar == '(')
    {
        advance();
        return {TokenType::OPEN_PAREN, "("};
    }
    if (currentChar == ')')
    {
        advance();
        return {TokenType::CLOSE_PAREN, ")"};
    }

    if (currentChar == '{')
    {
        advance();
        return {TokenType::OPEN_CURLY, "{"};
    }
    if (currentChar == '}')
    {
        advance();
        return {TokenType::CLOSE_CURLY, "}"};
    }

    if (currentChar == '=')
    {
        advance();
        return {TokenType::EQUAL, "="};
    }

    char unknown = currentChar;
    advance();
    return {TokenType::UNKNOWN, std::string(1, unknown)};
}

std::vector<sas::Token> sas::getAllTokens(const std::string &path) noexcept
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open script file\n";
        return {};
    }

    Tokenizer tokenizer(file);
    Token token;
    std::vector<Token> tokens;
    do
    {
        token = tokenizer.getNextToken();
        tokens.push_back(token);

    } while (token.type != TokenType::END_OF_FILE);

    return tokens;
}

void sas::generateCPPClass(std::vector<sas::Token> tokens, const std::string &fn, std::string_view name) noexcept
{
    std::ofstream out(fn);

    out << "#include \"../include/GameEngine/Globals.hpp\"\n#include \"../include/GameEngine/Asset.hpp\"\nextern\"C\"\n{\nclass " << name << "\n{\nprivate:\n\t";

    size_t index = 0;
    while (tokens[index].type != TokenType::FUNCTION_IDENTIFIER)
    {
        if (tokens[index].type == TokenType::NEW_LINE)
        {
            out << ';' << tokens[index].value;
        }
        else
        {
            out << ' ' << tokens[index].value;
        }
        ++index;
    }

    bool addParens = false;
    bool addSemiColon = false;
    bool ifCondition = false;

    out << "public:\n";

    while (tokens[index].type != TokenType::END_OF_FILE)
    {
        const auto &current = tokens[index];

        if (current.type == TokenType::FUNCTION_IDENTIFIER)
        {
            addParens = true;
            out << current.value << ' ';
            ++index;
        }
        else if (addParens)
        {
            out << current.value << "(sas::Asset& obj)";
            ++index;
            addParens = false;
        }
        else if (current.type == TokenType::IDENTIFIER && current.value == "if")
        {
            out << "if (";
            ifCondition = true;
            ++index;
        }
        else if (current.type == TokenType::NEW_LINE)
        {
            if (ifCondition)
            {
                ifCondition = false;
                out << ")\n";
            }
            else if (addSemiColon)
            {
                out << ";\n";
                addSemiColon = false;
            }

            ++index;
        }
        else if (current.type == TokenType::OPERATOR && tokens[index + 1].type == TokenType::EQUAL)
        {
            out << current.value << tokens[index + 1].value << ' ';
            index = index + 2;
        }
        else if (current.type == TokenType::DELTA_TIME)
        {
            out << "sas::Globals::instance().getDeltaTime() ";
            ++index;
        }
        else
        {
            out << current.value << ' ';
            addSemiColon = true;
            ++index;
        }
    }


    out << "};\n" 
        << name << "* create() { return new " << name << "();}\n"
        << "void update(" << name << "*instance, sas::Asset& obj){ instance->update(obj);}\n"
        << "void destroy(" << name << "* instance) { delete instance; }\n}\n";

    out.close();
}
