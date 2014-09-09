/* $Id: //dvt/mti/rel/10.0b/src/systemc/ptrarray.h#1 $ */

#ifndef _PTRARRAY_H_
#define _PTRARRAY_H_

#include <stdlib.h>   /* malloc and free */

/*
 * class ptrarray
 *
 * This implements a simple dynamical array that keeps the generic pointers.
 * The intent is to replace the std::vector usage in SystemC code in cases when
 * a simple pointer array would suffice, for example, the trigger lists of sc_event.
 * It is not intended to be used as a generic dynamically growing array utility class, 
 * since only interfaces being used in SystemC code are implemented.
 *
 * The std::vector calls shows up in profile report with a non-trivial percentage
 * CPU time unexpectedly. The use of a simple array had significantly reduced this 
 * performance cost.
 *
 * For any non-pointer template type instantiation of ptrarray, one would get compilation 
 * error.
 *
 * The unit test for this class is at tests_systemc/utility_tests/ptrarray1. It should be kept
 * updated when there are additionally required interfaces changes.
 *
 */
#ifndef NULL
#define NULL 0
#endif

template <typename PTR_TYPE>
class ptrarray
{
 public:
    ptrarray();
    ~ptrarray();

    int         size() const;

    PTR_TYPE    operator[](int index);
    PTR_TYPE    operator[](int index) const;

    // instead of accessing element by operator[], access by raw pointer directly
    // to gain performance, i.e., in sc_event::trigger().
    PTR_TYPE*   get_rawptr() const;

    // return 0 on success and 1 on failure (preserve element order)
    int        remove(PTR_TYPE elemt);

    // return 0 on success and 1 on failure (dont preserve element order)
    int        removeFast(PTR_TYPE elemt);

    // return 0 on success and 1 on failure
    int        remove_all();

    // return 0 on success and 1 on failure
    int        push_back(PTR_TYPE elemt);

 private:
    /* disable the default copy ctor and assignment operator */
    ptrarray(const ptrarray& src);
    ptrarray& operator=(const ptrarray& rhs); 

 private:    
    int resize_physical(int size);

 private:

  PTR_TYPE*   m_ptr;          // actual data storage
  int         m_size;         // logical size
  int         m_phys_size;    // physical size

  static  int m_default_init_size;   // default initial size
};


// only the pointer-type specialization is defined. For any non-pointer type,
// there will be compilation error when it is used as template parameter.

template <typename T>
class ptrarray<T*>
{
 public:
    ptrarray();
    ~ptrarray();

    int        size() const { return m_size; }

    T*         operator[](int index);
    T*         operator[](int index) const;

    // instead of accessing element by operator[], access by raw pointer directly
    // to gain performance, i.e., in sc_event::trigger().
    T**        get_rawptr() const;

    // return 0 on success and 1 on failure (preserve element order)
    int        remove(T* elemt);

    // return 0 on success and 1 on failure (dont preserve element order)
    int        removeFast(T* elemt);

    // return 0 on success and 1 on failure
    int        remove_all();

    // return 0 on success and 1 on failure
    int        push_back(T* elemt);

 private:
    /* disable the default copy ctor and assignment operator */
    ptrarray(const ptrarray& src);
    ptrarray& operator=(const ptrarray& rhs); 

 private:    
    int resize_physical(int size);

 private:

  T**         m_ptr;          // actual data storage
  int         m_size;         // logical size
  int         m_phys_size;    // physical size

  static      int m_default_init_size;   // default initial size
};

// implementations

#define PTRARRAY_MALLOC  malloc
#define PTRARRAY_FREE    free

template <typename T>
int ptrarray<T*>::m_default_init_size = 16;

template <typename T>
ptrarray<T*>::ptrarray()
  : m_ptr (NULL)
  , m_size (0)
  , m_phys_size (0)
{
}

template <typename T>
ptrarray<T*>::~ptrarray()
{
    if (m_ptr) {
        PTRARRAY_FREE(m_ptr);
	m_ptr = NULL;
    }
    m_size = 0;
    m_phys_size = 0;
}

template <typename T>
int
ptrarray<T*>::resize_physical(int requestedSize)
{
    register int newSize = (m_default_init_size < m_phys_size) ? m_phys_size : m_default_init_size;

    // growth factor about 1.5
    while (newSize < requestedSize)
        newSize += (newSize>>1);

    T** new_ptr = (T**) PTRARRAY_MALLOC(newSize*sizeof(T*));
    if (!new_ptr)
        return 1;

    if (m_ptr) {
        if (m_size > 0)
            memcpy(new_ptr, m_ptr, m_size*sizeof(T*));
        PTRARRAY_FREE(m_ptr);
    }
    m_ptr = new_ptr;
    m_phys_size = newSize;
    return 0;
}

template <typename T>
T*
ptrarray<T*>::operator[](int index)
{
  if (index < m_size)
      return m_ptr[index];
  return NULL;
}

template <typename T>
T*
ptrarray<T*>::operator[](int index) const
{
  if (index < m_size)
      return m_ptr[index];
  return NULL;
}

template <typename T>
T**
ptrarray<T*>::get_rawptr() const
{
    return m_ptr;
}

template <typename T>
int
ptrarray<T*>::remove_all()
{
    m_size = 0;
    return 0;
}

template <typename T>
int
ptrarray<T*>::remove(T* elemt)
{
    if (m_size == 0)
        return 1;

    // removed item is the last element.
    if (elemt == m_ptr[m_size-1]) {
        m_size -= 1;
	return 0;
    }

    if (m_size > 1) {
        int index = -1;
        for(int i = m_size - 2; i >= 0; --i) {
 	    if (elemt == m_ptr[i]) {
	        index = i;
		break;
	    }
	}

	if (index >= 0) {
	    for(int i = index; i < m_size-1; ++i) {
	        m_ptr[i] = m_ptr[i+1];
	    }
	    m_size -= 1;
	    return 0;
	}
    }

    return 1;
}	

template <typename T>
int
ptrarray<T*>::removeFast(T* elemt)
{
    if (m_size > 0) {
        for (int i = m_size-1; i >=0; --i) {
            if (elemt == m_ptr[i]) {
                m_ptr[i] = m_ptr[m_size-1];
                m_size -= 1;
                return 0;
            }
	}
    }
    return 1;
}

template <typename T>
int
ptrarray<T*>::push_back(T* elemt)
{
   if (m_size+1> m_phys_size) {
       int fail = resize_physical(m_size+1);
       if (fail)
           return 1;
   }
   m_ptr[m_size] = elemt;
   m_size += 1;
   return 0;
}

#endif
