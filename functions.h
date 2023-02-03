//
//  activations.h
//  NN
//
//  Created by Eric Wolos on 1/18/23.
//

#ifndef activations_h
#define activations_h

class max {
public:
    static long soft_arg_max (vector <float> input_vector)
    {
        long it = max_element(std::begin(input_vector), std::end(input_vector)) - std::begin(input_vector);
        return it;
    }
    
    // One hot encode
    static vector <float> OHE (long input_num, long desired_len)
    {
        vector <float> ans;
        int i = 0;
        for (; i < input_num; i++)
        {
            ans.push_back(0);
        }
        ans.push_back(1);
        for (int j = i+1; j < desired_len; j++)
        {
            ans.push_back(0);
        }
        return ans;
    }
};

class activation {
    
public:
    
    static float RELU (float input){
        if (input >= 0) {
            return input;
        }
        
        return 0;
    }
    
    static float Sigmoid (float input) {
        return 1 / (1 + exp(-1*input));
    }
    
    static vector <float> Softmax(vector <float> input) {
        vector <float> output;
        
        float denominator = 0;
        for (int i = 0; i < input.size(); i++)
        {
            denominator += exp(input[i]);
        }
        
        for (int i = 0; i < input.size(); i++)
        {
            output.insert(output.end(), exp(input[i]) / denominator);
        }
        
        return output;
    }
    
    
};

class loss {
    
public:
    
    static float categorical_crossentropy (vector <vector <float>> prediction_batch, vector <vector <float>> label_batch)
    {
        if (prediction_batch.size() != label_batch.size()){
            cerr << "Wrong batch sizes" << endl;
            assert(false);
        }
        
        float total_loss = 0;
        // Across N samples:
        for (int i = 0; i < prediction_batch.size(); i ++) {
            
            // Across C classes:
            vector <float> prediction = prediction_batch[i];
            vector <float> label = label_batch[i];
            
            float loss = 0;
            for (int j = 0; j < label.size(); j++)
            {
                if (prediction[j] == 0) {
                    // Treat log(0) as 0 instead of infinite / undefined.
                    continue;
                }
                loss += -1*label[j] * log(prediction[j]);
            }
            total_loss += loss;
        }
        
        return total_loss;
    }
    
};


#endif /* activations_h */
