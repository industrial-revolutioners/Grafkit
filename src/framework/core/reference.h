#pragma once
#if 0
#include <memory>

#define USE_STD_SHARED_PTR

/// @file taken from libosmscout: https://github.com/nkostelnik/libosmscout/blob/master/libosmscout/include/osmscout/util/Reference.h

/**
    Baseclass for all classes that support reference counting.
 */

#ifndef USE_STD_SHARED_PTR
class Referencable {
public:
    virtual ~Referencable() = default;
    Referencable() : _ref_count(0) { }

    /**
     Add a reference to this object.
     Increments the internal reference counter.

     **refracted** to have correspondence to [IUnknown::AddRef](https://msdn.microsoft.com/en-us/library/windows/desktop/ms691379(v=vs.85).aspx)
    */
    size_t AddRef() { ++_ref_count; return _ref_count; }

    /**
     RemoveByName a reference from this object.

     Decrements the internal reference counter.

     **refracted** to have correspondence to [IUnknown::Release](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682317(v=vs.85).aspx)
    */
    size_t Release() { _ref_count--; return _ref_count; }

    /**
        Returns the current reference count.
    */
    size_t GetReferenceCount() const { return _ref_count; }

private:
    size_t _ref_count;
};
#else //USE_STD_SHARED_PTR
#pragma message("Referencable is obsolete. Consider remove it.")
class Referencable {}; 
#endif //USE_STD_SHARED_PTR

#ifndef USE_STD_SHARED_PTR

/**
LazyRef does delay the allocation of the referenced object of type T. It also
implements references counting on copy and assignment, allow to exchange
costly copy operations with cheaper reference assignment operations. Using LazyRef
is useful if you reference objects at multiple locations where the object is only
destroyed if all locations delete the object and where copying the object is expensive.

LazyRef allocates a new object instance the first time the reference is dereferenced and
the object instance of type T is accessed.

Note that type T must inherit from class Referencable!
*/
template <typename T> class LazyRef {
private:
    mutable T* ptr;

public:
    /** Default constructor. Creates an empty reference. */
    LazyRef() : ptr(nullptr) { }

    /** Creates an reference holding an instance of T. */
    LazyRef(T* pointer) : ptr(pointer) {
        if (ptr != nullptr) {
            ptr->AddRef();
        }
    }

    /**
     Copy constructor for the same type of reference.
    */
    LazyRef(const LazyRef<T>& other)
        : ptr(other.ptr)
    {
        if (ptr != nullptr) {
            ptr->AddRef();
        }
    }

    /**
     Destructor
    */
    ~LazyRef()
    {
        if (ptr != nullptr &&
            ptr->Release() == 0) {
            delete ptr;
        }
    }

    /**
     Assignment operator.

     Assigns a new value to the reference. The reference count of the
     object hold by the handed reference - if the pointer is not nullptr -
     will be incremented. The reference count of the old value will be
     decremented and freed, if the count reached 0.
    */
    void operator=(const LazyRef<T>& other)
    {
        if (ptr != other.ptr) {
            if (ptr != nullptr &&
                ptr->Release() == 0) {
                delete ptr;
            }

            ptr = other.ptr;

            if (ptr != nullptr) {
                ptr->AddRef();
            }
        }
    }

    /**
     arrow operator.

     Returns the underlying pointer. Makes the reference behave like a pointer.
    */
    T* operator->() const
    {
        if (ptr == nullptr) {
            ptr = new T();
            ptr->AddRef();
        }

        return ptr;
    }

    /**
        Dereference operator.

        Returns a reference to the underlying object. Makes the reference behave like a pointer.
    */
    T& operator*() const
    {
        return *ptr;
    }
};

#else // USE_STD_SHARED_PTR
template <typename T> using LazyRef = std::shared_ptr<T>;
#endif //USE_STD_SHARED_PTR

#ifndef USE_STD_SHARED_PTR
/**
Ref handles references to object using reference counting semantic. The object of type
T is onbly deleted if all references have gone invalid.

Note that type T must inherit from class Referencable!
*/
template <typename T> class Ref
{
public:

    typedef T* clazz;

    /**
     Default constructor. Creates an empty reference.
    */
    Ref() : ptr(nullptr) {}

    /**
     Creates an reference holding an instance of T.
    */
    Ref(T* pointer) : ptr(pointer) {
        if (ptr != nullptr) {
            ptr->AddRef();
        }
    }

    /**
     Copy constructor
    */
    Ref(const Ref<T>& other) : ptr(other.ptr) {
        if (ptr != nullptr) {
            ptr->AddRef();
        }
    }

    /**
      Access operator.

      Returns the underlying pointer. Note that the object is still
      hold by the Reference.
     */
    T* Get() const {
        return ptr;
    }

    /**
     Copy constructor
    */
    template<typename T1>
    Ref(const Ref<T1>& other)
        : ptr(other.Get())
    {
        if (ptr != nullptr) {
            ptr->AddRef();
        }
    }

    /**
     Destructor
    */
    ~Ref()
    {
        if (ptr != nullptr &&
            ptr->Release() == 0) {
            delete ptr;
            ptr = nullptr;
        }
    }

    /**
    Assignment without using `operator =`.

    Assigns a new value to the reference. The reference count of the
    new object - if the pointer is not nullptr - will be incremented.
    The reference count of the old value will be decremented and freed,
    if the count reached 0.
    */

    void AssingnRef(T* pointer)
    {
        if (ptr != pointer) {

            if (pointer != nullptr) {
                pointer->AddRef();
            }

            if (ptr != nullptr &&
                ptr->Release() == 0) {
                delete ptr;
                ptr = nullptr;
            }

            ptr = pointer;
        }
    }

    /**
     Assignment operator.

     Assigns a new value to the reference. The reference count of the
     new object - if the pointer is not nullptr - will be incremented.
     The reference count of the old value will be decremented and freed,
     if the count reached 0.
    */
    Ref<T>& operator=(T* pointer)
    {
        AssingnRef(pointer);
        return *this;
    }

    /**
     Assignment operator.

     Assigns a new value to the reference. The reference count of the
     object hold by the handed reference - if the pointer is not nullptr -
     will be incremented. The reference count of the old value will be
     decremented and freed, if the count reached 0.
    */
    Ref<T>& operator=(const Ref<T>& other)
    {
        if (&other != this && this->ptr != other.ptr) {
            if (ptr != nullptr &&
                ptr->Release() == 0) {
                delete ptr;
            }

            ptr = other.ptr;

            if (ptr != nullptr) {
                ptr->AddRef();
            }
        }

        return *this;
    }

    template<typename T1> Ref<T>& operator=(const Ref<T1>& other)
    {
        if (&other != this && this->ptr != other.Get()) {
            if (ptr != nullptr &&
                ptr->Release()) {
                delete ptr;
            }

            ptr = other.Get();

            if (ptr != nullptr) {
                ptr->AddRef();
            }
        }

        return *this;
    }


    bool Valid() const { return ptr != nullptr; }
    bool Invalid() const { return ptr == nullptr; }

    /**
       arrow operator.

       Returns the underlying pointer. Makes the reference behave like a pointer.
    */
    T* operator->() const {
        return ptr;
    }

    /**
     Dereference operator.

     Returns a reference to the underlying object. Makes the reference behave
     like a pointer.
    */
    T& operator*() const {
        return *ptr;
    }

    /**
     Type conversion operator.

     Returns the underlying pointer. Allows reference to be
     passed as a parameter where the base pointer type is required.
    */
    operator T*() const {
        return ptr;
    }

    /**
     Type conversion operator.

     Returns the underlying object as reference. Allows reference to be
     passed as a parameter where the object type is required.
    */
    operator T&() const {
        return *ptr;
    }

    bool operator==(const Ref<T>& other) const {
        return ptr == other.ptr;
    }

    //bool operator==(const T* const & other) const {
    //	return ptr == other;
    //}

    bool operator!=(const Ref<T>& other) const {
        return ptr != other.ptr;
    }

    //bool operator!=(const T * const & other) const {
    //	return ptr != other;
    //}

    bool operator<(const Ref<T>& other) const {
        return ptr < other.ptr;
    }

    //bool operator<(const T * const & other) const {
    //	return ptr < other;
    //}

    bool operator>(const Ref<T>& other) const {
        return ptr > other.ptr;
    }

    //bool operator>(const T * const & other) const {
    //	return ptr > other;
    //}

    template<class AR>
    void Serialize(AR &ar){
        T * p = ptr;
        ar & p;
        AssingnRef(p);
    }

protected:
    T * ptr;
};

#else //USE_STD_SHARED_PTR
template <typename T> using Ref = std::shared_ptr<T>;
#endif //USE_STD_SHARED_PTR

/// Safe ptr release
#ifndef USE_STD_SHARED_PTR
template <typename T>
constexpr void RELEASE(T *& refptr) { if (refptr) { refptr->Release(); refptr = nullptr; } }
template <typename T>
constexpr void RELEASE(Ref<T> *& refptr) { if (refptr) { refptr->Release(); refptr = nullptr; } }
#else
#if 0
template <typename T>
constexpr void RELEASE(const Ref<T> & refptr) { if (refptr) { refptr->Release(); refptr = nullptr; } }
#endif //0
#endif //USE_STD_SHARED_PTR
#endif 