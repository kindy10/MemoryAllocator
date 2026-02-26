//
// Created by HP on 2/20/2026.
//
#include  "allocator.hpp"
#include <fstream>
#define POOL_SIZE 4096

//la memoire brute  (le "Heap" simule)
static char memory_pool[POOL_SIZE];

//le pointeur vers le debut de la liste de blocs
static Block* freeList = nullptr;

/*
 *Initialise le pool de memoire avec un seul bloc geant
 */
void initialize_allocator() {
    //On place notre premier Header tout aud debut du tableau
    freeList = (Block*)memory_pool;

    //La taille disponible est : Taille Totale - Taille du header
    freeList-> size = POOL_SIZE -  sizeof(Block);
    freeList->is_free = true;
    freeList-> next = nullptr;

    std::cout << "Allocator initialized with "<<POOL_SIZE<<" bytes."<<std::endl;
    std::cout<<"Header size: "<<sizeof(Block)<<" bytes."<<std::endl;
}
void* my_malloc(size_t size) {
    if (size == 0) return nullptr;
    //On arrondit la taille demandee au multiple de 8 le plus proche
    size = (size + 7) & ~7;
//Initailiser si ce n'est pas fait (securite)

    if (freeList == nullptr) initialize_allocator();

    Block* current = freeList;
    //2 Parcourir la liste pour trouver un bloc libre assez grand
    while (current != nullptr) {
        if (current->is_free && current->size >= size) {

            // ---Debug----;
            std::cout<<"[DEBUG] On a trouve un bloc ! Taille demandee: "<<size <<std::endl;
            //Logic de splitting
            if (current->size >= (size + sizeof(Block) + 8)) {
                Block* newBlock = (Block*)((char*)(current) + sizeof(Block) + size);
                newBlock ->size = current->size - size - sizeof(Block);
                newBlock-> is_free = true;
                newBlock-> next = current->next;

                current->size = size;
                current->next = newBlock;
            }
            current->is_free = false;
            return (void*)(current +1);
        }
        current = current->next;
    }
    return nullptr; // Plus de place !!
}
void display_mem_status() {
    Block* current = freeList;
    int i = 0;
    std::cout<<"\n----Memory status-----:"<<std::endl;
    while (current != nullptr) {
        std::cout<<"Bloc "<<i++<<"| Address: "<<current
            <<" | Size: "<<current->size
            <<" | Stat: "<<(current->is_free? "fREE" : "BUSY")
            <<" | Next: "<<current->next <<std::endl;
        current = current->next;
    }
    std::cout<<"-------------------------------\n"<<std::endl;
}
void my_free(void* ptr) {
    if (ptr == nullptr) return;

    //1.Retrouver le Header a partir du pointeur de l'utilisateur
    //On recule de 1 Header

    Block* blockToFree = (Block*)ptr -1;

    //2 Marquer comme libre
    blockToFree->is_free = true;
    std::cout<<"[FREE] Bloc at  "<<blockToFree<<" is now free"<<std::endl;

    //3.COALESCENCE (Fusionner les blocs libres adjacents)
    Block* current = freeList;
    while (current != nullptr && current->next != nullptr) {
        if (current->is_free && current->next->is_free) {
            //On fusionne : Taille actuelle + header du suivant + Taille du suivant
            current->size +=sizeof(Block) + current->next->size;
            //On "saute " le bloc suivant car il est maintenant integre au premier;
            current->next = current->next->next;

            //On ne passe pas au bloc suivant (current= current->next) car le nouveau
            //bloc fusionne peut peut_etre encore fusionner avec son nouveau voisin!
            continue;
        }
        current = current->next;
    }

}

void export_to_json(const std:: string & filename) {
    std::ofstream file(filename);
    file <<"[\n";
    Block* current = freeList;
    while (current) {
        file << "  {\"address\": \"" << current << "\", "
            << "\"size\": " << current->size << ", "
            << "\"free\": " << (current->is_free ? "true" : "false") << "}";
        if (current->next) file <<",";
        file << "\n";
        current = current->next;
    }
    file <<"]";
    file.close();
    std::cout<<"Data exported to "<<filename<<std::endl;
}


