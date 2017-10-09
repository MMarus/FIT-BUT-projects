/*
 * algorithm: odd-even transposition sort (alg. ~40 lines long)
 * author: jakub zak
 *
 */

#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/time.h>

using namespace std;

#define TAG 0

int main(int argc, char *argv[])
{
    int numprocs;               //pocet procesoru
    int myid;                   //muj rank
    int neighnumber;            //hodnota souseda
    int mynumber;               //moje hodnota
    MPI_Status stat;            //struct- obsahuje kod- source, tag, error
    int tagX = 0;
    int tagY = 1;
    int tagYI = 2;
    int tagZ = 3;
    int tagR = 4;
    int empty = -500;

    //MPI INIT
    MPI_Init(&argc,&argv);                          // inicializace MPI
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);       // zjistíme, kolik procesů běží
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);           // zjistíme id svého procesu

    //NACTENI SOUBORU
    /* -proc s rankem 0 nacita vsechny hodnoty
     * -postupne rozesle jednotlive hodnoty vsem i sobe
    */


    if(myid == 0){


        char input[]= "numbers";                          //jmeno souboru
        int number;                                     //hodnota pri nacitani souboru
        vector<int> inputNumbers;
        vector<int> outputNumbers;
        fstream fin;                                    //cteni ze souboru
        fin.open(input, ios::in);

        while(fin.good()){
            number= fin.get();
            if(!fin.good()) break;                      //nacte i eof, takze vyskocim
            inputNumbers.push_back(number);
        }//while
        fin.close();

        for (int i = 0; i < inputNumbers.size(); i++) {
            cout << inputNumbers[i];
            if(i<inputNumbers.size()-1)
                cout << " ";
            else
                cout << endl;
        }

        if(inputNumbers.size() +1 != numprocs) {
            cerr << "The number +1 of processors " << numprocs << " does not match count of numbers to sort = " << inputNumbers.size() << endl;
            MPI_Finalize();
            return 1;
        }

        timespec tS;
        tS.tv_sec = 0;
        tS.tv_nsec = 0;
        clock_settime(CLOCK_PROCESS_CPUTIME_ID, &tS);
        for (int i = 0; i < inputNumbers.size(); ++i) {
            //Send Yi to proc 1
            MPI_Send(&inputNumbers[i], 1, MPI_INT, 1, tagY, MPI_COMM_WORLD); //buffer,velikost,typ,rank prijemce,tag,komunikacni skupina
            MPI_Send(&inputNumbers[i], 1, MPI_INT, i+1, tagX, MPI_COMM_WORLD); //buffer,velikost,typ,rank prijemce,tag,komunikacni skupina
        }
        for (int i = 0; i < inputNumbers.size() ; ++i) {
            int output = 0;
            MPI_Recv(&output, 1, MPI_INT, inputNumbers.size(), tagR, MPI_COMM_WORLD, &stat); //buffer,velikost,typ,rank odesilatele,tag, skupina, stat
            outputNumbers.push_back(output);
        }
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tS);
//        Zapis merania casu
//        fstream fout;
//        fout.open("times", ios::app);
//        fout << outputNumbers.size() << ";" << tS.tv_sec << ";" << tS.tv_nsec << endl;
//        fout.close();

        for (int i = 1; i <= outputNumbers.size(); i++) {
            cout<< outputNumbers[outputNumbers.size()-i] << endl;
        }
    }//nacteni souboru
    else{
        int count = 1;
        int countOfNumbers = numprocs-1;
        int x = empty;
        int y = empty;
        int yIndex = empty;
        int z = empty;
        int h;

        for (int i = 1; i <= countOfNumbers*2; ++i) {
            if(i<=countOfNumbers){
                if(myid == 1){
                    MPI_Recv(&y, 1, MPI_INT, 0, tagY, MPI_COMM_WORLD, &stat); //buffer,velikost,typ,rank odesilatele,tag, skupina, stat
                    yIndex = i;
                }
                if(myid == i) {
                    MPI_Recv(&x, 1, MPI_INT, 0, tagX, MPI_COMM_WORLD, &stat); //buffer,velikost,typ,rank odesilatele,tag, skupina, stat
                }
            }

            if(i<=countOfNumbers)
                h = 1;
            else
                h = i-countOfNumbers+1;

            if(myid >= h) {

                if(x!=empty && y!=empty) {
                    if ( x>y )
                        count++;
                    else if(x == y && yIndex < myid)
                        count++;
                }
            }


            if(myid <= countOfNumbers-1) {
                MPI_Send(&y, 1, MPI_INT, myid+1, tagY, MPI_COMM_WORLD); //buffer,velikost,typ,rank prijemce,tag,komunikacni skupina
                MPI_Send(&yIndex, 1, MPI_INT, myid+1, tagYI, MPI_COMM_WORLD); //buffer,velikost,typ,rank prijemce,tag,komunikacni skupina
            }
            if(myid > 1) {
                MPI_Recv(&y, 1, MPI_INT, myid-1, tagY, MPI_COMM_WORLD, &stat); //buffer,velikost,typ,rank odesilatele,tag, skupina, stat
                MPI_Recv(&yIndex, 1, MPI_INT, myid-1, tagYI, MPI_COMM_WORLD, &stat); //buffer,velikost,typ,rank odesilatele,tag, skupina, stat
            }

            if(i>=countOfNumbers && myid == h) {
                MPI_Send(&x, 1, MPI_INT, count, tagZ, MPI_COMM_WORLD); //buffer,velikost,typ,rank prijemce,tag,komunikacni skupina
            }
        }
        MPI_Recv(&z, 1, MPI_INT, MPI_ANY_SOURCE, tagZ, MPI_COMM_WORLD, &stat); //buffer,velikost,typ,rank odesilatele,tag, skupina, stat

        for (int i = 0; i < countOfNumbers; i++) {
            if(myid < countOfNumbers) {
                MPI_Send(&z, 1, MPI_INT, myid+1, tagR, MPI_COMM_WORLD); //buffer,velikost,typ,rank prijemce,tag,komunikacni skupina
            }
            if(myid == countOfNumbers) {
                MPI_Send(&z, 1, MPI_INT, 0, tagR, MPI_COMM_WORLD); //buffer,velikost,typ,rank prijemce,tag,komunikacni skupina
            }
            if(myid > 1) {
                MPI_Recv(&z, 1, MPI_INT, myid-1, tagR, MPI_COMM_WORLD, &stat); //buffer,velikost,typ,rank odesilatele,tag, skupina, stat
            }
        }

    }

    MPI_Finalize();
    return 0;

}//main

