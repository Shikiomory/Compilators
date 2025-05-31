#include "Scanner.h"
#include <string>
#include "../TextDriver/Driver.h"
#include "../error/Error.h"
#include <unordered_map>
#include <string>
#include <vector>
Lex Scanner::lex = Lex::NONE;
std::string Scanner::nameValue = "";
double Scanner::numValue = 0.0;
unsigned int Scanner::lexPosition = 0;

Scanner::Scanner(Driver& driver, std::shared_ptr<Error> errorPtr, Freq* freqPtr) :
    driver(driver), errorPtr(std::move(errorPtr)), freqPtr(freqPtr) {
    Scanner::lex = Lex::NONE;
    lexPosition = 0;
    numValue = 0;
    driver.nextCh();
}

void Scanner::nextLex()
{
    while (Driver::ch == Driver::chSpace ||
        Driver::ch == Driver::chTab ||
        Driver::ch == Driver::chEOL)
    {
        driver.nextCh();
    }

    Scanner::lexPosition = Driver::position;

    if ((Driver::ch >= 'A' && Driver::ch <= 'Z') ||
        (Driver::ch >= 'a' && Driver::ch <= 'z') ||
        Driver::ch == '_')
    {
        scanName(false);
    }
    else if (Driver::ch >= '0' && Driver::ch <= '9')
    {
        scanNumber();
    }
    else if (Driver::ch == '"') {
        driver.nextCh();
        scanStrLit();
    }
    else if (Driver::ch == '\'') {
        driver.nextCh();
        scanCharLiteral();
    }
    else if (Driver::ch == ';')
    {
        lex = Lex::SEMICOLON;
        driver.nextCh();
    }
    else if (Driver::ch == ':')
    {
        driver.nextCh();
        lex = Lex::COLON;
    }
    else if (Driver::ch == '.')
    {
        lex = Lex::DOT;
        driver.nextCh();
    }
    else if (Driver::ch == ',')
    {
        lex = Lex::COMMA;
        driver.nextCh();
    }
    else if (Driver::ch == '+')
    {
        driver.nextCh();
        if (Driver::ch == '+') {
            lex = Lex::INCREMENT;
            driver.nextCh();
        }
        else if (Driver::ch == '=') {
            lex = Lex::PLUS_ASSIGN;
            driver.nextCh();
        }
        else {
        lex = Lex::PLUS;
        }
    }
    else if (Driver::ch == '-')
    {
        driver.nextCh();
        if (Driver::ch == '-') {
            lex = Lex::DECREMENT;
            driver.nextCh();
        }
        else if (Driver::ch == '=') {
            lex = Lex::MINUS_ASSIGN;
            driver.nextCh();
        }
        else {
            lex = Lex::MINUS;
        }
    }
    else if (Driver::ch == '*') {
        driver.nextCh();
        if (Driver::ch == '=') { 
            lex = Lex::MULT_ASSIGN;
            driver.nextCh();
        }
        else {
            lex = Lex::MULT;
        }
    }
    else if (Driver::ch == '/') {
        driver.nextCh();
        if (Driver::ch == '/') {
            skipSingleComment();
        }
        else if (Driver::ch == '*') {
            skipComment();
        }
        else if (Driver::ch == '=') {
            lex = Lex::DIV_ASSIGN;
            driver.nextCh();
        }
        else {
            lex = Lex::DIV;
        }
    }
    else if (Driver::ch == '%') {
        driver.nextCh();
        if (Driver::ch == '=') {
            lex = Lex::MOD_ASSIGN;
            driver.nextCh();
        }
        else {
            lex = Lex::MOD;
        }
    }
    else if (Driver::ch == '(')
    {
        lex = Lex::LPAREN;
        driver.nextCh();
    }
    else if (Driver::ch == ')')
    {
        lex = Lex::RPAREN;
        driver.nextCh();
    }
    else if (Driver::ch == '{') {
        lex = Lex::LBRACE;
        driver.nextCh();
    }
    else if (Driver::ch == '}') {
        lex = Lex::RBRACE;
        driver.nextCh();
    }
    else if (Driver::ch == '[') {
        lex = Lex::LBRACKET;
        driver.nextCh();
    }
    else if (Driver::ch == ']') {
        lex = Lex::RBRACKET;
        driver.nextCh();
    }
    else if (Driver::ch == '?') {
        driver.nextCh();
        if (Driver::ch == '?') {
            driver.nextCh();
            if (Driver::ch == '=') {
                lex = Lex::NULL_COALESCING_ASSIGN;
                driver.nextCh();
            }
            else {
                lex = Lex::NULL_COALESCING;
            }
        }
        else if (Driver::ch == '.') {
            lex = Lex::NULL_CONDITIONAL;
            driver.nextCh();
        }
        else {
            lex = Lex::QUESTION;
        }
        }

    else if (Driver::ch == '$' || Driver::ch == '@') {
        bool isInterpolated = false;
        bool isVerbatim = false;
        bool isString = false;

        if (Driver::ch == '$') {
            isInterpolated = true;
            driver.nextCh();
            if (Driver::ch == '@') {
                isVerbatim = true;
                driver.nextCh();
            }

            if (Driver::ch == '"') {
                isString = true;
            }
        }
        else if (Driver::ch == '@') {
            isVerbatim = true;
            driver.nextCh();

            if (Driver::ch == '$') {
                isInterpolated = true;
                driver.nextCh();
            }

            if (Driver::ch == '"') {
                isString = true;
            }
        }

        if (isString) {
            driver.nextCh();
            if (isInterpolated) {
                scanIntrStrLit();
            }
            else {
                lex = Lex::VERBATIM_STRING;
                while (Driver::ch != '"' && Driver::ch != Driver::chEOT) {
                    if (Driver::ch == '"') {
                        driver.nextCh();
                        if (Driver::ch == '"') {
                            driver.nextCh();
                        }
                        else {
                            break;
                        }
                    }
                    else {
                        driver.nextCh();
                    }
                }
                if (Driver::ch == '"') driver.nextCh();
            }
        }
        else if ((Driver::ch >= 'A' && Driver::ch <= 'Z') ||
            (Driver::ch >= 'a' && Driver::ch <= 'z') ||
            Driver::ch == '_') {
            scanName(true);
        }
        else {
            errorPtr->lexError("Ожидается строка или идентификатор после '@' / '$'");
        }
        }

    else if (Driver::ch == '=')
    {
        driver.nextCh();
        if (Driver::ch == '=') {
            lex = Lex::EQ;
            driver.nextCh();
        }
        else if (Driver::ch == '>') {
            lex = Lex::ARROW;
            driver.nextCh();
        }
        else {
            lex = Lex::ASSIGN;
        }
    }
    else if (Driver::ch == '!')
    {
        driver.nextCh();
        if (Driver::ch == '=') {
            lex = Lex::NE;
            driver.nextCh();
        }
        else {
            lex = Lex::NOT;
        }
    }
    else if (Driver::ch == '<') {
        driver.nextCh();
        if (Driver::ch == '<') {
            driver.nextCh();
            if (Driver::ch == '=') {
                lex = Lex::LSHIFT_ASSIGN;
                driver.nextCh();
            }
            else {
                lex = Lex::LSHIFT;
            }
        }
        else if (Driver::ch == '=') {
            lex = Lex::LE;
            driver.nextCh();
        }
        else {
            lex = Lex::LT;
        }
        }
    else if (Driver::ch == '>') {
            driver.nextCh();
            if (Driver::ch == '>') {
                driver.nextCh();
                if (Driver::ch == '=') {
                    lex = Lex::RSHIFT_ASSIGN;
                    driver.nextCh();
                }
                else {
                    lex = Lex::RSHIFT;
                }
            }
            else if (Driver::ch == '=') {
                lex = Lex::GE;
                driver.nextCh();
            }
            else {
                lex = Lex::GT;
            }
            }
    else if (Driver::ch == '&') {
        driver.nextCh();
        if (Driver::ch == '&') {
            lex = Lex::AND;
            driver.nextCh();
        }
        else if (Driver::ch == '=') {
            lex = Lex::AND_ASSIGN;
            driver.nextCh();
        }
        else {
            lex = Lex::BITWISE_AND;
        }
}
    else if (Driver::ch == '|') {
        driver.nextCh();
        if (Driver::ch == '|') {
            lex = Lex::OR;
            driver.nextCh();
        }
        else if (Driver::ch == '=') {
            lex = Lex::OR_ASSIGN;
            driver.nextCh();
        }
        else {
            lex = Lex::BITWISE_OR;
        }
}
    else if (Driver::ch == '#') {
        driver.nextCh();
        scanPreprocessorDirective();
    }
    else if (Driver::ch == '~') {
        lex = Lex::BITWISE_NOT;
        driver.nextCh();
}
    else if (Driver::ch == '^') {
        driver.nextCh();
        if (Driver::ch == '=') {
            lex = Lex::XOR_ASSIGN;
            driver.nextCh();
        }
        else {
            lex = Lex::BITWISE_XOR;
        }
}

    else if (Driver::ch == Driver::chEOT)
    {
        lex = Lex::EOT;
    }
    
    else
    {
        errorPtr->lexError("Недопустимый символ");
    }
}

void Scanner::scanName(bool isVerbatim)
{
    nameValue = isVerbatim ? "@" : "";

    while ((Driver::ch >= 'A' && Driver::ch <= 'Z') ||
        (Driver::ch >= 'a' && Driver::ch <= 'z') ||
        (Driver::ch >= '0' && Driver::ch <= '9') ||
        Driver::ch == '_') {
        nameValue.push_back(Driver::ch);
        driver.nextCh();
    }

    if (isVerbatim) {
        lex = Lex::NAME;
        return;
    }

    auto it = lexTable.find(nameValue);
    lex = (it != lexTable.end()) ? it->second : Lex::NAME;
}

void Scanner::scanStrLit() {
    nameValue = "";

    while (Driver::ch != '"' && Driver::ch != Driver::chEOT) {
        if (Driver::ch == '\\') {
            int escChar = scanEscapeSequence();
            nameValue += static_cast<char>(escChar);
        }
        else {
            nameValue += Driver::ch;
            driver.nextCh();
        }
    }

    if (Driver::ch == '"') {
        driver.nextCh(); 
        lex = Lex::STRINGLITERAL;
    }
    else {
        errorPtr->lexError("Ожидается закрывающая кавычка");
    }
}


void Scanner::scanCharLiteral() {
    int value = 0;

    if (Driver::ch == '\\') {
        value = scanEscapeSequence();
    }
    else {
        if (Driver::ch == '\'' || Driver::ch == '\n' || Driver::ch == '\r' || Driver::ch == Driver::chEOT) {
            errorPtr->lexError("Недопустимый символ в символьном литерале");
            return;
        }
        value = Driver::ch;
        driver.nextCh(); 
    }

    if (Driver::ch == '\'') {
        driver.nextCh();
        lex = Lex::CHARLITERAL;
        numValue = value;
    }
    else {
        errorPtr->lexError("Ожидается закрывающая кавычка");
    }
}

void Scanner::scanIntrStrLit() {
    while (Driver::ch != '"' && Driver::ch != Driver::chEOT) {
        if (Driver::ch == '{') {
            driver.nextCh(); 

            if (Driver::ch == '{') {
                driver.nextCh();
            }
            else {
                lex = Lex::LBRACE;
                if (freqPtr) {
                    freqPtr->put(lex);
                }

                int braceCount = 1;
                while (braceCount > 0 && Driver::ch != Driver::chEOT) {
                    nextLex();

                    if (lex != Lex::NONE && freqPtr) {
                        freqPtr->put(lex);
                    }

                    if (lex == Lex::LBRACE) braceCount++;
                    else if (lex == Lex::RBRACE) braceCount--;
                }

                continue;
            }
        }

        else if (Driver::ch == '}') {
            driver.nextCh();
            if (Driver::ch == '}') {
                driver.nextCh();
            }
            else {
                errorPtr->lexError("Неэкранированная '}'");
            }
        }
        else {
            driver.nextCh();
        }
    }

    if (Driver::ch == '"') {
        driver.nextCh();
    }
    else {
        errorPtr->lexError("Ожидается закрывающая кавычка");
    }

    lex = Lex::INTERPOLATED_STRING;
}

void Scanner::scanNumber() {
    std::string numStr;
    bool isFloat = false;
    bool hasExponent = false;

    if (Driver::ch == '0') {
        numStr += '0';
        driver.nextCh();

        if (Driver::ch == 'b' || Driver::ch == 'B') {
            driver.nextCh();
            std::string binStr;
            while (Driver::ch == '0' || Driver::ch == '1' || Driver::ch == '_') {
                if (Driver::ch != '_') binStr += Driver::ch;
                driver.nextCh();
            }

            try {
                numValue = std::stoi(binStr, nullptr, 2);
            }
            catch (...) {
                errorPtr->lexError("Недопустимый бинарный литерал: " + binStr);
                numValue = 0;
            }

            lex = Lex::NUMBER;
            return;
        }

        if (Driver::ch == 'x' || Driver::ch == 'X') {
            driver.nextCh();
            std::string hexStr;
            while ((Driver::ch >= '0' && Driver::ch <= '9') ||
                (Driver::ch >= 'a' && Driver::ch <= 'f') ||
                (Driver::ch >= 'A' && Driver::ch <= 'F') ||
                Driver::ch == '_') {
                if (Driver::ch != '_') hexStr += Driver::ch;
                driver.nextCh();
            }

            try {
                numValue = std::stoi(hexStr, nullptr, 16);
            }
            catch (...) {
                errorPtr->lexError("Недопустимый шестнадцатеричный литерал: " + hexStr);
                numValue = 0;
            }

            lex = Lex::NUMBER;
            return;
        }

    }

    while ((Driver::ch >= '0' && Driver::ch <= '9') || Driver::ch == '_') {
        if (Driver::ch != '_') numStr += Driver::ch;
        driver.nextCh();
    }

    if (Driver::ch == '.') {
        isFloat = true;
        numStr += '.';
        driver.nextCh();

        while ((Driver::ch >= '0' && Driver::ch <= '9') || Driver::ch == '_') {
            if (Driver::ch != '_') numStr += Driver::ch;
            driver.nextCh();
        }
    }

    if (Driver::ch == 'e' || Driver::ch == 'E') {
        isFloat = true;
        hasExponent = true;
        numStr += Driver::ch;
        driver.nextCh();

        if (Driver::ch == '+' || Driver::ch == '-') {
            numStr += Driver::ch;
            driver.nextCh();
        }

        while ((Driver::ch >= '0' && Driver::ch <= '9') || Driver::ch == '_') {
            if (Driver::ch != '_') numStr += Driver::ch;
            driver.nextCh();
        }
    }

    if (Driver::ch == 'm' || Driver::ch == 'M' ||
        Driver::ch == 'f' || Driver::ch == 'F' ||
        Driver::ch == 'd' || Driver::ch == 'D') {
        driver.nextCh(); 
    }

    try {
        numValue = std::stod(numStr);
    }
    catch (...) {
        errorPtr->lexError("Недопустимый числовой литерал: " + numStr);
        numValue = 0;
    }

    lex = Lex::NUMBER;
}

void Scanner::scanPreprocessorDirective() {

    while (Driver::ch == ' ' || Driver::ch == '\t') {
        driver.nextCh();
    }

    nameValue = "";
    while ((Driver::ch >= 'a' && Driver::ch <= 'z') ||
        (Driver::ch >= 'A' && Driver::ch <= 'Z') ||
        Driver::ch == '_') {
        nameValue += Driver::ch;
        driver.nextCh();
    }

    while (Driver::ch != '\n' && Driver::ch != '\r' && Driver::ch != Driver::chEOT) {
        driver.nextCh();
    }

    lex = Lex::PREPROC_DIRECTIVE;
}

void Scanner::skipSingleComment() {

    while (Driver::ch != Driver::chEOT && Driver::ch != '\n' && Driver::ch != '\r') {
        driver.nextCh();
    }

    if (Driver::ch == '\r') {
        driver.nextCh();
        if (Driver::ch == '\n') {
            driver.nextCh(); 
        }
    }
    else if (Driver::ch == '\n') {
        driver.nextCh(); 
    }

    nextLex(); 
}

void Scanner::skipComment()
{
    driver.nextCh(); 

    while (true)
    {
        while (Driver::ch != '*' && Driver::ch != Driver::chEOT)
        {
            driver.nextCh();

            if (Driver::ch == '/')
            {
                driver.nextCh();
                if (Driver::ch == '*')
                {
                    skipComment();
                }
            }
        }

        if (Driver::ch == Driver::chEOT)
        {
            errorPtr->lexError("Нет конца комментария");
        }
        else
        {
            driver.nextCh();
        }

        if (Driver::ch == '/')
        {
            break;
        }
    }
    driver.nextCh();
    nextLex();
}

std::string Scanner::getStringNameOfLex(Lex lex)
{
    auto it = lexToStr.find(lex);
    std::string name = (it != lexToStr.end()) ? it->second : "лексема не найдена";

    return name;
}

int Scanner::scanEscapeSequence() {
    driver.nextCh();

    switch (Driver::ch) {
    case '\'': driver.nextCh(); return '\'';
    case '"':  driver.nextCh(); return '"';
    case '\\': driver.nextCh(); return '\\';
    case '0':  driver.nextCh(); return '\0';
    case 'a':  driver.nextCh(); return '\a';
    case 'b':  driver.nextCh(); return '\b';
    case 'f':  driver.nextCh(); return '\f';
    case 'n':  driver.nextCh(); return '\n';
    case 'r':  driver.nextCh(); return '\r';
    case 't':  driver.nextCh(); return '\t';
    case 'v':  driver.nextCh(); return '\v';

    case 'u':
        driver.nextCh();
        return scanUnicodeEscape();

    case 'x':
        driver.nextCh();
        return scanHexEscape();

    default: {
        errorPtr->lexError("Недопустимая escape-последовательность: \\" + std::string(1, Driver::ch));
        int invalid = Driver::ch;
        driver.nextCh();
        return invalid;
    }
    }
}


int Scanner::scanUnicodeEscape() {
    int value = 0;
    for (int i = 0; i < 4; i++) {
        if (Driver::ch >= '0' && Driver::ch <= '9') {
            value = value * 16 + (Driver::ch - '0');
        }
        else if (Driver::ch >= 'a' && Driver::ch <= 'f') {
            value = value * 16 + (Driver::ch - 'a' + 10);
        }
        else if (Driver::ch >= 'A' && Driver::ch <= 'F') {
            value = value * 16 + (Driver::ch - 'A' + 10);
        }
        else {
            errorPtr->lexError("Ожидается шестнадцатеричная цифра");
            break;
        }
        driver.nextCh();
    }
    return value;
}

int Scanner::scanHexEscape() {
    int value = 0;
    int count = 0;
    while (count < 4) {
        if (Driver::ch >= '0' && Driver::ch <= '9') {
            value = value * 16 + (Driver::ch - '0');
        }
        else if (Driver::ch >= 'a' && Driver::ch <= 'f') {
            value = value * 16 + (Driver::ch - 'a' + 10);
        }
        else if (Driver::ch >= 'A' && Driver::ch <= 'F') {
            value = value * 16 + (Driver::ch - 'A' + 10);
        }
        else {
            break;
        }
        count++;
        driver.nextCh(); 
    }
    return value;
}
