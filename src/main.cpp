#include "precalc.cpp"
#include <chrono>


int main(int argc, char* argv[])
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    int K_[] = {1 << 10, 1 << 12, 1 << 14};
    int L_[] = {1 << 5, 1 << 6, 1 << 7};


    // first attack;
    std::vector<double> times(9);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << "For K = " << K_[i] << " and L = " << L_[j] << std::endl;
            auto t1 = high_resolution_clock::now();
            build_precalc_table_in_memory_multithreaded(2, K_[i], L_[j]);
            auto t2 = high_resolution_clock::now();
            auto ms_int = duration_cast<milliseconds>(t2 - t1);
            duration<double, std::milli> ms_double = t2 - t1;
            std::cout << ms_double.count() << "ms\n";
            times[i] = ms_double.count();
        }
    }
    double time_sum = 0;
    for (int i = 0; i < 9; ++i) {
        time_sum += times[i];
    }
    std::cout << "Total time ";
    std::cout << time_sum << "ms" <<std::endl;
    
    
    // ---------------------------------------


    // second attack

    

    /*
    std::vector<double> times(9);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << "For K = " << K_[i] << " and L = " << L_[j] << std::endl;
            auto t1 = high_resolution_clock::now();
            build_precalc_table_in_memory_multithreaded(K_[i], K_[i], L_[j]);
            auto t2 = high_resolution_clock::now();
            auto ms_int = duration_cast<milliseconds>(t2 - t1);
            duration<double, std::milli> ms_double = t2 - t1;
            std::cout << ms_double.count() << "ms\n";
            times[i] = ms_double.count();
        }
    }
    double time_sum = 0;
    for (int i = 0; i < 9; ++i) {
        time_sum += times[i];
    }
    std::cout << "Total time ";
    std::cout << time_sum << "ms" <<std::endl;
    */
    return 0;
}