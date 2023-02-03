//
//  matrix.h
//  NN
//
//  Created by Eric Wolos on 1/30/23.
//

#ifndef matrix_h
#define matrix_h

class matrix_ {
    
public:
    
vector<vector <float>*> matrix;
    
~matrix_ ()
    {
    // destruct

}
    
vector <vector <float>*> matrix_return() {
    return matrix;
}
    
vector <float> dot (vector<vector<float>*> A, vector<float> B) {
    vector <float> output;
    
    if (A.size() <= 0) {
        cerr << "Empty matrix" << endl;
        assert(false);
    }
    
    if (A[0]->size() != B.size()) {
        cerr << "Wrong vector sizing" << endl;
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
    
    
private:
    
};

class matrix_transpose: public matrix_ {

public:
    
    ~matrix_transpose() {
    for (int i = 0; i < matrix.size(); i++)
    {
        delete matrix[i];
    }
        
    }

    matrix_transpose (vector<vector<float>*> A) {
        vector <vector <float>*> B;
        
        if (A.size() == 0) {
            cerr << "invalid matrix size" << endl;
            assert(false);
        }
        
        for (int j = 0; j < A[0]->size(); j++)
        {
            vector <float>* new_row = new vector <float>;
            
            for (int i = 0; i < A.size(); i++) {
                new_row->push_back(A[i]->at(j));
            }
            
            B.push_back(new_row);
        }
        
         matrix = B;
        
    }
    

};


class matrix_dot : public matrix_ {
    
public:
    
    ~matrix_dot () {
        
        for (int i = 0; i < B.size(); i++)
        {
            delete B[i];
        }
        
        
        
    }
    
    matrix_dot (vector<vector<float>*> A, vector<vector<float>*> B)
    {
        for (int i = 0; i < B[0]->size(); i++)
        {
            vector <float> column;
            for (int j = 0; j < B.size(); j++) {
                column.push_back(B[j]->at(i));
            }
            
            vector <float> ans = dot(A, column);
            vector <float> * p1 = new vector <float>{dot(A, column)};
            
            matrix.push_back(p1);

        }
        
        matrix = matrix_transpose_helper(matrix);
    }
    
    
private:
    
    // B is for the matrix_transpose_helper function
    vector <vector <float>*> B;
    vector <vector<float>*> matrix_transpose_helper (vector<vector<float>*> A) {
        
        if (A.size() == 0) {
            cerr << "invalid matrix size" << endl;
            assert(false);
        }
        
        for (int j = 0; j < A[0]->size(); j++)
        {
            vector <float>* new_row = new vector <float>;
            
            for (int i = 0; i < A.size(); i++) {
                new_row->push_back(A[i]->at(j));
            }
            
            B.push_back(new_row);
        }
        
         return B;
        
    }

};


#endif /* matrix_h */
