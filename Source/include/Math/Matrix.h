/**
 * @file
 *  ML_Matrix class declarations.
 *
 * @author George Kudrayvtsev
 * @version 1.0
 *
 * @addtogroup Math
 */
/// @{

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>

#include "Math/MathDef.h"

namespace math
{
    /**
     * A matrix class supporting virtually any type of operation.
     *
     * @todo Implement determinants for matrices greater than 3x3.
     */
    class ML_Matrix
    {
    public:
    	ML_Matrix(const u_int rows, const u_int columns);
    	ML_Matrix(const u_int rows, const u_int columns, const float** pdata);
    	ML_Matrix(const ML_Matrix& Copy);

        ML_Matrix& operator= (const ML_Matrix& Copy);
        ML_Matrix  operator+ (const ML_Matrix& Other) const;
        ML_Matrix  operator- (const ML_Matrix& Other) const;
        ML_Matrix  operator* (const ML_Matrix& Other) const;
        ML_Matrix  operator* (const float scalar)  const;
        bool    operator==(const ML_Matrix& Other) const;

        bool ChangeElement(const u_int row,
            const u_int column,
            const float new_value);
        
        int GetRowCount() const;
        int GetColumnCount() const;
        int GetSize() const;
        float Determinant() const;
        float GetElement(const u_int row, const u_int column) const;

        void Print() const;

    private:
        u_int m_rows, m_columns;
        std::vector< std::vector<float> > m_elements;
    };
}   // end of namespace math

#endif // MATRIX_H

/// @}