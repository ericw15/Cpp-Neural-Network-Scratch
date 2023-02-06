//
//  Model.h
//  NN
//
//  Created by Eric Wolos on 1/18/23.
//

#ifndef Model_h
#define Model_h

#include <vector>
#include <random>
#include "functions.h"
#include "matrix.h"

using namespace std;

class Model {
public:
    
    Model (int i_layer_size, string loss_type_, float lr) {
        input_layer_size = i_layer_size;
        loss_type = loss_type_;
        learning_rate = lr;
        iterations = 0;
        loss = 0;
        model = {};
    }
    
    
    ~Model ()
    {
        // Delete every node in each layer, then delete that layer
        for (int i = 0; i < model.size(); i++) {
            vector< vector <float>*>* cur_layer = model.at(i);
            for (int j = 0; j < cur_layer->size(); j++)
            {
                delete cur_layer->at(j);
            }
            delete cur_layer;
        }

    }
    
    // Necessary for prevention of memory leak if only forward passes are being called.
    void clear_layer_outputs ()
    {
        // Delete all members of layer outputs
        for (int i = 0; i < layer_outputs.size(); i++)
        {
            delete layer_outputs[i];
        }
    }
    
    float get_loss() {
        return loss / iterations;
    }
    
    void add_layer(int layer_size, string activation){
        
        activations.push_back(activation);
        
        vector <vector <float>*>* new_layer = new vector <vector <float>*>;
        
        unsigned long prev_layer_size;
        
        if (model.size() == 0) {
            prev_layer_size = input_layer_size;
        }
        else{
            prev_layer_size = model.at(model.size() - 1)->size();
        }
        
        
        for (int i = 0; i < layer_size; i++)
        {
            vector <float>* new_node = new vector <float>;
            // Generate random weights for new node
            for (int j = 0; j < prev_layer_size; j++)
            {
                new_node->push_back(init_random_weight(prev_layer_size));
            }
            
            // Add bias
            new_node->push_back(1);
            
            // Insert new layer
            new_layer->push_back(new_node);
        }
        
        model.push_back(new_layer);
        
        
        
    }
    
    
    
    vector <float> forward (vector<float> input_vector) {
        
        // Init layer outputs
        layer_outputs = {};
        
        // Bias
        input_vector.push_back(1);
        
        vector <float>* successor = &input_vector;
        
        // Keep track of 1st layer output
        vector <float>* temp = new vector<float>(input_vector);
        layer_outputs.push_back(temp);
        
        // For each layer in the model
        for (int i = 0; i < model.size(); i++) {

            // For each node in the layer
            vector<vector<float>*>* cur_layer = model.at(i);
            
            *successor = dot(*cur_layer, *successor);
            *successor = activate(*successor, activations[i]);
            
            
            if (i != model.size() - 1)
            {
                successor->push_back(1);
            }
            
            // Keep track of layer outputs
            vector <float>* temp = new vector<float>(*successor);
    
            layer_outputs.push_back(temp);
        }
        
        return *successor;
    }
    
    
    void backward(vector <float> model_prediction, vector <float> label) {
            
        float cur_loss = categorical_crossentropy(label, model_prediction);
        
        loss += cur_loss;
        iterations++;
        
        vector <vector < vector<float> *> *> deltas;

        // Iterate backwards to create deltas
        for (unsigned long i = model.size(); i > 0; i--) {

            // Treat last layer differently
            if (i == model.size()) {

                vector<float>* sd = new vector<float>;

                vector <float> error;

                for (int j = 0; j < label.size(); j++)
                {
                    // error loss
                    error.push_back(model_prediction[j] - label[j]);
                    
                    
                }

                if (activations[i-1] == "Sigmoid") {
                    *sd = sigmoid_derivative(model_prediction);
                }
                else {
                    cerr << "Bad activation function" << endl;
                    assert(false);
                }


                for (int j = 0; j < error.size(); j++)
                {
                    (*sd)[j] *=  error[j];
                }

                vector <vector<float>*>* to_push = new vector<vector<float>*>;
                to_push->push_back(sd);

                deltas.push_back(to_push);

                }
            

            // Other layers:
            else {
   
                matrix_transpose transpose(*model[i]);
                vector <vector<float>*> matrix_transp = transpose.matrix_return();
                vector <vector<float>*> last_delta = *deltas.at(deltas.size() - 1);
                
                
                if (deltas.size() > 1)
                {
                    last_delta.at(0)->pop_back();
                }

                matrix_dot m(last_delta,*model[i]);
                vector<float>* new_delta = new vector<float>(*m.matrix_return()[0]);

                vector<float> sd = sigmoid_derivative(*layer_outputs[i]);
                
                // Sigmoid derivative
                for (int j = 0; j < new_delta->size(); j++)
                {
                    new_delta->at(j) *= layer_outputs[i]->at(j);
                    new_delta->at(j) *= sd[j];
                }

                vector <vector<float>*>* to_push = new vector<vector<float>*>;
                to_push->push_back(new_delta);
                deltas.push_back(to_push);

            }



        }
    
        deltas.at(deltas.size() - 1)->at(0)->pop_back();
        // End backwards iterate

        // Since deltas are added iteratively before, they must be reversed to reflect the order of the layers
        reverse(deltas.begin(), deltas.end());

        assert(deltas.size() == model.size());


        for (int i = 0; i < model.size(); i++)
        {
            // Transpose layer output
            
            matrix_transpose m( {layer_outputs[i]} );
            vector <vector<float>*> layer_output_transpose = m.matrix_return();
            
            // Dot this layer transpose with the delta corresponding to this later
            matrix_dot m2(layer_output_transpose, *deltas[i]);
            vector <vector<float>*> partial_derivative_matrix = m2.matrix_return();

            // Tranpose again
            matrix_transpose m3(partial_derivative_matrix);
            vector <vector<float>*> pdm_final = m3.matrix_return();

            // Multiply by learning rate and subtract to get adjusted weights
            *model.at(i) = subtract(*model[i], multiply(pdm_final, learning_rate));
            
        }



        for (int i = 0; i < deltas.size(); i++)
        {
            for (int j = 0; j < deltas[i]->size(); j++)
            {
                delete deltas[i]->at(j);
            }
            delete deltas[i];
        }
        
        for (int i = 0; i < layer_outputs.size(); i++)
        {
            delete layer_outputs[i];
        }


    }
        
        
    void set_lr (float new_lr)
    {
        learning_rate = new_lr;
    }
    
    float get_lr ()
    {
        return learning_rate;
    }
    
    vector <vector < vector<float>*>*> get_model() {
        return model;
    }
    
    
    void get_layer_sizes ()
    {
        cout << endl;
        cout << "Getting layer sizes:" << endl;
        cout << endl;
        for (int i = 0; i < model.size(); i++)
        {
            cout << "Layer " << i << ":" << endl;
            cout << model.at(i)->size() << " rows, " << model.at(i)->at(0)->size() << " columns." << endl;
        }
        cout << endl;
    }
    
    
private:
    
    int input_layer_size;
    vector <vector < vector<float> *> *> model;
    vector <string> activations;
    vector <int> biases;
    string loss_type;
    vector <vector <float>*> layer_outputs;
    float learning_rate;
    
    float loss;
    float iterations;
    
    
    std::default_random_engine generator;
    
    float init_random_weight(unsigned long prev_layer_size) {
        float stddv = prev_layer_size == 0 ? 1: 1.0 / prev_layer_size;
        std::normal_distribution<float> distribution(0.0,stddv);
        return distribution(generator);
    }
    
    vector <float> dot (vector<vector<float>*> A, vector<float> B) {
        vector <float> output;
        
        if (A.size() <= 0) {
            cerr << "Empty matrix" << endl;
            assert(false);
        }
        if (A[0]->size() != B.size()) {
            cerr << "Wrong vector sizing" << endl;
            cerr << "A's columns: " << A[0]->size() << endl;
            cerr << "B's rows: " << B.size() << endl;
            assert(false);
        }
        
        for (int i = 0; i < A.size(); i++) {
            float sum = 0;
            vector <float>* cur_row = A[i];
            for (int j = 0; j< cur_row->size(); j++)
            {
                sum += cur_row->at(j) * B[j];
            }
            output.insert(output.end(), sum);
        }
        
        return output;
    }
    
    
    // Multiply every element of a matrix by a factor
    vector <vector <float>*> multiply (vector<vector<float>*> input, float factor)
    {
        for (int i = 0; i < input.size(); i++)
        {
            for (int j = 0; j < input[i]->size(); j++)
            {
                input[i]->at(j) *= factor;
            }
        }
        
        return input;
    }
    
    // Subtract every element of matrices A and B together
    vector <vector <float>*> subtract (vector<vector<float>*> A, vector<vector<float>*> B)
    {
        assert(A.size() == B.size() && ( (A[0]->size() == B[0]->size()) || (B.size() == 0 && A.size() == 0) ) );
        
        
        for (int i = 0; i < A.size(); i++)
        {
            for (int j = 0; j < A[i]->size(); j++)
            {
                A[i]->at(j) -= B[i]->at(j);
            }
        }
        
        return A;
    }
    
    vector <float> activate(vector<float> input, string activation) {
        if (activation == "Sigmoid") {
            for (int i = 0; i < input.size(); i++)
            {
                input[i] = 1 / (1 + exp(-1*input[i]));
            }
        }
        
        else {
            cerr << "bad activation function" << endl;
            assert(false);
        }
        
        return input;
    }
    
    vector <float> sigmoid_derivative(vector<float> input) {
        for (int i = 0; i < input.size(); i++)
        {
            input[i] = input[i] * (1 - input[i]);
        }
        
        return input;
    }
    
    float categorical_crossentropy(vector <float> y, vector <float> yhat) {
        if (y.size() != yhat.size()) {
            cerr << "wrong vector sizes for CCE loss" << endl;
            assert(false);
        }
        
        float loss = 0;
        // Across C classes:
        for (int j = 0; j < y.size(); j++)
        {
            if (y[j] == 0) {
                continue;
            }

            loss += -1*y[j] * log(yhat[j]);
        }
            
        return loss;
    }
    
    
    
};

#endif /* Model_h */
