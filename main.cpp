//
//  main.cpp
//  NN
//
//  Created by Eric Wolos on 1/18/23.
//

#include <iostream>
#include "Model.h"
#include "functions.h"
#include <vector>
#include "matrix.h"
#include <set>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

/*
 Inevitably, when a project begins, there appears to be a better way to solve it once your 90% through. Here, I'd argue that the pytorch style implementation is a little better in terms of generality (i.e. more opportunity to add more features). Additionally, all operations are handled through the model class instead of creating seperate matrix operations classes. I have not implemented these because I imagine this involves essentially reinventing numpy (which I have no interest doing)
 
 */

int main(int argc, const char * argv[]) {
    // insert code here...
    /*
     Project Objective:
     Create a simple neural network with
     * A generic structure allowing for as many layers with as great a size as desired (within reason)
     * two activation functions (Sigmoid and RELU)
     * a final activation function
     * the ability to set individual weights and biases if desired
     * A categorical crossentropy loss
     * Stochastic gradient descent
     * Batch learning
     
     */
    
    

    string file_name = "/Users/ericwolos/ML/NNscratch/mnist_dummy.csv";

    vector<pair<vector<float>*, int*>> train_data;

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
        cout << endl;
        train_data.push_back(pair(data_point, label));
    }
    }
    cout << train_data[0].first->at((6*28)+14) << " sized vector" << endl;

    cout << max::soft_arg_max({0,0,0,0,0,0,1}) << endl;

    vector <float> v1 = max::OHE(5,10);
    for (int j = 0; j < v1.size(); j++)
        {
            cout << v1[j] << ' ';
        }

    int input_vector_size = 784;
    int output_vector_size = 10;
    
    Model m(input_vector_size,"CCE", 0.9);
    m.add_layer(512, "Sigmoid");
    m.add_layer(256, "Sigmoid");
    m.add_layer(64, "Sigmoid");
    m.add_layer(16, "Sigmoid");
    m.add_layer(output_vector_size, "Sigmoid");
    
    m.get_layer_sizes();
    
//    vector <vector <float>> X = {{0,0} , {1,0}, {0,1}, {1,1}};
//    vector <vector <float>> y = {{1,0} , {0,1}, {0,1}, {1,0}};
    unsigned long data_size = train_data.size();
    unsigned long correct = 0;
    for (int epochs = 0; epochs < 50000; epochs++)
    {
        pair<vector<float>*, int*> data_point = train_data[epochs % data_size];
        vector <float> input = *data_point.first;
        
        int label = *data_point.second;
        vector <float> prediction = m.forward(input);
        
        if (max::soft_arg_max(prediction) == label) correct++;
        
        if (epochs % 5 == 0) cout << '=';
        
        if (epochs % 50 == 0)
        {
            cout << "finished epoch: " << epochs <<  endl;
            cout << "current loss: " << m.get_loss() <<  endl;
            cout << "current accuracy: " << float(correct / (epochs+1)) <<  endl;
        }


        m.backward(prediction,max::OHE(label, output_vector_size));

    }
//
//    vector <float> result1 = m.forward({0,1});
//    //cout << "model found: " << result1[1] << " which should be 1" << endl;
//    assert(result1[0] < result1[1]);
//    vector <float> result2 = m.forward({1,1});
//    assert(result2[0] > result2[1]);
//    //cout << "model found: " << result2[1] << " which should be 0" << endl;
//
    
    
//    
//    vector <float> p1 = {3};
//    vector <float> p2 = {4,5,6};
//
//    vector <float> p3 = {7,8,3,4,5,6,7};
//    vector <float> p4 = {9,10};
//    vector <float> p5 = {11,12};
//    matrix_dot v2({&p1}, {&p3});
//    vector <vector <float> * > v1 = v2.matrix_return();
//    cout << v1.size() << endl;
//    cout << v1[0]->size() << endl;
//
//    for (int i = 0; i < v1.size(); i++)
//        {
//            for (int j = 0; j < v1[i]->size(); j++)
//            {
//                cout << v1[i]->at(j) << ' ';
//            }
//            cout << endl;
//        }
    
    return 0;
}
