//
// Created by sumida on 2016/11/16.
//

#ifndef NMF_FOR_CPP_ARRAY2D_HPP
#define NMF_FOR_CPP_ARRAY2D_HPP

#include <iostream>
#include <random>

template<class T>
class Array2D {
private:
    /**
     * Row
     */
    int _row;

    /**
     * Column
     */
    int _column;

    /**
     * Array to store data
     */
    T **_value;

public:

    /**
     * Constructor
     */
    Array2D() {
        _value = NULL;
        _row = 0;
        _column = 0;
    }

    /**
     * Destructor
     */
    ~Array2D() {
        if (this->_value) {
            for (int i = 0; i < this->_column; i++) {
                if (this->_value[i]) delete[] this->_value[i];
            }
            delete[] this->_value;
        }
    }

    /**
     * Resizeing of matrix
     * T[col][row]
     * @param   col     - column
     * @param   row     - row
     */
    void _resize(int col, int row) {
        this->_row = row;
        this->_column = col;

        if (this->_value)
            delete[] this->_value;

        this->_value = new T *[this->_column];
        for (int i = 0; i < this->_column; i++)
            this->_value[i] = new T[this->_row + i];
    }

    /**
     * Function to set data in the column direction.
     * @param   row     - row index
     * @param   vector  - set data
     */
    void _set_Row_Vector(int row, T *vector) {
        if (row >= this->_row || row < 0) {
            std::cout << "row = " << row << std::endl;
            std::cout << "0 <= row < " << this->_row << std::endl;
            return;
        }

        for (int i = 0; i < this->_column; i++)
            this->_value[i][row] = vector[i];
    }

    /**
     * Function to set data in the row direction.
     * @param   col     - column index
     * @param   vector  - set data
     */
    void _set_Col_Vector(int col, T *vector) {
        if (col >= this->_column || col < 0) {
            std::cout << "row = " << col << std::endl;
            std::cout << "0 <= row < " << this->_column << std::endl;
            return;
        }

        for (int i = 0; i < this->_row; i++)
            this->_value[col][i] = vector[i];
    }

    /**
     * Initialize the matrix with the numerical value specified in the argument
     * @param   value   - specific value
     */
    void _init_value(T value) {
        for (int i = 0; i < this->_column; i++) {
            for (int j = 0; j < this->_row; j++) {
                this->_value[i][j] = value;
            }
        }
    }

    /**
     * A function to initialize a random number in a specified range of a activation matrix
     * @param   min     - Minimum value of random number range
     * @param   max     - Maximum value of random number range
     */
    void _init_random(T min, T max) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> rnd(min, max);

        for (int i = 0; i < this->_column; i++) {
            for (int j = 0; j < this->_row; j++) {
                this->_value[i][j] = rnd(mt);
            }
        }

    }

    /**
     * Getter of data pointer
     * @return  value
     */
    inline T** _getPointer()
    {
        return this->_value;
    }


    /**
     * @param   i   index
     * @return _value corresponding to the specified i
     */
    inline T *operator[](unsigned int i) {
        return (this->_value[i]);
    }

    /**
     * @param   i   index
     * @return _value corresponding to the specified i
     */
    inline const T *operator[](unsigned int i) const {
        return (this->_value[i]);
    }

    /**
     * Getter of Row
     * @return  row
     */
    inline int _get_Row() {
        return this->_row;
    }

    /**
     * Getter of Column
     * @return  column
     */
    inline int _get_Column() {
        return this->_column;
    }

};

#endif //NMF_FOR_CPP_ARRAY2D_HPP
