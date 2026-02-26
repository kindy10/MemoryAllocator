#ifndef ALLOCATOR_HPP // C'est une "Header Guard" ca empeche le compilateur d'importer deux fois le meme fichier, ce qui causerait des erreurs
#define ALLOCATOR_HPP

#include <cstddef> // for size_t
#include <iostream>

//Structure of Headr for eahc bloc of memory

struct Block{
      size_t size;  // Size of the bloc (without the header)
      bool is_free;  //  true if bloc is disponible
      Block* next;  // Pointeru for the next bloc

};

//Initialization of the pool of memoir(Notre Heap prive)
void initialize_allocator();

//Alloue de la memoir ( equivalent to malloc)
void* my_malloc(size_t size);

//Free  the memory ( equivalent to free)
void my_free(void* p);

//Display  memory (for debug)
void display_mem_status();

//Export data for python
void export_to_json(const std:: string & filename);
#endif //Allocator_HPP