#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

namespace MathHelper
{
    template<unsigned int rows, unsigned int columns>
    class Matrix
    {
    public:
        Matrix();
        Matrix(const float data[rows][columns]);
        Matrix(const Matrix& Copy);

        Matrix& operator= (const Matrix& Copy);
        Matrix  operator+ (const Matrix& Other) const;
        Matrix  operator- (const Matrix& Other) const;
        Matrix  operator* (const float scalar)  const;
        bool    operator==(const Matrix& Other) const;
        
        bool ChangeElement(unsigned int row,    // Change element at [row][column] to new value.
            unsigned int column,                // Returns false if [row][column] doesn't exist
            float new_value);

        int GetRowCount() const;
        int GetColumnCount() const;
        int GetSize() const;
        float GetDeterminant() const;

        float GetElement(unsigned int row, unsigned int column) const;  // Returns element at [row][column]. -1.0f if out of bounds.

        void Print() const; // Print to stdout;

    private:
        unsigned int matrix_size, row_count, column_count;
        std::vector<std::vector<float> > Elements;
    };

    typedef Matrix<3, 3> Matrix3;
    typedef Matrix<2, 2> Matrix2;

    /* Fuck this template shit. 
     * Never freaking again. Couldn't they have made some way that you could include
     * the template source in a .cpp file? COULDN'T THEY?
     */
    template<unsigned int rows, unsigned int columns>
    Matrix<rows, columns>::Matrix() : matrix_size(rows*columns), row_count(rows), column_count(columns)
    {
        this->Elements.resize(rows);
    
        for(int i = 0; i < rows; i++)
            this->Elements[i].resize(columns);
    }

    template<unsigned int rows, unsigned int columns>
    Matrix<rows, columns>::Matrix(const float data[rows][columns]) : matrix_size(rows*columns), row_count(rows), column_count(columns)
    {
        this->Elements.resize(rows);
    
        for(size_t i = 0; i < rows; i++)
            this->Elements[i].resize(columns);

        for(size_t i = 0; i < rows; i++)
            for(size_t j = 0; j < columns; j++)
                this->Elements[i][j] = data[i][j];
    }

    template<unsigned int rows, unsigned int columns>
    Matrix<rows, columns>::Matrix(const Matrix<rows, columns>& Copy) : row_count(Copy.GetRowCount()),
        column_count(Copy.GetColumnCount()), matrix_size(row_count * column_count)
    {
        this->Elements.resize(this->row_count);
        for(size_t i = 0; i < this->row_count; i++)
        {
            this->Elements[i].resize(this->column_count);
        }

        for(size_t i = 0; i < this->row_count; i++)
        {
            for(size_t j = 0; j < this->column_count; j++)
            {
                this->Elements[i][j] = Copy.GetElement(i, j);
            }
        }
    }

    template<unsigned int rows, unsigned int columns>
    Matrix<rows, columns>& Matrix<rows, columns>::operator= (const Matrix<rows, columns>& Copy)
    {
        this->row_count = Copy.GetRowCount();
        this->column_count = Copy.GetColumnCount();
        this->matrix_size = this->row_count * this->column_count;

        this->Elements.resize(this->row_count);
        for(size_t i = 0; i < this->row_count; i++)
        {
            this->Elements[i].resize(this->column_count);
        }

        for(size_t i = 0; i < this->row_count; i++)
        {
            for(size_t j = 0; j < this->column_count; j++)
            {
                this->Elements[i][j] = Copy.GetElement(i, j);
            }
        }

        return *this;
    }

    template<unsigned int rows, unsigned int columns>
    bool Matrix<rows, columns>::operator== (const Matrix<rows, columns>& Other) const
    {
        for(size_t i = 0; i < this->row_count; i++)
        {
            for(size_t j = 0; j < this->column_count; j++)
            {
                if(Other.GetElement(i, j) != this->Elements[i][j])
                    return false;
            }
        }

        return true;
    }

    template<unsigned int rows, unsigned int columns>
    Matrix<rows, columns> Matrix<rows, columns>::operator+ (const Matrix<rows, columns>& Other) const
    {
        Matrix<rows, columns> Answer;
        for(size_t i = 0; i < this->row_count; i++)
        {
            for(size_t j = 0; j < this->column_count; j++)
            {
                Answer.ChangeElement(i, j, this->Elements[i][j] + Other.GetElement(i, j));
            }
        }

        return Answer;
    }

    template<unsigned int rows, unsigned int columns>
    Matrix<rows, columns> Matrix<rows, columns>::operator- (const Matrix<rows, columns>& Other) const
    {
        Matrix<rows, columns> Answer;
        for(size_t i = 0; i < this->row_count; i++)
        {
            for(size_t j = 0; j < this->column_count; j++)
            {
                Answer.ChangeElement(i, j, this->Elements[i][j] - Other.GetElement(i, j));
            }
        }

        return Answer;
    }

    template<unsigned int rows, unsigned int columns>
    Matrix<rows, columns> Matrix<rows, columns>::operator* (const float scalar) const
    {
        Matrix<rows, columns> Answer;
        for(size_t i = 0; i < this->row_count; i++)
        {
            for(size_t j = 0; j < this->column_count; j++)
            {
                Answer.ChangeElement(i, j, this->Elements[i][j] * scalar);
            }
        }

        return Answer;
    }

    template<unsigned int rows, unsigned int columns>
    bool Matrix<rows, columns>::ChangeElement(unsigned int r, unsigned int c, float new_value)
    {
        if(r >= this->Elements.size() || c >= this->Elements[0].size())
            return false;
        
        this->Elements[r][c] = new_value;
        return true;
    }

    template<unsigned int rows, unsigned int columns>
    int Matrix<rows, columns>::GetRowCount() const
    {
        return this->row_count;
    }

    template<unsigned int rows, unsigned int columns>
    int Matrix<rows, columns>::GetColumnCount() const
    {
        return this->column_count;
    }

    template<unsigned int rows, unsigned int columns>
    int Matrix<rows, columns>::GetSize() const
    {
        return this->matrix_size;
    }

    template<unsigned int rows, unsigned int columns>
    float Matrix<rows, columns>::GetDeterminant() const
    {
        /* Can't do determinant on un-square matrices! */
        if(this->row_count != this->column_count)
            gk::handle_error("Uneven matrix!");

        float d = 0;
    
        if(this->matrix_size == 4)          // 2x2 matrix
        {
            d = (this->Elements[0][0] * this->Elements[1][1]) - 
                (this->Elements[0][1] * this->Elements[1][0]);
        }
        else if(this->matrix_size == 9)     // 3x3 matrix
        {
            d = (this->Elements[0][0] * ((this->Elements[1][1] * this->Elements[2][2]) - (this->Elements[2][1] * this->Elements[1][2]))) - 
                (this->Elements[0][1] * ((this->Elements[1][0] * this->Elements[2][2]) - (this->Elements[2][0] * this->Elements[1][2]))) + 
                (this->Elements[0][2] * ((this->Elements[1][0] * this->Elements[2][1]) - (this->Elements[2][0] * this->Elements[1][1])));
        }
        else
            gk::handle_error("Matrix is greater than 4x4\nI haven't done the math for larger matrix determinants.\n", "Error: Sorry!");
        
        return d;
    }

    template<unsigned int rows, unsigned int columns>
    float Matrix<rows, columns>::GetElement(unsigned int r, unsigned int c) const
    {
        static std::stringstream err;
        if(r >= this->Elements.size() || c >= this->Elements[0].size())
        {
            err << "Matrix[" << r << "][" << c << "] is out of bounds!";
            gk::handle_error(err.str().c_str());
            err.str(std::string());
        }
        
        return this->Elements[r][c];
    }

    template<unsigned int rows, unsigned int columns>
    void Matrix<rows, columns>::Print() const
    {
        for(size_t i = 0; i < this->row_count; i++)
        {
            printf("[ ");
            for(size_t j = 0; j < this->column_count; j++)
            {
                printf("%0.1f ", this->Elements[i][j]);
            }
            printf("]\n");
        }
        printf("\n");
    }
}

#endif // MATRIX_H