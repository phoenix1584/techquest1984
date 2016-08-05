/*
 * Copyright (c) 2016
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef TQ_VECTOR_HH
#define TQ_VECTOR_HH
/**
 * TQVector (TQV) is a sample STL vector like implementation. 
 * Reference : Data Structures and Algorithm Analysis in C++ by Mark Allen Weiss
 * Secondary objective is to use C++11 features as well.
 */
#include <memory>
namespace TechQuest{
    template <class T>
        class TQVector{
            private:
                /**
                 * The number of elements currently present in the TQV.
                 */
                int m_size; 

                /**
                 * Total number of elements that can be accommodated in the TQV.
                 */
                int m_capacity;

                /**
                 * The underlying memory element. Using unique pointer for stricter checking.
                 */
                std::unique_ptr<T> m_data; 

                /**
                 * Constant predefined capacity buffer
                 */ 
                enum CAPACITY : int {
                    RESIZE_FACTOR = 2,
                    SPARE_CAPACITY = 16 
                };

            public:

                /**
                 * Default explicit constructor.
                 */
                explicit TQVector (int initSize=0)
                    :m_size(initSize)
                     , m_capacity(initSize + SPARE_CAPACITY)
                     ,m_data(new T[m_capacity])
            {}

                /**
                 * Copy constructor
                 */
                TQVector (const TQVector & rhs ){
                    operator=(rhs); 
                }

                /**
                 * Move constructor 
                 */
                TQVector (TQVector&& rhs){
                    m_data(std::move(rhs.m_data)); 
                    m_size(std::move(rhs.m_size));
                    m_capacity(std::move(rhs.m_capacity));
                }

                /**
                 * Destructor
                 */
                ~TQVector(){}

                /**
                 * operator= is the trickiest since it can cause loss of data if not implemented correctly.
                 */
                const TQVector & operator= (const TQVector & rhs){
                    if(this != &rhs){
                        m_capacity = rhs.capacity();
                        m_data.reset(new T[m_capacity]);
                        // Possible since no reallocation will happen.
                        for(auto&& elem : rhs){
                            push_back(elem);
                        }
                    }
                    return *this;
                }

                /**
                 * Resize the vector ONLY to a size greater than existing size.
                 */
                void resize(int newSize){
                    if(newSize > m_capacity){
                        reserve( newSize * RESIZE_FACTOR + 1);
                    }
                    m_size = newSize;
                }

                /**
                 * Reserve a requested size of memory for the TQV.
                 */
                void reserve(int newCapacity){
                    if(newCapacity > m_capacity){
                        std::unique_ptr<T> backup(m_data);
                        m_data.reset(new T[newCapacity]);
                        for(auto&& elem : &backup.get()){
                            push_back(elem);
                        }
                        m_capacity = newCapacity ;
                    }
                }

                /**
                 * mutable index access. Out of bound check missing.
                 */
                T& operator[] (int index){
                    return m_data[index];
                }   

                /**
                 * constant index access. Out of bound check missing.
                 */
                const T& operator[] (int index) const {
                    return m_data[index];
                }  

                /**
                 * Return if TQV is empty.
                 */
                bool empty() const{
                    return (0 == m_size);
                }

                /**
                 * Return the size of the TQV.
                 */
                int size() const{
                    return m_size;
                }

                /**
                 * Return the capacity of TQV.
                 */
                 int capacity() const{
                    return m_capacity;
                 }
                
                /**
                 * Insert an element at the back.
                 */
                 void push_back(const T& e){
                    if(m_size == m_capacity)
                        reserve( RESIZE_FACTOR * m_capacity + 1);
                    m_data[m_size++] = e;
                 }
                
                /**
                 *  Pop the last element. Notice the optimization of not bothering to overwrite to some default value that element.
                 *  Emphasizing on the fact that iterators are the only safe way of traversing the underlying memory.
                 */
                 void pop_back(){
                    m_size--;
                 }
                
                /**
                 * Returns the last element. Again if no elements are present then beware !!
                 */
                 const T & back () const{
                    return m_data[m_size-1];
                 }

                 typedef T * iterator;
                 typedef const T * const_iterator;

                /**
                 * mutable access to the first element.
                 */
                 iterator begin(){
                    return m_data.get();
                 }
                 
                /**
                 * constant access for the first element.
                 */
                 const_iterator cbegin() const{
                    return m_data.get();
                 }
                 
                /**
                 * mutable access to the last element.
                 */
                 iterator end(){
                    return &m_data[size()];
                 }
                 
                /**
                 * constant access for the last element.
                 */
                 const_iterator cend() const{
                    return &m_data[size()];
                 }

        };
};

#endif /*TQ_VECTOR_HH*/
