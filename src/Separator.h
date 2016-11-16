//
// Created by sumida on 2016/11/16.
//

#ifndef NMF_FOR_CPP_SEPARATOR_H
#define NMF_FOR_CPP_SEPARATOR_H

#include <math.h>
#include "Array2D.hpp"


/**
 * Function group of necessary for separation.
 */

/**
 * Calculate approximate matrix.
 * @param   dic     - dictionary matrix
 * @param   act     - activation matrix
 * @param   approx  - approximate matrix
 * @param   r       - row
 * @param   c       - column
 * @param   k       - k
 */
inline template<class T>
void _calc_approx(T **dic, T **act, T **approx, int r, int c, int k) {
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < r; j++) {
            T sum = 0.0;
            for (int c = 0; c < k; c++) {
                sum += dic[i][c] * act[c][j];
            }
            approx[i][j] = sum;
        }
    }

}

/**
 * Calculate the cost of base matrix and approximate matrix
 * @param   data    - base matrix
 * @param   approx  - approximate matrix
 * @param   column  - column
 * @param   row     - row
 * @return  - cost
 */
template<class T>
T _cost(T **data, T **approx, int column, int row) {
    T diff = 0.0;

    for (int i = 0; i < column; i++) {
        for (int j = 0; j < row; j++) {
            diff += sqrt(pow(data[i][j] - approx[i][j], 2));
        }
    }
    return diff;

}

/**
 * Template separation using EUC
 * @param   data        - base matrix
 * @param   dictionary  - dictionary matrix
 * @param   activation  - activation matrix
 * @param   n           - k
 * @param   c           - column
 * @param   r           - row
 * @param   iter        - iteration number of updates
 * @param   thr         - threshold
 */
template<class T>
void _separate_euc_with_template(T **data, T **dictionary, T **activation, int n, int c, int r, int iter, T thr) {

    Array2D<T> approx;
    int column = c;
    int row = r;
    int k = n;
    int counter = 0;
    approx._resize(column, row);

    while (counter < iter) {

        //近似行列の計算
        _calc_approx(dictionary, activation, approx._getPointer(), row, column, k);

        T cost = _cost(data, approx._getPointer(), column, row);
        if (cost < thr)
            break;

        //励起行列の更新
        for (int i = 0; i < row; i++) {
            for (int c = 0; c < k; c++) {
                T sum1 = 0.0;
                T sum2 = 0.0;

                for (int j = 0; j < column; j++) {
                    sum1 += data[j][i] * dictionary[j][c];
                    sum2 += approx[j][i] * dictionary[j][c];
                }
                if (sum2 > 0) {
                    activation[c][i] = activation[c][i] * (sum1 / sum2);
                }
                else
                    activation[c][i] = 0.0;
            }
        }


        counter += 1;
    }


}


/**
 * Template separation using KL
 * @param   data        - base matrix
 * @param   dictionary  - dictionary matrix
 * @param   activation  - activation matrix
 * @param   n           - k
 * @param   c           - column
 * @param   r           - row
 * @param   iter        - iteration number of updates
 * @param   thr         - threshold
 */
template<class T>
void _separate_kl_with_template(T **data, T **dictionary, T **activation, int n, int c, int r, int iter, T thr) {
    Array2D<T> approx;
    int column = c;
    int row = r;
    int k = n;
    int counter = 0;
    approx._resize(column, row);

    while (counter < iter) {
        //近似行列の計算
        _calc_approx(dictionary, activation, approx._getPointer(), row, column, k);

        T cost = _cost(data, approx._getPointer(), column, row);
        if (cost < thr)
            break;

        //励起行列の更新
        for (int i = 0; i < row; i++) {
            for (int c = 0; c < k; c++) {
                T sum1 = 0.0;
                T sum2 = 0.0;

                for (int j = 0; j < column; j++) {
                    if (approx[j][i] > 0.0) {
                        sum1 += data[j][i] / approx[j][i] * dictionary[j][c];
                    }
                    sum2 += dictionary[j][c];
                }
                if (sum2 > 0) {
                    activation[c][i] = activation[c][i] * (sum1 / sum2);
                }
                else
                    activation[c][i] = 0.0;
            }
        }


        counter += 1;
    }

}


/**
 * Template separation using IS
 * @param   data        - base matrix
 * @param   dictionary  - dictionary matrix
 * @param   activation  - activation matrix
 * @param   n           - k
 * @param   c           - column
 * @param   r           - row
 * @param   iter        - iteration number of updates
 * @param   thr         - threshold
 */
template<class T>
void _separate_is_with_template(T **data, T **dictionary, T **activation, int n, int c, int r, int iter, T thr) {
    Array2D<T> approx;
    int column = c;
    int row = r;
    int k = n;
    int counter = 0;
    approx._resize(column, row);

    while (counter < iter) {

        //近似行列の計算
        _calc_approx(dictionary, activation, approx._getPointer(), row, column, k);

        T cost = _cost(data, approx._getPointer(), column, row);
        if (cost < thr)
            break;
        //励起行列の更新
        for (int i = 0; i < row; i++) {
            for (int c = 0; c < k; c++) {
                T sum1 = 0.0;
                T sum2 = 0.0;

                for (int j = 0; j < column; j++) {
                    if (approx[j][i] > 0.0) {
                        sum1 += data[j][i] / approx[j][i] * (dictionary[j][c] / approx[j][i]);
                        sum2 += dictionary[j][c] / approx[j][i];
                    }
                }
                if (sum2 > 0) {
                    activation[c][i] = activation[c][i] * (sum1 / sum2);
                }
                else
                    activation[c][i] = 0.0;
            }
        }


        counter += 1;
    }
}

/**
 * Separation without templates using EUC
 * @param   data        - base matrix
 * @param   dictionary  - dictionary matrix
 * @param   activation  - activation matrix
 * @param   n           - k
 * @param   c           - column
 * @param   r           - row
 * @param   iter        - iteration number of updates
 * @param   thr         - threshold
 */
template<class T>
void _separate_euc_without_template(T **data, T **dictionary, T **activation, int n, int c, int r, int iter, T thr) {
    Array2D<T> approx;
    int column = c;
    int row = r;
    int k = n;
    int counter = 0;
    approx._resize(column, row);

    while (counter < iter) {

        //近似行列の計算
        _calc_approx(dictionary, activation, approx._getPointer(), row, column, k);

        T cost = _cost(data, approx._getPointer(), column, row);
        if (cost < thr)
            break;

        //励起行列の更新
        for (int i = 0; i < row; i++) {
            for (int c = 0; c < k; c++) {
                T sum1 = 0.0;
                T sum2 = 0.0;

                for (int j = 0; j < column; j++) {
                    sum1 += data[j][i] * dictionary[j][c];
                    sum2 += approx[j][i] * dictionary[j][c];
                }
                if (sum2 > 0) {
                    activation[c][i] = activation[c][i] * (sum1 / sum2);
                }
                else
                    activation[c][i] = 0.0;
            }
        }

        _calc_approx(dictionary, activation, approx._getPointer(), row, column, k);

        for (int i = 0; i < column; i++) {
            for (int c = 0; c < k; c++) {
                T sum1 = 0.0;
                T sum2 = 0.0;
                for (int j = 0; j < row; j++) {
                    sum1 += data[i][j] * activation[c][j];
                    sum2 += approx[i][j] * activation[c][j];
                }
                if (sum2 > 0) {
                    dictionary[i][c] = dictionary[i][c] * (sum1 / sum2);
                }
                else
                    dictionary[i][c] = 0.0;
            }
        }


        counter += 1;
    }

}

/**
 * Separation without templates using KL
 * @param   data        - base matrix
 * @param   dictionary  - dictionary matrix
 * @param   activation  - activation matrix
 * @param   n           - k
 * @param   c           - column
 * @param   r           - row
 * @param   iter        - iteration number of updates
 * @param   thr         - threshold
 */
template<class T>
void _separate_kl_without_template(T **data, T **dictionary, T **activation, int n, int c, int r, int iter, T thr) {

    Array2D<T> approx;
    int column = c;
    int row = r;
    int k = n;
    int counter = 0;
    approx._resize(column, row);

    while (counter < iter) {
        //近似行列の計算
        _calc_approx(dictionary, activation, approx._getPointer(), row, column, k);

        T cost = _cost(data, approx._getPointer(), column, row);
        if (cost < thr)
            break;

        //励起行列の更新
        for (int i = 0; i < row; i++) {
            for (int c = 0; c < k; c++) {
                T sum1 = 0.0;
                T sum2 = 0.0;

                for (int j = 0; j < column; j++) {
                    if (approx[j][i] > 0.0) {
                        sum1 += data[j][i] / approx[j][i] * dictionary[j][c];
                    }
                    sum2 += dictionary[j][c];
                }
                if (sum2 > 0) {
                    activation[c][i] = activation[c][i] * (sum1 / sum2);
                }
                else
                    activation[c][i] = 0.0;
            }
        }

        _calc_approx(dictionary, activation, approx._getPointer(), row, column, k);

        for (int i = 0; i < column; i++) {
            for (int c = 0; c < k; c++) {
                T sum1 = 0.0;
                T sum2 = 0.0;
                for (int j = 0; j < row; j++) {
                    if (approx[i][j] > 0.0) {
                        sum1 += data[i][j] / approx[i][j] * activation[c][j];
                    }
                    sum2 += activation[c][j];
                }
                if (sum2 > 0) {
                    dictionary[i][c] = dictionary[i][c] * (sum1 / sum2);
                }
                else
                    dictionary[i][c] = 0.0;
            }
        }


        counter += 1;
    }
}

/**
 * Separation without templates using IS
 * @param   data        - base matrix
 * @param   dictionary  - dictionary matrix
 * @param   activation  - activation matrix
 * @param   n           - k
 * @param   c           - column
 * @param   r           - row
 * @param   iter        - iteration number of updates
 * @param   thr         - threshold
 */
template<class T>
void _separate_is_without_template(T **data, T **dictionary, T **activation, int n, int c, int r, int iter, T thr) {

    Array2D<T> approx;
    int column = c;
    int row = r;
    int k = n;
    int counter = 0;
    approx._resize(column, row);

    while (counter < iter) {

        //近似行列の計算
        _calc_approx(dictionary, activation, approx._getPointer(), row, column, k);

        T cost = _cost(data, approx._getPointer(), column, row);
        if (cost < thr)
            break;
        //励起行列の更新
        for (int i = 0; i < row; i++) {
            for (int c = 0; c < k; c++) {
                T sum1 = 0.0;
                T sum2 = 0.0;

                for (int j = 0; j < column; j++) {
                    if (approx[j][i] > 0.0) {
                        sum1 += data[j][i] / approx[j][i] * (dictionary[j][c] / approx[j][i]);
                        sum2 += dictionary[j][c] / approx[j][i];
                    }
                }
                if (sum2 > 0) {
                    activation[c][i] = activation[c][i] * (sum1 / sum2);
                }
                else
                    activation[c][i] = 0.0;
            }
        }

        _calc_approx(dictionary, activation, approx._getPointer(), row, column, k);

        for (int i = 0; i < column; i++) {
            for (int c = 0; c < k; c++) {
                T sum1 = 0.0;
                T sum2 = 0.0;
                for (int j = 0; j < row; j++) {
                    if (approx[i][j] > 0.0) {
                        sum1 += data[i][j] / approx[i][j] * (activation[c][j] / approx[i][j]);
                        sum2 += activation[c][j] / approx[i][j];
                    }
                }
                if (sum2 > 0) {
                    dictionary[i][c] = dictionary[i][c] * (sum1 / sum2);
                }
                else
                    dictionary[i][c] = 0.0;
            }
        }


        counter += 1;
    }
}

#endif //NMF_FOR_CPP_SEPARATOR_H
