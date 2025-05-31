#include "iostream"
#include "TextDriver/Driver.h"
#include "error/Error.h"
#include "Scann/Scanner.h"
#include "frequent/Freq.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <locale>
#include <memory> 

int main(int argc, char const* argv[])
{
    setlocale(LC_ALL, "Russian");
    std::cout << "Лексический анализатор языка С#" << std::endl;
    std::shared_ptr<Error> errorPtr = std::make_shared<Error>();
    Driver driver(errorPtr);

    if (argc < 2)
    {
        errorPtr->error("В качестве аргумента должен быть указан файл *.cs");
    }
    Freq freq;
    for (int i = 1; i < argc; i++) {
        driver.resetText(argv[i]);
        Scanner scanner(driver, errorPtr, &freq);


        while (Scanner::lex != Lex::EOT) {
            scanner.nextLex();
            if (scanner.lex != Lex::NONE) {
                freq.put(scanner.lex);
            }

        }
    }
   
    std::cout << std::endl;
    freq.print();

    return 0;
}
