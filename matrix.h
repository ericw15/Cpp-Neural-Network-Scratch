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
    
    ~matrix_dot ()
    {
        for (int i = 0; i < matrix.size(); i++)
        {
            delete matrix[i];
        }
    
    }
    
    matrix_dot (vector<vector<float>*> mat1, vector<vector<float>*> mat2)
    {
        
        int x, i, j;
        unsigned long m1 = mat1.size();
        unsigned long m2 = mat1[0]->size();
        unsigned long n1 = mat2.size();
        unsigned long n2 = mat2[0]->size();
        
        // make sure matrix sizes are correct
        assert(m2 == n1);
        
        vector<vector<float>*> res;
        
        for (i = 0; i < m1; i++)
        {
            vector<float>* new_row = new vector<float>(n2,0);
            res.push_back(new_row);
        }
            
        
        for (i = 0; i < m1; i++) {
            for (j = 0; j < n2; j++) {
                res.at(i)->at(j) = 0;
                for (x = 0; x < m2; x++) {
                    
                    *((*(res.begin() + i))->begin() + j) += *((*(mat1.begin() + i))->begin() + x) * (*((*(mat2.begin() + x))->begin() + j));
                                         
                }
            }
        }
        
        matrix = res;
        
    }

};


#endif /* matrix_h */
