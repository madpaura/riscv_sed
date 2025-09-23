#pragma once

#include "simple_map.h"
#include <cstdint>

// Sample C++ class demonstrating standard C++ features
class DataProcessor {
private:
    SimpleMap<int, int> data_map;
    int* dynamic_array;
    size_t array_size;
    static int instance_count;
    
public:
    // Constructor
    DataProcessor(size_t initial_size = 10);
    
    // Destructor
    ~DataProcessor();
    
    // Copy constructor
    DataProcessor(const DataProcessor& other);
    
    // Assignment operator
    DataProcessor& operator=(const DataProcessor& other);
    
    // Add data to the map
    void add_data(int key, int value);
    
    // Get data from the map
    int* get_data(int key);
    
    // Process data using dynamic array
    void process_array_data(const int* input, size_t input_size);
    
    // Get processed results
    const int* get_processed_data() const { return dynamic_array; }
    size_t get_array_size() const { return array_size; }
    
    // Static method to get instance count
    static int get_instance_count() { return instance_count; }
    
    // Demonstrate map operations
    void demonstrate_map_operations();
    
    // Demonstrate dynamic memory operations
    void demonstrate_dynamic_memory();
    
    // Print statistics
    void print_statistics() const;
};
