
#include <iostream>
#include <omp.h>

void oddEvenSort(int a[], int n, int thread_count) {
    int i, tmp, phase;
    #pragma omp parallel num_threads(thread_count) default(none) shared(a, n) private(i, tmp, phase)
    {
        for (phase = 0; phase < n; phase++) {
            if (phase % 2 == 0) {
                #pragma omp for
                for (i = 1; i < n; i += 2) {
                    if (a[i-1] > a[i]) {
                        tmp = a[i-1];
                        a[i-1] = a[i];
                        a[i] = tmp;
                    }
                }
            } else {
                #pragma omp for
                for (i = 1; i < n-1; i += 2) {
                    if (a[i] > a[i+1]) {
                        tmp = a[i+1];
                        a[i+1] = a[i];
                        a[i] = tmp;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <array_size> <thread_count>\n";
        return 1;
    }

    int n = std::stoi(argv[1]); // Dizi boyutu
    int thread_count = std::stoi(argv[2]); // İş parçacığı sayısı

    int a[n];

    // Diziye rasgele değerler ata
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        a[i] = rand() % 1000; // 0-99 arası rasgele değerler
    }

    std::cout << "Before sorting:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;

    // Sıralamayı yap
    double start_time=omp_get_wtime();	
    oddEvenSort(a, n, thread_count);
    double end_time=omp_get_wtime();
    double time_taken=end_time-start_time;

    std::cout << "After sorting:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    std::cout<<"Time Elapsed->"<<time_taken<<std::endl;
    
    return 0;
}

