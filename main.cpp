//
//  main.cpp
//  NN
//
//  Created by Eric Wolos on 1/18/23.
//

#include <iostream>
#include "Model.h"
#include <vector>
#include "matrix.h"
#include "functions.h"
#include <set>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

void delete_data(vector<pair<vector<float>*, int*>> data)
{
    for (int i = 0; i < data.size(); i++)
    {
        delete data[i].first;
        delete data[i].second;
    }
}

vector<pair<vector<float>*, int*>> get_data (string filename)
{
    string file_name = filename;
    
    vector<pair<vector<float>*, int*>> data;

    string line, word;

    fstream file (file_name, ios::in);

    bool firstLine = true;

    if(file.is_open())
    {
    while(getline(file, line))
    {
        // Skip first line
        if (firstLine) {firstLine = false; continue;}

        vector<float>* data_point = new vector<float>;

        stringstream str(line);

        bool firstWord = true;
        int* label = new int;

        while(getline(str, word, ','))
        {

            int w = stoi(word);
            if (firstWord)
            {
                *label = w;
                firstWord = false;
            }
            else
            {
                data_point->push_back(w);
            }
        }
        data.push_back(pair(data_point, label));
    }
    }
    return data;
}

// Main routine
int main(int argc, const char * argv[]) {
     
    
    time_t time_begin = time(NULL);

    vector<pair<vector<float>*, int*>> train_data = get_data("/Users/ericwolos/ML/NNscratch/mnist_train.csv");
    
    int input_vector_size = 784;
    int output_vector_size = 10;
    float initial_lr = 0.001;
    
    Model m(input_vector_size,"CCE", initial_lr);
    
    m.add_layer(128, "Sigmoid");
    
    m.add_layer(output_vector_size, "Sigmoid");
    
    m.get_layer_sizes();
    
    unsigned long data_size = train_data.size();
    unsigned long correct = 0;
    unsigned long total_correct = 0;
    unsigned long total_trained_points = 50;
    unsigned long output_chunk_size = 1000;
    
    float prev_accuracy = 0.3;
    float lr_momentum = 0.98;
    
    // Training
    for (int trained_points = 0; trained_points < total_trained_points; trained_points++)
    {
        pair<vector<float>*, int*> data_point = train_data[trained_points % data_size];
        vector <float> input = *data_point.first;
        
        int label = *data_point.second;
        vector <float> prediction = m.forward(input);
        
        if (max::soft_arg_max(prediction) == label) {correct++; total_correct++;}
        
        if (trained_points % 25 == 0 && trained_points != 0) cout << '=';
        
        if (trained_points % output_chunk_size == 0 && trained_points != 0)
        {
            cout << endl;
            cout << "finished training point: " << trained_points <<  endl;
            cout << "current loss: " << m.get_loss() <<  endl;
            float accuracy = correct / float(output_chunk_size);
            cout << "current chunk accuracy: " << accuracy <<  endl;
            correct = 0;
            
            if (accuracy > prev_accuracy)
            {
                m.set_lr(m.get_lr() * lr_momentum);
                prev_accuracy = accuracy;
            }
            
            
        }


        m.backward(prediction,max::OHE(label, output_vector_size));

    }
    
    cout << endl;
    cout << "final train accuracy:" << total_correct / float(total_trained_points) << endl;
    
    delete_data(train_data);
    
    time_t time_end = time(NULL);
    
    cout << endl;
    cout << "Training took " << time_end - time_begin << " seconds" << endl;
    
    vector<pair<vector<float>*, int*>> test_data = get_data("/Users/ericwolos/ML/NNscratch/mnist_test.csv");
    
    int test_correct = 0;
    
    // Testing
    for (int tested_points = 0; tested_points < test_data.size(); tested_points++)
    {
        
        pair<vector<float>*, int*> data_point = test_data[tested_points];
        vector <float> input = *data_point.first;
        
        int label = *data_point.second;
        vector <float> prediction = m.forward(input);
        
        if (max::soft_arg_max(prediction) == label) {test_correct++;}
        
        if (tested_points % 25 == 0 && tested_points != 0) cout << '=';
        
        if (tested_points % output_chunk_size == 0 && tested_points != 0)
        {
            cout << endl;
        }
        m.clear_layer_outputs();

    }
    
    
    float test_accuracy = test_correct / float(test_data.size());
    cout << endl;
    cout << endl;
    cout << "-----------------------" << endl;
    cout << "final test accuracy:" << test_accuracy << endl;
    cout << endl;
    
    delete_data(test_data);
    
    
    return 0;
}
