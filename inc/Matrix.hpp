/**
*  @file    Matrix.hpp
*  @author  Alexander Arzhanov
*  @version 25/01/18
*
***********
*  @brief   Definition of Matrix templates.
***********
**/


#pragma once

#include <exception>
#include <valarray>
#include <ostream>
#include <type_traits>
#include <utility>

#include <algorithm>  // NEED: std::copy

// Namespace
namespace aarz {

   // Forward declarations for T-specializations of 'operator<<'
   template<typename T> class Matrix;
   template<typename T> std::ostream& operator<<(std::ostream&, const Matrix<T>&);


   template <class T>
   class Matrix {

      using Storage = std::valarray<T>;
      using SelfType = Matrix<T>;

   public:

      // CTORS:
      explicit Matrix(const size_t size1, const size_t size2);
      Matrix(const Matrix &mx);

      // TODO: 
      //   - move ctor
      //   - other ctors?

      // OPETORS:
      Matrix& operator=(const Matrix &mx);      

      // INDICES:
      T& operator()(const size_t r, const size_t c) { return _data[r * _size2 + c]; }
      const T& operator()(const size_t r, const size_t c) const { return _data[r * _size2 + c]; }

      // ACCESSORS:
      auto begin( ) { return std::begin(_data); }
      auto begin( ) const { return std::begin(_data); }
      auto end( ) { return std::end(_data); }
      auto end( ) const { return std::end(_data); }

      // FRIENDS:
      friend std::ostream& operator<< <> (std::ostream &os, const Matrix &mx);

   private:

      size_t _size1;
      size_t _size2;
      Storage _data;
   };


   
   
   // CTORS:
   template<class T> inline
   Matrix<T>::Matrix(const size_t size1, const size_t size2) :
      _size1 { size1 },
      _size2 { size2 }
   {
      if (_size1 == 0 || _size2 == 0) {
         throw MatrixException_InvalidSize {};
      }

      _data.resize(_size1 * _size2);
   }

   template<class T> inline
   Matrix<T>::Matrix(const Matrix &mx) :
      _size1 { mx._size1 },
      _size2 { mx._size2 }
   {
      _data.resize(_size1 * _size2);
      std::copy(mx.begin( ), mx.end( ), std::begin(_data));
   }

   // OPETORS:
   template<class T> inline
   Matrix<T>& Matrix<T>::operator=(const Matrix<T> &mx)
   {
      if (this != &mx) {

         _size1 = mx._size1;
         _size2 = mx._size2;
         _data.resize(_size1 * _size2);

         std::copy(mx.begin( ), mx.end( ), std::begin(_data));
      }

      return *this;
   }


   // EXCEPTIONS:
   class MatrixException : std::exception {
   public:
      MatrixException(const char* msg) : std::exception(msg) { }
   };

   class MatrixException_InvalidSize : MatrixException {
   public:
      MatrixException_InvalidSize( ) : MatrixException("All dimensions of a matrix should be strictly positive") { }
   };


   // TODO: move out from decl
   //template<class T> Matrix;

   template<class T>
   std::ostream& operator<<(std::ostream &os, const Matrix<T> &mx) {

      for (size_t i = 0; i < mx._data.size( ); i++)
         os << ((i % mx._size2) ? " " : "\n ") << mx._data[i];

      return os << "\n";
   }


} // endof: namespace aarz