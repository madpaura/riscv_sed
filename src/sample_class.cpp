#include "sample_class.h"
#include "memory.h"

// Static member definition
int DataProcessor::instance_count = 0;

DataProcessor::DataProcessor(size_t initial_size) : array_size(initial_size) {
    // Allocate dynamic array
    dynamic_array = new int[array_size];
    
    // Initialize array with zeros
    for (size_t i = 0; i < array_size; ++i) {
        dynamic_array[i] = 0;
    }
    
    // Increment instance count
    instance_count++;
}

DataProcessor::~DataProcessor() {
    delete[] dynamic_array;
    instance_count--;
}

DataProcessor::DataProcessor(const DataProcessor& other) 
    : data_map(other.data_map), array_size(other.array_size) {
    
    // Deep copy dynamic array
    dynamic_array = new int[array_size];
    for (size_t i = 0; i < array_size; ++i) {
        dynamic_array[i] = other.dynamic_array[i];
    }
    
    instance_count++;
}

DataProcessor& DataProcessor::operator=(const DataProcessor& other) {
    if (this != &other) {
        // Clean up existing resources
        delete[] dynamic_array;
        
        // Copy data
        data_map = other.data_map;
        array_size = other.array_size;
        
        // Deep copy dynamic array
        dynamic_array = new int[array_size];
        for (size_t i = 0; i < array_size; ++i) {
            dynamic_array[i] = other.dynamic_array[i];
        }
    }
    return *this;
}

void DataProcessor::add_data(int key, int value) {
    data_map.insert(key, value);
}

int* DataProcessor::get_data(int key) {
    return data_map.find(key);
}

void DataProcessor::process_array_data(const int* input, size_t input_size) {
    // Resize array if needed
    if (input_size > array_size) {
        delete[] dynamic_array;
        array_size = input_size;
        dynamic_array = new int[array_size];
    }
    
    // Process data (simple transformation: multiply by 2 and add 1)
    for (size_t i = 0; i < input_size; ++i) {
        dynamic_array[i] = input[i] * 2 + 1;
    }
    
    // Fill remaining elements with zeros
    for (size_t i = input_size; i < array_size; ++i) {
        dynamic_array[i] = 0;
    }
}

void DataProcessor::demonstrate_map_operations() {
    // Add some data to the map
    add_data(1, 100);
    add_data(2, 200);
    add_data(3, 300);
    add_data(4, 400);
    
    // Retrieve and modify data
    int* value = get_data(2);
    if (value) {
        *value = 250; // Modify the value
    }
    
    // Add more data
    add_data(5, 500);
    
    // Try to get non-existent data
    int* missing = get_data(99);
    (void)missing; // Suppress unused variable warning
}

void DataProcessor::demonstrate_dynamic_memory() {
    // Create test input data
    const int test_data[] = {10, 20, 30, 40, 50};
    const size_t test_size = sizeof(test_data) / sizeof(test_data[0]);
    
    // Process the data
    process_array_data(test_data, test_size);
    
    // Create additional dynamic objects
    int* temp_array = new int[3];
    temp_array[0] = 1;
    temp_array[1] = 2;
    temp_array[2] = 3;
    
    // Clean up
    delete[] temp_array;
}

void DataProcessor::print_statistics() const {
    // This would normally print to console, but in baremetal we'll just
    // store the values in variables to demonstrate the functionality
    size_t map_size = data_map.size();
    size_t free_memory = SimpleAllocator::get_free_memory();
    int instances = get_instance_count();
    
    // In a real implementation, these would be printed
    (void)map_size;
    (void)free_memory;
    (void)instances;
}
