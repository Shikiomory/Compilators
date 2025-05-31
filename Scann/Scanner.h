#include "../TextDriver/Driver.h"
#include "../frequent/Freq.h"
#include "../LexType.h"
#pragma once

#include <memory> 
#include <unordered_map>
#include <string>
#include <vector>

class Freq;

class Scanner
{
public:
    static Lex lex;
    static double numValue;
    static unsigned int lexPosition;
    static std::string nameValue;

    Scanner(Driver& driver, std::shared_ptr<Error> errorPtr, Freq* freqPtr);
    void nextLex();
    void scanName(bool isVerbatim);
    void scanStrLit();
    void scanCharLiteral();
    void scanIntrStrLit();
    void scanNumber();
    void scanPreprocessorDirective();
    void skipComment();
    void skipSingleComment();
    std::string getStringNameOfLex(Lex lex);
    int scanEscapeSequence();
    int scanUnicodeEscape();
    int scanHexEscape();
private:
    Driver driver;
    std::shared_ptr<Error> errorPtr;
    Freq* freqPtr;
    // служебные слова
    std::unordered_map<std::string, Lex> lexTable = {
        {"abstract", Lex::ABSTRACT},
        {"as", Lex::AS},
        {"base", Lex::BASE},
        {"bool", Lex::BOOL},
        {"break", Lex::BREAK},
        {"byte", Lex::BYTE},
        {"case", Lex::CASE},
        {"catch", Lex::CATCH},
        {"char", Lex::CHAR},
        {"checked", Lex::CHECKED},
        {"class", Lex::CLASS},
        {"const", Lex::CONST},
        {"continue", Lex::CONTINUE},
        {"decimal", Lex::DECIMAL},
        {"default", Lex::DEFAULT},
        {"delegate", Lex::DELEGATE},
        {"do", Lex::DO},
        {"double", Lex::DOUBLE},
        {"else", Lex::ELSE},
        {"enum", Lex::ENUM},
        {"event", Lex::EVENT},
        {"explicit", Lex::EXPLICIT},
        {"extern", Lex::EXTERN},
        {"false", Lex::FALSE},
        {"finally", Lex::FINALLY},
        {"fixed", Lex::FIXED},
        {"float", Lex::FLOAT},
        {"for", Lex::FOR},
        {"foreach", Lex::FOREACH},
        {"goto", Lex::GOTO},
        {"if", Lex::IF},
        {"implicit", Lex::IMPLICIT},
        {"in", Lex::IN},
        {"int", Lex::INT},
        {"interface", Lex::INTERFACE},
        {"internal", Lex::INTERNAL},
        {"is", Lex::IS},
        {"lock", Lex::LOCK},
        {"long", Lex::LONG},
        {"namespace", Lex::NAMESPACE},
        {"new", Lex::NEW},
        {"null", Lex::NULL_},
        {"object", Lex::OBJECT},
        {"operator", Lex::OPERATOR},
        {"out", Lex::OUT},
        {"override", Lex::OVERRIDE},
        {"params", Lex::PARAMS},
        {"private", Lex::PRIVATE},
        {"protected", Lex::PROTECTED},
        {"public", Lex::PUBLIC},
        {"readonly", Lex::READONLY},
        {"ref", Lex::REF},
        {"return", Lex::RETURN},
        {"sbyte", Lex::SBYTE},
        {"sealed", Lex::SEALED},
        {"short", Lex::SHORT},
        {"sizeof", Lex::SIZEOF},
        {"stackalloc", Lex::STACKALLOC},
        {"static", Lex::STATIC},
        {"string", Lex::STRING},
        {"struct", Lex::STRUCT},
        {"switch", Lex::SWITCH},
        {"this", Lex::THIS},
        {"throw", Lex::THROW},
        {"true", Lex::TRUE},
        {"try", Lex::TRY},
        {"typeof", Lex::TYPEOF},
        {"uint", Lex::UINT},
        {"ulong", Lex::ULONG},
        {"unchecked", Lex::UNCHECKED},
        {"unsafe", Lex::UNSAFE},
        {"ushort", Lex::USHORT},
        {"using", Lex::USING},
        {"virtual", Lex::VIRTUAL},
        {"void", Lex::VOID},
        {"volatile", Lex::VOLATILE},
        {"var", Lex::VAR},
        {"while", Lex::WHILE},
    };

    std::unordered_map<Lex, std::string> lexToStr = {
        {Lex::NAME, "идентификатор"},
        {Lex::NUMBER, "число"},
        {Lex::DECIMAL, "десятичное число"},
        {Lex::FLOAT, "вещественное число (float)"},
        {Lex::DOUBLE, "вещественное число (double)"},

        {Lex::STRINGLITERAL, "строковый литерал"},
        {Lex::CHARLITERAL, "символьный литерал"},
        {Lex::PREPROC_DIRECTIVE, "#"},

        // Операторы
        {Lex::PLUS, "+"},
        {Lex::MINUS, "-"},
        {Lex::MULT, "*"},
        {Lex::DIV, "/"},
        {Lex::MOD, "%"},
        {Lex::BITWISE_NOT, "~"},
        {Lex::BITWISE_XOR, "^"},
        {Lex::XOR_ASSIGN, "^="},
        {Lex::AND_ASSIGN, "&="},
        {Lex::OR_ASSIGN, "|="},
        {Lex::LSHIFT, "<<"},
        {Lex::RSHIFT, ">>"},
        {Lex::LSHIFT_ASSIGN, "<<="},
        {Lex::RSHIFT_ASSIGN, ">>="},
        {Lex::NULL_COALESCING_ASSIGN, "??="},
        {Lex::PLUS_ASSIGN, "+="},
        {Lex::MINUS_ASSIGN, "-="},
        {Lex::MULT_ASSIGN, "*="},
        {Lex::DIV_ASSIGN, "/="},
        {Lex::MOD_ASSIGN, "%="},
        {Lex::ASSIGN, "="},
        {Lex::EQ, "=="},
        {Lex::NE, "!="},
        {Lex::LT, "<"},
        {Lex::LE, "<="},
        {Lex::GT, ">"},
        {Lex::GE, ">="},
        {Lex::AND, "&&"},
        {Lex::OR, "||"},
        {Lex::NOT, "!"},
        {Lex::BITWISE_AND, "&"},
        {Lex::BITWISE_OR, "|"},
        {Lex::INCREMENT, "++"},
        {Lex::DECREMENT, "--"},
        {Lex::ARROW, "=>"},
        {Lex::QUESTION, "?"},
        {Lex::NULL_COALESCING, "??"},
        {Lex::NULL_CONDITIONAL , ".?"},

        // Разделители
        {Lex::DOT, "."},
        {Lex::COMMA, ","},
        {Lex::COLON, ":"},
        {Lex::SEMICOLON, ";"},
        {Lex::LPAREN, "("},
        {Lex::RPAREN, ")"},
        {Lex::LBRACE, "{"},
        {Lex::RBRACE, "}"},
        {Lex::LBRACKET, "["},
        {Lex::RBRACKET, "]"},

        {Lex::EOT, "конец файла"}
    };
};