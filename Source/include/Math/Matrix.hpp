/**
 * @file
 *  CMatrix class declarations.
 *
 * @author      George Kudrayvtsev (switch1440)
 * @version     1.0
 * @copyright   Apache License v2.0
 *  Licensed under the Apache License, Version 2.0 (the "License").\n
 *  You may not use this file except in compliance with the License.\n
 *  You may obtain a copy of the License at:
 *  http://www.apache.org/licenses/LICENSE-2.0 \n
 *  Unless required by applicable law or agreed to in writing, software\n
 *  distributed under the License is distributed on an "AS IS" BASIS,\n
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
 *  See the License for the specific language governing permissions and\n
 *  limitations under the License.
 *
 * @addtogroup Math
 **/
/// @{

#ifndef MATH__MATRIX_HPP
#define MATH__MATRIX_HPP

#include <iostream>
#include <vector>

#include "Math/MathDef.hpp"

namespace math
{
    /**
     * A matrix class supporting virtually any type of operation.
     * @todo Implement determinants for matrices greater than 3x3.
     **/
    class CMatrix
    {
    public:
    	CMatrix(const u_int rows, const u_int columns);
    	CMatrix(const u_int rows, const u_int columns, const float* ppdata[]);
    	CMatrix(const CMatrix& Copy);

        CMatrix& operator= (const CMatrix& Copy);
        CMatrix  operator+ (const CMatrix& Other) const;
        CMatrix  operator- (const CMatrix& Other) const;
        CMatrix  operator* (const CMatrix& Other) const;
        CMatrix  operator* (const float scalar)   const;
        bool     operator==(const CMatrix& Other) const;

        bool ChangeElement(const u_int row,
            const u_int column,
            const float new_value);
        
        int GetRowCount()   const;
        int GetColumnCount()const;
        int GetSize()       const;
        float Determinant() const;
        float GetElement(const u_int row, const u_int column) const;

        void Print() const;

    private:
        u_int m_rows, m_columns;
        std::vector< std::vector<float> > m_elements;
    };
}

#endif // MATH__MATRIX_HPP

/// @}