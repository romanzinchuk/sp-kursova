#include "stdafx.h"
#include "Controller.h"
#include "Core/Parser/TokenRegister.h"
#include "Core/Parser/TokenParser.h"
#include "Core/Generator/Generator.h"

int main(int argc, std::string* argv)
{
    try
    {
        std::filesystem::path file;

        const std::string extention = ".z05";
        const std::string default_input = "input" + extention;

        const std::string longLine = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";

        std::cout << longLine << std::endl;
        std::cout << "TRANSLATOR (" << extention << "->ASSEMBLER)" << std::endl;
        std::cout << longLine << std::endl;

        if (argc != 2)
        {
            printf("Provide input file name, or type '.' to use default location (%s)\n",default_input.c_str());
            std::cin >> file;
        }
        else
        {
            file = argv->c_str();
        }
        if (file == ".")
            file = default_input;
        Init();

        if (file.extension() != extention)
        {
            std::cout << longLine << std::endl;
            std::cout << "Wrong file extension" << std::endl;
            system("pause");
            return 0;
        }

        std::string fileName = file.replace_extension("").string();
        std::string errorFileName = fileName + "_errors.txt";
        std::string lexemsFileName = fileName + "_lexems.txt";
        std::string tokensFileName = fileName + "_tokens.txt";
        std::string asmFileName = fileName + ".asm";

        std::cout << longLine << std::endl;
        std::cout << "Breaking into lexems are starting..." << std::endl;
        std::fstream inputFile{ fileName + extention, std::ios::in };
        auto tokens = TokenParser::Instance()->tokenize(inputFile);
        inputFile.close();
        std::cout << "Breaking into lexems completed. There are " << tokens.size() << " lexems" << std::endl;

        std::fstream lexemsFile(lexemsFileName, std::ios::out);
        TokenParser::PrintTokens(lexemsFile, tokens);
        lexemsFile.close();
        std::cout << "Report file: " << lexemsFileName << std::endl;
        std::cout << longLine << std::endl;

        std::cout << "Error checking are starting... " << std::endl;
        std::fstream errorFile(errorFileName, std::ios::out);
        auto semanticCheckRes = CheckSemantic(errorFile, tokens);
        errorFile.close();
        if (semanticCheckRes)
        {
            std::cout << "There are no errors in the file" << std::endl;
            std::cout << longLine << std::endl;
        }
        else
        {
            std::cout << "There are errors in the file. Check " << errorFileName << " for more information" << std::endl;
            std::cout << longLine << std::endl;
        }

        std::fstream tokensFile(tokensFileName, std::ios::out);
        TokenParser::PrintTokens(tokensFile, tokens);
        tokensFile.close();
        std::cout << "There are " << tokens.size() << " tokens." << std::endl;
        std::cout << "Report file: " << tokensFileName << std::endl;

        if (semanticCheckRes)
        {
            std::cout << longLine << std::endl;
            std::cout << "Code generation is starting..." << std::endl;
            std::fstream asmFile(asmFileName, std::ios::out);
            Generator::Instance()->generateCode(asmFile, tokens);
            asmFile.close();

            if (std::filesystem::is_directory("masm32"))
            {
                std::cout << "Code generation is completed" << std::endl;
                std::cout << longLine << std::endl;
                system(std::string("masm32\\bin\\ml /c /coff " + fileName + ".asm").c_str());
                system(std::string("masm32\\bin\\Link /SUBSYSTEM:WINDOWS " + fileName + ".obj").c_str());
            }
            else
            {
                std::cout << "WARNING!" << std::endl;
                std::cout << "Can't compile asm file, because masm32 doesn't exist" << std::endl;
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unknown internal error. Better call Saul" << std::endl;
    }

    system("pause");
    return 0;
}