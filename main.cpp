#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"
#include <regex>

using namespace std;

const int SIZE = 1000; //1e9
const int THREADS = 8;

// =========================== SEQUENTIAL ==========================
    void resaltado_sec(int argc, char* argv[]){
        for (int i = 1; i < argc; i++){
            fstream arch;
            arch.open(argv[i], ios::in);
            string line;
            do{
                getline(arch,line);
                cout <<line;
            } while (!arch.eof());
            
        }

        string boilerplate = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<meta charset=\"UTF-8\">\n\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Document</title>\n</head>\n<body>\n";


        vector<string> filenames(argv, argv+argc);
        filenames.erase(filenames.begin());
        for (int i = 0; i < filenames.size(); i++){
            string tmp = filenames[i];
            tmp = tmp.replace(0,filenames[i].find('/')+1,"");
            tmp = tmp.replace((tmp.size()-4),4,"");
            filenames[i] = tmp;

            fstream arch;
            arch.open("output_files/" + filenames[i] + ".html", ios::out);
            arch<<"el archivo " + filenames[i] + " fue creado";
            arch.close();
        }
    }
// =================================================================

// =========================== CONCURRENT ==========================

typedef struct {
    int start, end, *array;
    double val;
 } Block;

void* task(void* param){
    Block *block;
    double acum = 0;
    block = (Block*) param;
    for (int i = block->start; i < block->end; i++) {
        if(block->array[i] % 2 == 0){
            acum += block->array[i];
        }
    }
    block->val = acum;
    pthread_exit(0);
}
// =================================================================

void createHTML(int argc, char* argv[]){
    fstream arch;
    vector<string> filenames(argv, argv+argc);
    filenames.erase(filenames.begin());
    for (int i = 0; i < filenames.size(); i++){
        string tmp = filenames[i];
        tmp = tmp.replace(0,filenames[i].find('/')+1,"");
        tmp = tmp.replace((tmp.size()-4),4,"");
        filenames[i] = tmp;

        arch.open("output_files/" + filenames[i] + ".html", ios::out);
        arch<<"el archivo " + filenames[i] + " fue creado";
        arch.close();
    }
}

//llamar códgio con input_files/*.cpp
int main(int argc, char* argv[]){
    double sequential, concurrent, result;
    int *array, blockSize;
    pthread_t tids[THREADS];
    Block blocks[THREADS];  
    array = new int[SIZE];
    random_array(array, SIZE);

    //==========Secuencial=========
    sequential = 0;
    start_timer();

    resaltado_sec(argc, argv);
    
    sequential += stop_timer();
    cout << "result = " << fixed << setprecision(0) << result << endl;
    cout << "sequential average time = " << setprecision(5) << (sequential / N) << " ms" << endl;

    // ============Concurrente===============
    blockSize = SIZE/THREADS;
    for (int i = 0; i < THREADS; i++) {
        blocks[i].start = i * blockSize;
        blocks[i].end = (i != (THREADS - 1))? (i + 1) * blockSize : SIZE;
        blocks[i].array = array;
    }
    concurrent = 0;
    for(int i = 0; i < N; i++){
        start_timer();
        for(int j = 0; j < THREADS; j++){
            pthread_create(&tids[j], NULL, task, &blocks[j]);
        }
        result = 0;
        for(int j = 0; j < THREADS; j++){
            pthread_join(tids[j], NULL);
            result += blocks[j].val;
        }
        concurrent += stop_timer();
    }

    cout << "result = " << fixed << setprecision(0) << result << endl;
    cout << "concurrent average time = " << setprecision(5) << (concurrent / N) << " ms" << endl;

    cout << "speed up = " << setprecision(5) << (sequential / concurrent) << " ms" << endl;
    
    delete [] array;

    createHTML(argc,argv);
    return 0;
}
