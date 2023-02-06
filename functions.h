//
//  activations.h
//  NN
//
//  Created by Eric Wolos on 1/18/23.
//

#ifndef activations_h
#define activations_h
using namespace std;
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


#endif /* activations_h */
