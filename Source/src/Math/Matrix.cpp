/**
 * @file
 *  CMatrix class implementations.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 **/

#include "Math/Matrix.hpp"

using math::CMatrix;

/**
 * Create a matrix with the specified dimensions.
 *
 * @param int The amount of rows in the matrix
 * @param int The amount of columns in the matrix
 **/
CMatrix::CMatrix(u_int rows, u_int columns) : m_rows(rows), m_columns(columns)
{
    m_elements.resize(rows);

    for(u_int i = 0; i < rows; ++i)
        m_elements[i].resize(columns);

    for(u_int i = 0; i < rows; ++i)
        for(u_int j = 0; j < columns; ++j)
            m_elements[i][j] = 0.0f;
}

/**
 * Create a matrix with the specified dimensions and load it with custom data.
 *
 * @param int The amount of rows in the matrix
 * @param int The amount of columns in the matrix
 * @param float[][] Data to insert into the matrix.
 *   If it's too small, there is no bounds checking. So it's the callers
 *   responsibility to ensure that the array is large enough to accommodate
 *   the matrix.
 **/
CMatrix::CMatrix(u_int rows, u_int columns, const float** pdata) : m_rows(rows),
    m_columns(columns)
{
    m_elements.resize(rows);

    for(u_int i = 0; i < rows; ++i)
        m_elements[i].resize(columns);

    for(u_int i = 0; i < rows; ++i)
        for(u_int j = 0; j < columns; ++j)
            m_elements[i][j] = pdata[i][j];
}

/**
 * Create a matrix copied from another specified matrix.
 *
 * @param CMatrix The matrix to copy.
 **/
CMatrix::CMatrix(const CMatrix& Copy) : m_rows(Copy.GetRowCount()),
    m_columns(Copy.GetColumnCount())
{
    m_elements.resize(m_rows);

    for(u_int i = 0; i < m_rows; ++i)
        m_elements[i].resize(m_columns);

    for(u_int i = 0; i < m_rows; ++i)
        for(u_int j = 0; j < m_columns; ++j)
            m_elements[i][j] = Copy.GetElement(i, j);
}

/**
 * Assign all values of a new matrix to the current one.
 *
 * @param CMatrix The matrix to get values from
 * 
 * @return The current matrix, modified.
 **/
CMatrix& CMatrix::operator= (const CMatrix& Copy)
{
    m_rows = Copy.GetRowCount();
    m_columns = Copy.GetColumnCount();
    
    m_elements.resize(m_rows);

    for(u_int i = 0; i < m_rows; ++i)
        m_elements[i].resize(m_columns);

    for(u_int i = 0; i < m_rows; ++i)
        for(u_int j = 0; j < m_columns; ++j)
            m_elements[i][j] = Copy.GetElement(i, j);

    return *this;
}

/** 
 * Add two matrices together.
 *
 * The matrices MUST be the same dimensions.
 *
 * @param CMatrix The matrix to add to the current one
 *
 * @return A new matrix consisting of the added elements.
 **/
CMatrix CMatrix::operator+ (const CMatrix& Other) const
{
    if(m_rows != Other.GetRowCount() || 
        m_columns != Other.GetColumnCount())
    {
        gk::handle_error("Matrices must be the same size!");
    }

    CMatrix Answer(m_rows, m_columns);

    for(u_int i = 0; i < m_rows; ++i)
    {
        for(u_int j = 0; j < m_columns; ++j)
        {
            float sum = this->GetElement(i, j) + Other.GetElement(i, j);
            Answer.ChangeElement(i, j, sum);
        }
    }

    return Answer;
}

/** 
 * Subtract two matrices from each other.
 *
 * The matrices MUST be the same dimensions.
 *
 * @param CMatrix The matrix to add to the current one
 *
 * @return A new matrix consisting of the subtracted elements.
 **/
 CMatrix CMatrix::operator- (const CMatrix& Other) const
{
    if(m_rows != Other.GetRowCount() || 
        m_columns != Other.GetColumnCount())
    {
        gk::handle_error("Matrices must be the same size!");
    }

    CMatrix Answer(m_rows, m_columns);

    for(u_int i = 0; i < m_rows; ++i)
    {
        for(u_int j = 0; j < m_columns; ++j)
        {
            float diff = this->GetElement(i, j) - Other.GetElement(i, j);
            Answer.ChangeElement(i, j, diff);
        }
    }

    return Answer;
}

/** 
 * Creates a matrix with each element multiplied by a scalar variable.
 *
 * @param CMatrix the matrix to add to the current one
 *
 * @return A new matrix scalar'ed.
 **/
CMatrix CMatrix::operator* (const float scalar) const
{
    CMatrix Answer(m_rows, m_columns);

    for(u_int i = 0; i < m_rows; ++i)
    {
        for(u_int j = 0; j < m_columns; ++j)
        {
            float value = this->GetElement(i, j) * scalar;
            Answer.ChangeElement(i, j, value);
        }
    }

    return Answer;
}

/** 
 * Multiply two matrices together. 
 *
 * @param CMatrix the matrix to add to the current one
 *
 * @return A new matrix representing the multiplied matrices
 **/
CMatrix CMatrix::operator* (const CMatrix& Other) const
{
    CMatrix Answer(m_rows, Other.GetColumnCount());

    for(u_int i = 0; i < m_rows; ++i)
    {
        for(u_int j = 0; j < m_columns; ++j)
        {
            float value = 0.0f;
            for(u_int k = 0; k < m_columns; k++)
            {
                value += m_elements[i][k] * Other.GetElement(k, j);
            }

            Answer.ChangeElement(i, j, value);
        }
    }

    return Answer;
}

/**
 * Compare two matrices.
 *
 * The individual elements of both matrices are compared. If the dimensions
 * differ, or any of the elements are not the same, the matrices are not
 * equal.
 *
 * @param CMatrix the matrix to compare to
 *
 * @return TRUE if matrices are equal, FALSE otherwise.
 **/
bool CMatrix::operator== (const CMatrix& Other) const
{
    if(m_columns != Other.GetColumnCount() ||
        m_rows != Other.GetRowCount())
    {
        return false;
    }

    for(u_int i = 0; i < m_rows; ++i)
        for(u_int j = 0; j < m_columns; ++j)
            if(m_elements[i][j] != Other.GetElement(i, j))
                return false;

    return true;
}

/**
 * Change the value of a certain element in the matrix.
 *
 * @param int Row position
 * @param int Column position
 * @param float New value
 *
 * @return TRUE if element was changed, FALSE if it doesn't exist.
 **/
bool CMatrix::ChangeElement(const u_int r, const u_int c, const float value)
{
    if(r > m_rows || c > m_columns)
        return false;

    m_elements[r][c] = value;
    return true;
}

/**
 * Retreives the row count.
 *
 * @return Row count
 **/
int CMatrix::GetRowCount() const
{
    return m_rows;
}

/**
 * Retreives the column count.
 *
 * @return Column count
 **/
int CMatrix::GetColumnCount() const
{
    return m_columns;
}

/**
 * Retreive matrix size.
 *
 * @return CMatrix size
 **/
int CMatrix::GetSize() const
{
    return m_rows * m_columns;
}

/**
 * Calculates the determinant of the matrix.
 *
 * @return The determinant.
 **/
float CMatrix::Determinant() const
{
    /* Can't do determinant on un-square matrices! **/
    if(m_rows != m_columns)
        gk::handle_error("Uneven matrix!");

    float d = 0;

    switch(this->GetSize())
    {
        case 4:
            d = (m_elements[0][0] * m_elements[1][1]) - 
                (m_elements[0][1] * m_elements[1][0]);
            break;

        case 9:
            d = (m_elements[0][0] *
                    ((m_elements[1][1] * m_elements[2][2]) - 
                    (m_elements[2][1] * m_elements[1][2]))) - 
                (m_elements[0][1] * 
                    ((m_elements[1][0] * m_elements[2][2]) - 
                    (m_elements[2][0] * m_elements[1][2]))) + 
                (m_elements[0][2] * 
                    ((m_elements[1][0] * m_elements[2][1]) -
                    (m_elements[2][0] * m_elements[1][1])));
            break;

        default:
            gk::handle_error("Determinants for matrices greater than 3x3"
                "have not been implemented yet!");
            break;
    }

    return d;
}

/**
 * Retreives the value at a certain location.
 *
 * @param int Row
 * @param int Column
 *
 * @return Value at [row][column].
 **/
float CMatrix::GetElement(const u_int r, const u_int c) const
{
    if(r > m_rows || c > m_columns)
        gk::handle_error("CMatrix out-of-bounds!");

    return m_elements[r][c];
}

/**
 * Prints the values of the matrix in an organized format.
 **/
void CMatrix::Print() const
{
    std::cout.precision(2);

    for(size_t i = 0; i < m_rows; ++i)
    {
        std::cout << "[ ";
        for(size_t j = 0; j < m_columns; ++j)
        {
            std::cout << m_elements[i][j];
        }
        std::cout << "]" << std::endl;
    }
    
    std::cout << std::endl;
}
