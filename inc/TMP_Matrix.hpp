#pragma once

Matrix(Matrix&& other) :
   _rowCount { other._rowCount },
   _columnCount { other._columnCount },
   _data { std::move(other._data) }
{
   other._rowCount = other._columnCount = 0;
   other._data = std::vector<T> {};
}

virtual ~Matrix( ) = default;

static Matrix CreateRowMatrix(const size_t columnCount)
{
   return Matrix { columnCount, 1 };
}

static Matrix CreateRowMatrix(const std::vector<T>& arr)
{
   Matrix resultMatrix { 1, arr.size( ), false };
   std::copy(arr.begin( ), arr.end( ), resultMatrix.begin( ));

   return resultMatrix;
}

static Matrix CreateColumnMatrix(const size_t rowCount)
{
   return Matrix { rowCount, 1 };
}

static Matrix CreateColumnMatrix(const std::vector<T>& arr)
{
   Matrix resultMatrix { arr.size( ), 1, false };
   std::copy(arr.begin( ), arr.end( ), resultMatrix.begin( ));

   return resultMatrix;
}

// todo, enable only for primitive type
static Matrix CreateIdentityMatrix(const size_t size, const T value, bool rightToLeft = false)
{
   Matrix resultMatrix { size, size };
   for (size_t i { 0 }; i < size; ++i)
   {
      resultMatrix(i, rightToLeft ? (size - i - 1) : i) = value;
   }

   return resultMatrix;
}

// for non primitive arithmetic types, pass by const reference
// TODO: is it really useful? little optimization(not passing primitive type by reference) vs code bloat from template??? - same with multiplication by scalar
// maybe remove the enableif and pass everything by const ref
template<class U = T, typename std::enable_if<!std::is_arithmetic<U>::value>::type>
static Matrix CreateIdentityMatrix(const size_t size, const T& value, bool rightToLeft = false)
{
   Matrix resultMatrix { size, size };
   for (size_t i { 0 }; i < size; ++i)
   {
      resultMatrix(i, rightToLeft ? (size - i - 1) : i) = value;
   }

   return resultMatrix;
}

// ************** END OF CONSTRUCTION / DESTRUCTION ************** //
// ************** BEGIN OF OPERATORS ************** //

Matrix& operator=(const Matrix& other)
{
   if (this != &other)
   {
      _rowCount = _columnCount = 0;
      _data.empty( );
      _data.resize(other._data.size( ));

      std::copy(other.begin( ), other.end( ), _data.begin( ));
      _rowCount = other._rowCount;
      _columnCount = other._columnCount;
   }

   return *this;
}

Matrix& operator=(Matrix&& other)
{
   if (this != &other)
   {
      _rowCount = other._rowCount;
      _columnCount = other._columnCount;
      _data = std::move(other._data);

      other._rowCount = other._columnCount = 0;
      other._data = std::vector<T> {};
   }

   return *this;
}

T& operator[](const size_t index) { return _data[index]; }
const T& operator[](const size_t index) const { return _data[index]; }

// substitution for double subscript[][]
T& operator()(const size_t rowIndex, const size_t columnIndex) { return _data[index(rowIndex, columnIndex)]; }
const T& operator()(const size_t rowIndex, const size_t columnIndex) const { return _data[index(rowIndex, columnIndex)]; }

Matrix& operator+=(const Matrix& rightMatrix)
{
   if (_rowCount != rightMatrix._rowCount)
   {
      throw Matrix_DifferentRowCount {};
   }
   else if (_columnCount != rightMatrix._columnCount)
   {
      throw Matrix_DifferentColumnCount {};
   }

   auto leftIt { begin( ) };
   auto rightIt { rightMatrix.begin( ) };
   while (leftIt != end( ))
   {
      *leftIt++ += *rightIt++;
   }

   return *this;
}

Matrix& operator-=(const Matrix& rightMatrix)
{
   if (_rowCount != rightMatrix._rowCount)
   {
      throw Matrix_DifferentRowCount {};
   }
   else if (_columnCount != rightMatrix._columnCount)
   {
      throw Matrix_DifferentColumnCount {};
   }

   auto leftIt { begin( ) };
   auto rightIt { rightMatrix.begin( ) };
   while (leftIt != end( ))
   {
      *leftIt++ -= *rightIt++;
   }

   return *this;
}

// TODO: enableif U is primitive arithmetic type
template<class U>
Matrix& operator*=(const U scalar)
{
   auto leftIt { begin( ) };
   while (leftIt != end( ))
   {
      *leftIt++ *= scalar;
   }

   return *this;
}

template<class U = T, typename std::enable_if<!std::is_arithmetic<U>::value>::type>
Matrix& operator*=(const U& scalar)
{
   auto leftIt { begin( ) };
   while (leftIt != end( ))
   {
      *leftIt++ *= scalar;
   }

   return *this;
}

Matrix operator*=(const Matrix& rightMatrix)
{
   if (_columnCount != rightMatrix._rowCount)
   {
      throw Matrix_RowColumnMismatch {};
   }

   *this = std::move(*this * rightMatrix);

   return *this;
}

// ************** END OF OPERATORS ************** //
// ************** BEGIN OF ACCESSORS ************** //

// AARZ: IMPL
auto begin( ) { return _data.begin( ); }
auto begin( ) const { return _data.begin( ); }
auto end( ) { return _data.end( ); }
auto end( ) const { return _data.end( ); }

size_t GetRowCount( ) const { return _rowCount; }
size_t GetColumnCount( ) const { return _columnCount; }
size_t GetDataCount( ) const { return _data.size( ); }

const size_t index(const size_t row, const size_t column) const { return row * _columnCount + column; }

// ************** END OF ACCESSORS ************** //

void SwapRows(const size_t row1, const size_t row2)
{
   // todo: assert input

   // we can optimize this for rows, swap entire row in one run
   for (size_t column = 0; column < _columnCount; ++column)
   {
      std::swap((*this)(row1, column), (*this)(row2, column));
   }
}

void SwapColumns(const size_t column1, const size_t column2)
{
   // todo: assert input

   for (size_t row = 0; row < _rowCount; ++row)
   {
      std::swap((*this)(row, column1), (*this)(row, column2));
   }
}

void RemoveRows(const size_t beginRow, const size_t amount = 1)
{
   *this = std::move(RemoveRows(*this, beginRow, amount));
}

//void RemoveColumns(const size_t beginRow, const size_t amount = 1)
//{
//  *this = std::move(RemoveCols(*this, beginRow, amount));
//}

static Matrix Add(const Matrix& leftMatrix, const Matrix& rightMatrix) { return leftMatrix + rightMatrix; }
static Matrix Subtract(const Matrix& leftMatrix, const Matrix& rightMatrix) { return leftMatrix - rightMatrix; }
static Matrix Multiply(const Matrix& leftMatrix, const Matrix& rightMatrix) { return leftMatrix * rightMatrix; }

static Matrix SwapRows(const Matrix& matrix, const size_t row1, const size_t row2)
{
   Matrix resultMatrix { matrix };
   resultMatrix.SwapRows(row1, row2);

   return resultMatrix;
}

static Matrix SwapColumns(const Matrix& matrix, const size_t column1, const size_t column2)
{
   Matrix resultMatrix { matrix };
   resultMatrix.SwapColumns(column1, column2);

   return resultMatrix;
}

static Matrix RemoveRows(const Matrix& matrix, const size_t beginRow, const size_t amount = 1)
{
   // TODO: assert input

   Matrix resultMatrix { matrix._rowCount - amount, matrix._columnCount, false };

   std::copy(matrix.begin( ), matrix.begin( ) + beginRow * matrix._columnCount, resultMatrix.begin( ));
   std::copy(matrix.begin( ) + (beginRow + amount) * matrix._columnCount, matrix.end( ), resultMatrix.begin( ) + beginRow * matrix._columnCount);

   return resultMatrix;
}

//static Matrix RemoveCols(const Matrix& matrix, const size_t beginRow, const size_t amount = 1)
//{
//  // TODO

//  return Matrix<T>{1, 1};
//}

// todo: move outside of the class
// AARZ: IMPL
friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
   os << "\n";
   for (size_t index { 0 }; index < matrix._data.size( ); ++index)
   {
      if (index != 0 && index % matrix._columnCount == 0)
      {
         os << "\n";
      }

      os << matrix._data[index] << " ";
   }

   return os << "\n";
}

protected:
   size_t _rowCount, _columnCount;
   Storage _data;
};

template<class T>
bool operator==(const Matrix<T>& leftMatrix, const Matrix<T>& rightMatrix)
{
   if (&leftMatrix == &rightMatrix)
   {
      return true;
   }

   if (leftMatrix.GetRowCount( ) != rightMatrix.GetRowCount( ) ||
      leftMatrix.GetColumnCount( ) != rightMatrix.GetColumnCount( ))
   {
      return false;
   }

   auto leftIt { leftMatrix.begin( ) };
   auto rightIt { rightMatrix.begin( ) };

   while (leftIt != leftMatrix.end( ))
   {
      if (*leftIt++ != *rightIt++)
      {
         return false;
      }
   }

   return true;
}

template<class T>
bool operator!=(const Matrix<T>& leftMatrix, const Matrix<T>& rightMatrix)
{
   return !(leftMatrix == rightMatrix);
}


// TODO: remove the code and return += in here or leave it as it is, same with operator-
template<class T>
Matrix<T> operator+(const Matrix<T>& leftMatrix, const Matrix<T>& rightMatrix)
{
   if (leftMatrix.GetRowCount( ) != rightMatrix.GetRowCount( ))
   {
      throw Matrix_DifferentRowCount {};
   }
   else if (leftMatrix.GetColumnCount( ) != rightMatrix.GetColumnCount( ))
   {
      throw Matrix_DifferentColumnCount {};
   }

   auto resultMatrix { Matrix<T>{leftMatrix.GetRowCount( ), leftMatrix.GetColumnCount( ), false } };
   auto resultIt { resultMatrix.begin( ) };
   auto it1 { leftMatrix.begin( ) };
   auto it2 { rightMatrix.begin( ) };

   while (resultIt != resultMatrix.end( ))
   {
      *resultIt++ = *it1++ + *it2++;
   }

   return resultMatrix;
}

template<class T>
Matrix<T> operator-(const Matrix<T>& leftMatrix, const Matrix<T>& rightMatrix)
{
   if (leftMatrix.GetRowCount( ) != rightMatrix.GetRowCount( ))
   {
      throw Matrix_DifferentRowCount {};
   }
   else if (leftMatrix.GetColumnCount( ) != rightMatrix.GetColumnCount( ))
   {
      throw Matrix_DifferentColumnCount {};
   }

   auto resultMatrix { Matrix<T>{leftMatrix.GetRowCount( ), leftMatrix.GetColumnCount( ), false } };
   auto resultIt { resultMatrix.begin( ) };
   auto leftIt { leftMatrix.begin( ) };
   auto rightIt { rightMatrix.begin( ) };

   while (resultIt != resultMatrix.end( ))
   {
      *resultIt++ = *leftIt++ - *rightIt++;
   }

   return resultMatrix;
}

template<class T>
Matrix<T> operator-(const Matrix<T>& matrix)
{
   auto resultMatrix { Matrix<T>{matrix.GetRowCount( ), matrix.GetColumnCount( ), false } };
   auto resultIt { resultMatrix.begin( ) };
   auto matrixIt { matrix.begin( ) };

   while (resultIt != resultMatrix.end( ))
   {
      *resultIt++ = -*matrixIt++;
   }

   return resultMatrix;
}

// TODO: enable only for primitive arithmetic types
template<class T, class U>
Matrix<T> operator*(const Matrix<T>& matrix, const U scalar)
{
   auto resultMatrix { Matrix<T>{matrix.GetRowCount( ), matrix.GetColumnCount( ), false } };
   auto resultIt { resultMatrix.begin( ) };
   auto matrixIt { matrix.begin( ) };

   while (resultIt != resultMatrix.end( ))
   {
      *resultIt++ = *matrixIt++ * scalar;
   }

   return resultMatrix;
}

template<class T, typename std::enable_if<!std::is_arithmetic<T>::value>::type>
Matrix<T> operator*(const Matrix<T>& matrix, const T& scalar)
{
   auto resultMatrix { Matrix<T>{matrix.GetRowCount( ), matrix.GetColumnCount( ), false } };
   auto resultIt { resultMatrix.begin( ) };
   auto matrixIt { matrix.begin( ) };

   while (resultIt != resultMatrix.end( ))
   {
      *resultIt++ = *matrixIt++ * scalar;
   }

   return resultMatrix;
}

template<class T>
Matrix<T> operator*(const Matrix<T>& leftMatrix, const Matrix<T>& rightMatrix)
{
   if (leftMatrix.GetColumnCount( ) != rightMatrix.GetRowCount( ))
   {
      throw Matrix_RowColumnMismatch {};
   }

   auto resultMatrix { Matrix<T>{leftMatrix.GetRowCount( ), rightMatrix.GetColumnCount( ), false } };
   for (size_t row { 0 }; row < resultMatrix.GetRowCount( ); ++row)
   {
      for (size_t column { 0 }; column < resultMatrix.GetColumnCount( ); ++column)
      {
         for (size_t i { 0 }; i < leftMatrix.GetColumnCount( ); ++i)
         {
            resultMatrix(row, column) += leftMatrix(row, i) * rightMatrix(i, column);
         }
      }
   }

   return resultMatrix;
}

class Matrix_Exception : std::exception
{
public:
   Matrix_Exception(const char* msg) : std::exception(msg) {};
};

class Matrix_WrongRowCount : Matrix_Exception
{
public:
   Matrix_WrongRowCount( ) : Matrix_Exception("Row count should be at least 1") {}
};

class Matrix_WrongColumnCount : Matrix_Exception
{
public:
   Matrix_WrongColumnCount( ) : Matrix_Exception("Column count should be at least 1") {}
};

class Matrix_ArrayIsEmpty : Matrix_Exception
{
public:
   Matrix_ArrayIsEmpty( ) : Matrix_Exception("Array argument is empty") {}
};

class Matrix_ArrayIsJagged : Matrix_Exception
{
public:
   Matrix_ArrayIsJagged( ) : Matrix_Exception("Array argument is jagged") {}
};

class Matrix_DifferentRowCount : Matrix_Exception
{
public:
   Matrix_DifferentRowCount( ) : Matrix_Exception("Matrices have different row count") {}
};

class Matrix_DifferentColumnCount : Matrix_Exception
{
public:
   Matrix_DifferentColumnCount( ) : Matrix_Exception("Matrices have different column count") {}
};

class Matrix_RowColumnMismatch : Matrix_Exception
{
public:
   Matrix_RowColumnMismatch( ) : Matrix_Exception("Left column count is different than right row count") {}
};