#include "pch.h"

struct berResults
{
    double tot; //total number of bits
    double err; //error bits (different bits)
    float ber;  //ber calc result
    clock_t t1; //calc start time
    clock_t t2; //calc stop time
};

uint8_t hammingDistance(uint8_t n1, uint8_t n2);
void createFile1(const std::string name, const int count, const char value);
berResults calculateBer(std::string fpath1, std::string fpath2);
void printResult(berResults results);

int main(int argc, char * argv[])
{
    std::string fpath1; //path to file 1
    std::string fpath2; //path to file 2
    berResults results; 

    openLog("log.log"); //create log.log file for logging purposes
    if (argc != 3) //check the number of arguments
    {
        saveLog("User did not provide path to the files.");
        saveLog("Creating the test files for user...");
        //test 1
        createFile1("test1_file1.bin", 100, 0xFF); //1111 1111
        createFile1("test1_file2.bin", 100, 0xFE); //1111 1110
        //test 2
        
        /*
            missing function to change 10 bits in the file
        */

        //test 3
        //createFile1("test3_file1.bin",400000000,0x55); 
        //createFile1("test3_file2.bin",400000000,0x50);
        saveLog("Test files are prepared");
        saveLog("Re-run with correct arguments ie: ./task_iv_ber.exe test1_file1.bin test1_file2.bin");
    }
    else //arguments are passed correctly
    {
        fpath1 = argv[1];
        fpath2 = argv[2];

        saveLog("Processing test files");
        results = calculateBer(fpath1, fpath2);
        printResult(results);
    }

    closeLog();
    return 0;
}

uint8_t hammingDistance(uint8_t n1, uint8_t n2)
{
    uint8_t x = n1 ^ n2; // XOR
    uint8_t setBits = 0;
    while (x > 0)
    {
        setBits += x & 1;
        x >>= 1;
    }
    return setBits;
}

void createFile1(const std::string name, const int count, const char value)
{
    std::fstream f;
    f.open(name.c_str(), std::ios::binary | std::ios::out);
    for (int i = 0; i < count; i++)
    {
        f.write((char*)&value,1);
    }
    f.close();
}

berResults calculateBer(std::string fpath1, std::string fpath2)
{
    std::fstream f1, f2; //handlers to files
    berResults results;
    results.t1 = 0;
    results.t2 = 0;
    results.ber = 0;
    results.err = 0;
    results.tot = 0;

    saveLog("Calculating BER...");
    f1.open(fpath1.c_str(), std::ios::binary | std::ios::in);
    f2.open(fpath2.c_str(), std::ios::binary | std::ios::in);
    char a = 0x00;
    char b = 0x00;
    results.t1 = clock();

    while (!f1.eof())
    {
        f1 >> a; //read 1 char from file 1
        f2 >> b; //read 1 char from file 2
        if (!f1.eof()) // till the end of the 1st file
        {
            results.err += hammingDistance(a, b); //add to the .err the number of different bits
            results.tot += 8; //add to the .tot the number of compared bits
        }
    }

    results.ber = (float)results.err / results.tot; // calculate ber
    results.t2 = clock(); 
    saveLog("BER calculations are done");
    return results; //return structure with all results
}

void printResult(berResults results)
{
    std::stringstream message;
    message << "Results are: " << std::endl;
    message << "BER: " << results.ber << std::endl;
    message << "Tot: " << results.tot << std::endl;
    message << "Err: " << results.err << std::endl;
    message << "Calc time: " << ((float)results.t2 - results.t1) / CLOCKS_PER_SEC << " sec " << std::endl;
    saveLog(message.str());
}