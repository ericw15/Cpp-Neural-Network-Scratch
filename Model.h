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
#include "Node.h"
#include "Layer.h"
#include "functions.h"
#include "matrix.h"

using namespace std;

//class Model {
//
//public:
//
//    Model() {
//        learning_rate = 0.001;
//    }
//
//    void add_layer(Layer new_layer) {
//
//        layers.insert(layers.end(), new_layer);
//
//    }
//
//    void init_layers(vector <float> input_layer) {
//
//        init_input_layer(input_layer);
//
//        for (int j = 1; j < layers.size(); j++){
//            vector <Node>* nodes = layers[j].get_nodes();
//            for (int i = 0; i < nodes->size(); i++){
//
//                unsigned long prev_layer_size = layers[j-1].get_nodes()->size();
//
//                init_weights(prev_layer_size, nodes);
//
//            }
//        }
//    }
//
//    // For debug
//    void print_layers() {
//
//        for (int j = 0; j < layers.size(); j++){
//            vector <Node>* nodes = layers[j].get_nodes();
//
//            cout << "Layer " << j << endl;
//            for (int i = 0; i < nodes->size(); i++){
//
//                cout << nodes->at(i).get_value() << " w:";
//                nodes->at(i).print_weights();
//                cout << "| ";
//
//            }
//            cout << endl;
//
//        }
//    }
//
//
//    void forward() {
//        // For each pair of layers:
//        for (int j = 1; j < layers.size(); j++){
//            vector <Node>* nodes = layers[j].get_nodes();
//            vector <Node>* prev_nodes = layers[j-1].get_nodes();
//
//            // For each set of nodes in that layer:
//            for (int i = 0; i < nodes->size(); i++){
//                vector <float> cur_weights = nodes->at(i).get_weights();
//
//                float weighted_sum = 0;
//                // For all the weights in the layer, compute weighted sums
//                for (int y = 0; y < cur_weights.size(); y++){
//
//                    weighted_sum += prev_nodes->at(y).get_value() * cur_weights[y];
//                }
//
//                // Add bias
//                weighted_sum += nodes->at(i).get_bias();
//
//                // Call activation functon
//                float output = layers[j].call_activation_function(weighted_sum);
//
//                // Update new nodes' value
//                nodes->at(i).update_value(output);
//
//            }
//
//        }
//    }
//
//
//
//    void backward(float loss_calculation, vector<float> label_key)
//    {
//        // Compute last layer weight adjustment
//        vector<Layer>::reverse_iterator beginning = layers.rend() - 1;
//
//        bool last_layer = true;
//        for (vector<Layer>::reverse_iterator it = layers.rbegin(); it != beginning; ++it) {
//            // TODO
//
//            /*
//             Alternative idea: have a vector that always stores the previous piece of information necessary. That way, accessing previous layers is not necessary and possibly loss does not need to be dealt with weirdly.
//             */
//            vector <Node>* nodes = it->get_nodes();
//
//            // something to do with loss
//            if (last_layer) {
//                // If RELU activation
//                function <float (float) > test = activation::Sigmoid;
//                if (it->return_activation_function() == test) {
//                    // For each node to be updated
//                    for (int i = 0; i < nodes->size(); i++)
//                    {
//                        float yhat = nodes->at(i).get_value();
//                        float y = label_key[i];
//
//                        // dLoss / dOutput * dOutput / dsum * dSum / dWeight
//
//                        // for each weight in that node
//                        vector<float> weights = nodes->at(i).get_weights();
//                        for (int j = 0; j < weights.size(); j++){
//                            float last_value = (it + 1)->get_nodes()->at(j).get_value();
//                            float pd = (-1 * y / yhat) * (yhat * (1 - yhat)) * (last_value);
//                            float new_weight = yhat + pd * learning_rate;
//
//                            weights[j] = new_weight;
//
//                        }
//
//
//                        //RELU
//                        //if (yhat <= 0) pd = 0; else pd = -1 * y / yhat * last_value;
//
//
//                    }
//                }
//
//                last_layer = false;
//            }
//            // something to do with just raw sums
//            else {
//                if (it->return_activation_function() == activation::Sigmoid) {
//                    for (int i = 0; i < nodes->size(); i++)
//                    {
//                        float yhat = nodes->at(i).get_value();
//                        float y = label_key[i];
//
//                        // dLoss / dOutput * dOutput / dsum * dSum / dWeight
//
//                        // for each weight in that node
//                        vector<float> weights = nodes->at(i).get_weights();
//                        for (int j = 0; j < weights.size(); j++){
//
//
//
////                            float last_value = (it + 1)->get_nodes()->at(j).get_value();
////                            float cur_weight = (it - 1)->get_nodes()->at(j).get_weights()[i];
////                            float pd =  * (yhat * (1 - yhat)) * (last_value);
////                            float new_weight = yhat + pd * learning_rate;
////
////                            weights[j] = new_weight;
//
//                        }
//
//                    }
//
//                }
//
//            }
//
//        }
//    }
//
//
//    vector <float> output() {
//        vector <float> my_out;
//        vector <Node>* final_layer = layers[layers.size() - 1].get_nodes();
//        for (int i = 0; i < final_layer->size(); i++)
//        {
//            my_out.insert(my_out.end(), final_layer->at(i).get_value());
//        }
//
//
//        return my_out;
//    }
//
//    void set_learning_rate(float new_lr) {
//        learning_rate = new_lr;
//    }
//
//
//private:
//
//    vector <Layer> layers;
//    std::default_random_engine generator;
//
//    float learning_rate;
//
//
//    void init_weights(unsigned long prev_layer_size, vector <Node>* nodes) {
//
//        float stddv = prev_layer_size == 0 ? 1: 1.0 / prev_layer_size;
//        std::normal_distribution<float> distribution(0.0,stddv);
//
//        for (int i = 0; i < nodes->size(); i++) {
//            float weight = distribution(generator);
//            nodes->at(i).add_weight(weight);
//        }
//
//    }
//
//    void init_input_layer(vector <float> input) {
//        if (layers.size() <= 0 or input.size() != layers[0].get_nodes()->size() ) {
//            cerr << "Error in inputing 1st layer into empty model" << endl;
//            assert(false);
//        }
//
//        else {
//            for (int i = 0; i < input.size(); i++ )
//            {
//                // Initalize every element of the first layer to the input vector
//                layers[0].get_nodes()->at(i).update_value(input[i]);
//            }
//
//        }
//    }
//
//};

class Model {
public:
    
    Model (int i_layer_size, string loss, float lr) {
        input_layer_size = i_layer_size;
        loss_type = loss;
        learning_rate = lr;
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
        
        // Delete all members of layer outputs
        for (int i = 0; i < layer_outputs.size(); i++)
        {
            delete layer_outputs[i];
        }

    }
    
    float get_loss() {
        return loss;
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
//        // Bias
        input_vector.push_back(1);
        
        vector <float>* successor = &input_vector;
        
        // Keep track of 1st layer output
        vector <float>* temp = new vector<float>(input_vector);
        layer_outputs.push_back(temp);
        
        // For each layer in the model
        for (int i = 0; i < model.size(); i++) {
//            cout << endl;
//            cout << i <<" pass"<< endl;
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
        
        loss = cur_loss;
        
        vector <vector < vector<float> *> *> deltas;

        // Iterate backwards to create deltas
        for (unsigned long i = model.size(); i > 0; i--) {

            // Treat last layer differently
            if (i == model.size()) {

                vector<float>* sd = new vector<float>;

                vector <float> error;

                for (int j = 0; j < label.size(); j++)
                {
//                     vanilla loss
                    error.push_back(model_prediction[j] - label[j]);

                    //error.push_back(-1*(label[j] + 0.1) / model_prediction[j]);
                }

                if (activations[i-1] == "Sigmoid") {
                    *sd = sigmoid_derivative(model_prediction);
                }
                else {
                    cerr << "Bad activation function" << endl;
                    assert(false);
                }

//                    // calculate loss times derivative of activation function.
//                    for (int x : *sd) {
//                        x *= loss;
//                    }

                for (int j = 0; j < error.size(); j++)
                {
                    (*sd)[j] *=  error[j];
                }

                vector <vector<float>*>* to_push = new vector<vector<float>*>;
                to_push->push_back(sd);
//                    cout << sd[0].size() << "bananas";
                deltas.push_back(to_push);

                }
            

            // Other layers:
            else {
                /*

                 there is a chance here that the sizes are being stored incorrectly. I should use this transpose insead, but it for some reason is not necessary.

                 */
                matrix_transpose transpose(*model[i]);
                vector <vector<float>*> matrix_transp = transpose.matrix_return();
                vector <vector<float>*> last_delta = *deltas.at(deltas.size() - 1);
                
//                    cout << "LD:" << last_delta.size() << " rows by " << last_delta[0]->size() << " columns";
//                    cout << "MT:" << matrix_transp.size() << " rows by " << matrix_transp[0]->size() << " columns";
                
                if (deltas.size() > 1)
                {
//                        cout << endl;
                    last_delta.at(0)->pop_back();
//                        cout << "fuck you eric "<< last_delta.at(0)->size() << endl;
                }

                matrix_dot m(last_delta,*model[i]);
                vector<float>* new_delta = new vector<float>(*m.matrix_return()[0]);

//                    cout << new_delta.size() << " rows by " << new_delta[0]->size() << " columns";
                //new_delta = multiply(new_delta, )

                vector<float> sd = sigmoid_derivative(*layer_outputs[i]);
                
                // Sigmoid derivative
                for (int j = 0; j < new_delta->size(); j++)
                {
                    new_delta->at(j) *= layer_outputs[i]->at(j);
                    new_delta->at(j) *= sd[j];
                    //float p = layer_outputs[i]->at(j);
                }

                vector <vector<float>*>* to_push = new vector<vector<float>*>;
                to_push->push_back(new_delta);
                deltas.push_back(to_push);

            }



        }
    
        deltas.at(deltas.size() - 1)->at(0)->pop_back();
        // End backwards iterate

        // Begin weight adjustment
        reverse(deltas.begin(), deltas.end());

        assert(deltas.size() == model.size());


        for (int i = 0; i < model.size(); i++)
        {
            // Transpose layer output
            
            matrix_transpose m( {layer_outputs[i]} );
            vector <vector<float>*> layer_output_transpose = m.matrix_return();

//                cout << "lot rows: " << layer_output_transpose.size() << endl;
//                cout << "lot cols: " << layer_output_transpose[0]->size() << endl;
//                cout << endl;
//                cout << "respective delta rows: " << deltas[i]->size() << endl;
//                cout << "respective delta columns: " << deltas[i]->at(0)->size() << endl;

            // dot transpose with delta vector
            matrix_dot m2(layer_output_transpose, *deltas[i]);
            vector <vector<float>*> partial_derivative_matrix = m2.matrix_return();

            matrix_transpose m3(partial_derivative_matrix);
            vector <vector<float>*> pdm_final = m3.matrix_return();

//                cout << "pdm rows: " << partial_derivative_matrix.size() << endl;
//                cout << "pdm cols: " << partial_derivative_matrix[0]->size() << endl;
//
//                cout << "model i rows: " << model[i]->size() << endl;
//                cout << "model i cols: " << model[i]->at(0)->size() << endl;
            
            
            
//            vector<vector<float>*>* temp = new vector<vector<float>*>(subtract(*model[i], multiply(pdm_final, learning_rate)));
//            delete model.at(i);
//            model.at(i) = temp;
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
    
    // Add every element of matrices A and B together
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
    
    
    vector <float> add(vector<float> input, float addition) {
        for (int i = 0; i < input.size(); i++)
        {
            input[i] += addition;
        }
        
        return input;
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
//            cout << "-1*y[j] is " << -1*y[j] << endl;
//            cout << "log(yhat[j]) is " << log(yhat[j]) << endl;
            loss += -1*y[j] * log(yhat[j]);
        }
            
        return loss;
    }
    
    
    
};

#endif /* Model_h */
