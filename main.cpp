/*#include <iostream>
#include <filesystem>
#include "include/allocator.hpp"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    std::cout << "--- ETAPE 1 : Allocations ---" << std::endl;
    void* p1 = my_malloc(1);
    void* p2 = my_malloc(100);
    display_mem_status(); // Doit montrer 3 blocs (BUSY, BUSY, FREE)9

    std::cout << "\n--- ETAPE 2 : Liberation de p1 ---" << std::endl;
    //my_free(p1);
    display_mem_status(); // Doit montrer (FREE, BUSY, FREE)

    std::cout << "\n--- ETAPE 3 : Liberation de p2 (FUSION) ---" << std::endl;
    //my_free(p2);
    display_mem_status(); // DOIT REVENIR A UN SEUL BLOC GEANT !

    std::cout << "Le fichier sera cree ici : " << std::filesystem::current_path() << std::endl;
    export_to_json("heap_data.json");
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.
*/
#include <iostream>
#include <vector>
#include <ctime>
#include <filesystem>
#include "include/allocator.hpp"

void stress_test(int iterations) {
    std::vector<void*> ptrs;
    srand(time(NULL));

    std::cout << "Lancement du Stress Test : " << iterations << " operations..." << std::endl;

    for (int i = 0; i < iterations; ++i) {
        int action = rand() % 10;

        if (action < 7) { // 70% de chance d'allouer
            size_t size = (rand() % 200) + 1; // Tailles entre 1 et 200 octets
            void* p = my_malloc(size);
            if (p) {
                ptrs.push_back(p);
            }
        } /*else { // 30% de chance de libérer
            if (!ptrs.empty()) {
                int index = rand() % ptrs.size();
                my_free(ptrs[index]);
                ptrs.erase(ptrs.begin() + index);
            }
        }*/
    }

    std::cout << "Stress test termine. Nettoyage final..." << std::endl;
    for (void* p : ptrs) {
        my_free(p);
    }
    std::cout << "Le fichier sera cree ici : " << std::filesystem::current_path() << std::endl;
    export_to_json("stress_test_final.json");
    display_mem_status();
}

int main() {
    initialize_allocator();

    stress_test(500); // On commence avec 500 opérations
//T(n) = 2T(n-1) + n   => 2^n
    //T(n) = 4T(n/2) + n
    return 0;
}