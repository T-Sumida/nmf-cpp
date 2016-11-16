//
// Created by sumida on 2016/11/16.
//

#ifndef NMF_FOR_CPP_NMF_HPP
#define NMF_FOR_CPP_NMF_HPP

#include "Separator.h"


/**
 * Class to undertake work other than separation at NMF
 */

template<class T>
class NMF {
private:

    /**
     * Factorization number
     */
    int _k;

    /**
     * Base matrix's Row
     */
    int _row;

    /**
     * Base matrix's Column
     */
    int _column;

    /**
     * Dictionary matrix
     */
    Array2D<T> _dictionary;

    /**
     * Activation matrix
     */
    Array2D<T> _activation;

    /**
     * Base matrix
     */
    Array2D<T> _base_matrix;


public:
    /**
     * Enumeration type for algorithm selection
     */
    enum {EUC,KL,IS};

    /*
     * Constructor
     */
    NMF() {
        this->_k = 0;
        this->_row = 0;
        this->_column = 0;
    }

    /*
     * Destructor
     */
    ~NMF() {

    }

    /*
     * Initialization method
     * Initialize dictionary matrix and activation matrix as a random number between [0,1]
     * @param   column  - column
     * @param   row     - row
     * @param   k       - factorization number
     */
    void _init(int column, int row, int k) {
        this->_k = k;
        this->_row = row;
        this->_column = column;

        this->_base_matrix._resize(this->_column,this->_row);
        this->_dictionary._resize(this->_column,this->_k);
        this->_activation._resize(this->_k,this->_row);
        this->_base_matrix._init_value(1.0);
        this->_dictionary._init_random(0.0,1.0);
        this->_activation._init_random(0.0,1.0);

    }

    /**
     * Setter of decomposed symmetric matrix
     * @param   data    - Base matrix
     */
    void _set_baseMatrix(T** data)
    {
        for(int i = 0;i < this->_column;i++)
        {
            for(int j = 0;j < this->_row;j++)
            {
                this->_base_matrix[i][j] = data[i][j];
            }
        }
    }

    /**
     * A function to set a template in a dictionary matrix
     * @param   index           - Template ID (0 <= index < _k)
     * @param   template_data   - Template data
     */
    void _setDictionary(int index,T* template_data)
    {
        this->_dictionary._set_Row_Vector(index,template_data);

    }

    /**
     * A function to initialize a random number in a specified range of a dictionary matrix
     * @param   min     - Minimum value of random number range
     * @param   max     - Maximum value of random number range
     */
    void _init_dictionary_in_random(T min,T max)
    {
        this->_dictionary._init_random(min,max);
    }

    /**
     * A function to initialize a random number in a specified range of a activation matrix
     * @param   min     - Minimum value of random number range
     * @param   max     - Maximum value of random number range
     */
    void _init_activation_in_random(T min,T max)
    {
        this->_activation._init_random(min,max);
    }

    /**
     * A function that fills a dictionary matrix with a specified value
     * @param   value   - specified value
     */
    void _init_dictionary_in_constant(T value)
    {
        this->_dictionary._init_value(value);
    }

    /**
     * A function that fills a activation matrix with a specified value
     * @param   value   - specified value
     */
    void _init_activation_in_constant(T value)
    {
        this->_activation._init_value(value);
    }

    /**
     * Function to perform factorization with templates
     * (Dictionary matrix is not eligible for update)
     * @param   iter    - Iteration number of updates
     * @param   algf    - Algorithm variable from enumeration type
     * @param   thr     - Threshold of error between base matrix and approximate matrix
     */
    void _separate_with_template(int iter,int algf,T thr)
    {
        if(iter > INT_MAX)
            iter = INT_MAX;
        else if(iter <= 0)
            iter = 1;


        switch (algf)
        {
            case EUC:
                _separate_euc_with_template(this->_base_matrix._getPointer(),this->_dictionary._getPointer(),this->_activation._getPointer(),this->_k,this->_column,this->_row,iter,thr);
                break;
            case KL:
                _separate_kl_with_template(this->_base_matrix._getPointer(),this->_dictionary._getPointer(),this->_activation._getPointer(),this->_k,this->_column,this->_row,iter,thr);
                break;
            case IS:
                _separate_is_with_template(this->_base_matrix._getPointer(),this->_dictionary._getPointer(),this->_activation._getPointer(),this->_k,this->_column,this->_row,iter,thr);
                break;
        }

    }

    /**
     * Function to perform factorization without templates
     * (The dictionary matrix is also updated)
     * @param   iter    - Iteration number of updates
     * @param   algf    - Algorithm variable from enumeration type
     * @param   thr     - Threshold of error between base matrix and approximate matrix
     */
    void _separate_without_template(int iter,int algf,T thr)
    {
        if(iter > INT_MAX)
            iter = INT_MAX;
        else if(iter <= 0)
            iter = 1;


        switch (algf)
        {
            case EUC:
                _separate_euc_without_template(this->_base_matrix._getPointer(),this->_dictionary._getPointer(),this->_activation._getPointer(),this->_k,this->_column,this->_row,iter,thr);
                break;
            case KL:
                _separate_kl_without_template(this->_base_matrix._getPointer(),this->_dictionary._getPointer(),this->_activation._getPointer(),this->_k,this->_column,this->_row,iter,thr);
                break;
            case IS:
                _separate_is_without_template(this->_base_matrix._getPointer(),this->_dictionary._getPointer(),this->_activation._getPointer(),this->_k,this->_column,this->_row,iter,thr);
                break;
        }

    }

    /**
     * A function that calculates an approximate matrix from the current dictionary matrix and excitation matrix and outputs it as a standard
     */
    void _print_approx()
    {
        std::cout << "------------- Approx Matrix ---------------" << std::endl;
        for(int i = 0;i < this->_column;i++)
        {
            for(int j = 0;j < this->_row;j++)
            {
                T sum = 0.0;
                for (int c = 0; c < this->_k; c++) {
                    sum += this->_dictionary[i][c] * this->_activation[c][j];
                }
                std::cout << sum <<",";
            }
            std::cout << "" << std::endl;
        }
        std::cout << "------------- Approx Matrix ---------------" << std::endl;
    }

    /**
     * Getter in the contents of a dictionary matrix
     * @return  dictionary matrix
     */
    T** _getDic()
    {
        return this->_dictionary._getPointer();
    }

    /**
     * Getter in the contents of a activation matrix
     * @return  activation matrix
     */
    T** _getAct()
    {
        return this->_activation._getPointer();
    }
};


#endif //NMF_FOR_CPP_NMF_HPP
