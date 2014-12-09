#include <stdexcept>
#include <ostream>
#include <mutex>
#include <functional>


template<typename T>
struct function_traits;

template<typename R, typename ...Args>
struct function_traits<R(*)(Args...)>
{
    static const size_t nargs = sizeof...(Args);

    typedef R result_type;

    template <size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
    };
};


template<class T>
class weak_ptr;

class ref_counter
{
private:
    long _shared_refs;
    long _weak_refs;

    mutable std::mutex lock;

public:
    ref_counter() :
        _shared_refs(1),
        _weak_refs(0)
    {
    }

    virtual ~ref_counter() {}

    void inc_shared()
    {
        std::lock_guard<std::mutex> lk(lock);

        ++_shared_refs;
    }

    void dec_shared()
    {
        std::lock_guard<std::mutex> lk(lock);

        if (_shared_refs == 0)
        {
            throw std::logic_error("shared references: already zero");
        }

        --_shared_refs;

        if (_shared_refs == 0)
        {
            dispose();
        }
    }

    void inc_weak()
    {
        std::lock_guard<std::mutex> lk(lock);

        ++_weak_refs;
    }

    void dec_weak()
    {
        std::lock_guard<std::mutex> lk(lock);

        if (_weak_refs == 0)
        {
            throw std::logic_error("weak references: already zero");
        }

        --_weak_refs;
    }

    long shared_refs() const
    {
        std::lock_guard<std::mutex> lk(lock);

        return _shared_refs;
    }

    long weak_refs() const
    {
        std::lock_guard<std::mutex> lk(lock);

        return _weak_refs;
    }

    bool has_refs() const
    {
        std::lock_guard<std::mutex> lk(lock);

        return _shared_refs + _weak_refs > 0;
    }

    virtual void dispose() = 0;
};

template<class Y>
struct ptr_and_block : ref_counter
{
    Y* _ptr;
    explicit ptr_and_block(Y* p) : _ptr(p) {}
    virtual void dispose() 
    {
        delete _ptr; 
        _ptr = nullptr;
    }
};

template<class Y>
struct object_and_block : ref_counter
{
    Y object;

    template<class ... Args>
    object_and_block(Args && ...args) :
        object(args...)
    {}

    virtual void dispose() { /* no-op */ }
};

template<class Y, class D>
struct ptr_and_deleter : ref_counter
{
    typedef Y erased_type;

    Y* _ptr;
    D _deleter;
    
    ptr_and_deleter(Y* ptr, D deleter) :
        _ptr(ptr),
        _deleter(deleter)
    {}

    virtual void dispose() { _deleter(_ptr); }
};

template<class T>
class shared_ptr
{
private:

    T* _obj_ptr;
    ref_counter* _ref_cntr;

    void _check_delete_ptr()
    {
        if (_ref_cntr == nullptr)
        {
            return;
        }

        _ref_cntr->dec_shared();

        if (!_ref_cntr->has_refs())
        {
            delete _ref_cntr;
        }

        _reset_no_delete();
    }

    void _reset_no_delete()
    {
        _obj_ptr = nullptr;
        _ref_cntr = nullptr;
    }

    void _reset_ref_counter(T* ptr, ref_counter* cntr)
    {
        _obj_ptr = ptr;
        _ref_cntr = cntr;
    }

    template<class Y>
    void _set_ptr(Y* ptr)
    {
        _ref_cntr = new ptr_and_block<Y>(ptr);
        _obj_ptr = ptr;
    }

    template<class Y, class Deleter>
    void _set_deleter_ptr(Y* ptr, Deleter d)
    {
        _ref_cntr = new ptr_and_deleter<Y, Deleter>(ptr, d);
        _obj_ptr = ptr;
    }

    template<class U, class ... Args>
    friend shared_ptr<U> make_shared(Args && ... args);

    template<class U>
    friend class weak_ptr;

    template<class Deleter, class T>
    friend Deleter* get_deleter(const shared_ptr<T>& p);

    template<class U>
    friend class shared_ptr;

public:
    shared_ptr()
    {
        _set_ptr<T>(nullptr);
    }

    shared_ptr(nullptr_t)
    {
        _set_ptr<T>(nullptr);
    }

    template<class Deleter>
    shared_ptr(nullptr_t, Deleter d)
    {
        _set_deleter_ptr<T, Deleter>(nullptr, d);
    }

    template<class Y>
    explicit shared_ptr(Y* ptr)
    {
        _set_ptr<Y>(ptr);
    }

    template< class Y, class Deleter >
    shared_ptr(Y* ptr, Deleter d)
    {
        _set_deleter_ptr<Y, Deleter>(ptr, d);
    }

    template< class Y >
    shared_ptr(const shared_ptr<Y>& other, T *ptr)
    {
        _obj_ptr = ptr;
        _ref_cntr = other._ref_cntr;

        _ref_cntr->inc_shared();
    }

    shared_ptr(const shared_ptr<T> & other)
    {
        _obj_ptr = other._obj_ptr;
        _ref_cntr = other._ref_cntr;

        _ref_cntr->inc_shared();
    }

    shared_ptr(shared_ptr<T> && other)
    {
        _obj_ptr = other._obj_ptr;
        _ref_cntr = other._ref_cntr;

        other._reset_no_delete();
    }

    shared_ptr<T> & operator=(const shared_ptr<T> & other)
    {
        _obj_ptr = other._obj_ptr;
        _ref_cntr = other._ref_cntr;

        _ref_cntr->inc_shared();

        return *this;
    }

    template<class U>
    shared_ptr(const shared_ptr<U> & other)
    {
        _obj_ptr = other._obj_ptr;
        _ref_cntr = other._ref_cntr;

        _ref_cntr->inc_shared();
    }

    template<class U>
    shared_ptr(shared_ptr<U> && other)
    {
        _obj_ptr = other._obj_ptr;
        _ref_cntr = other._ref_cntr;

        other._reset_no_delete();
    }

    template<class U>
    shared_ptr<T> & operator=(const shared_ptr<U> & other)
    {
        _obj_ptr = other._obj_ptr;
        _ref_cntr = other._ref_cntr;

        _ref_cntr->inc_shared();

        return *this;
    }

    shared_ptr(const weak_ptr<T> & ptr)
    {
        _obj_ptr = ptr._obj_ptr;
        _ref_cntr = ptr._ref_cntr;

        _ref_cntr->inc_shared();
    }

    ~shared_ptr()
    {
        _check_delete_ptr();
    }

    /*
    * replaces the managed object
    * If shared_ptr already owns an object, it decreases reference count, and, if it's the last _obj_ptr owns this object, destroys object
    */

    void reset()
    {
        _check_delete_ptr();
    }

    template<class U>
    void reset(U* ptr)
    {
        _check_delete_ptr();
        _set_ptr(ptr);
    }

    /*
    * swaps the managed objects
    */

    void swap(shared_ptr<T> & other)
    {
        std::swap(_obj_ptr, other._obj_ptr);
        std::swap(_ref_counter, other._ref_counter);
    }

    /*
    * returns a _obj_ptr to the managed object
    */

    T* get() const
    {
        return _obj_ptr;
    }

    /*
    * dereferences _obj_ptr to the managed object
    */

    T& operator*() const
    {
        return *_obj_ptr;
    }

    T* operator->() const
    {
        return _obj_ptr;
    }

    /*
    * returns the number of shared_ptr objects referring to the same managed object
    */

    long use_count() const
    {
        if (_ref_cntr == nullptr)
        {
            return 0;
        }

        return _ref_cntr->use_count();
    }

    /*
    * checks whether the managed object is managed only by the current shared_ptr instance
    */

    bool unique() const
    {
        return use_count() == 1;
    }

    /*
    * checks if there is associated managed object
    */

    explicit operator bool() const
    {
        return get() != nullptr;
    }

    /*
    * comparison operators
    */

    template <class U>
    bool operator==(const shared_ptr<U>& other) const
    {
        return get() == other.get();
    }

    template <class U>
    bool operator!=(const shared_ptr<U>& other) const
    {
        return !(*this == other);
    }

    template <class U>
    bool operator<(const shared_ptr<U>& other) const
    {
        return get() < other.get();
    }

    template <class U>
    bool operator>(const shared_ptr<U>& other) const
    {
        return !(*this < other) && *this != other;
    }

    template <class U>
    bool operator<=(const shared_ptr<U>& other) const
    {
        return !(get() > other.get());
    }

    template <class U>
    bool operator>=(const shared_ptr<U>& other) const
    {
        return !(get() < other.get());
    }
};

template<class T>
class weak_ptr
{
private:
    T* _obj_ptr;
    ref_counter* _ref_cntr;

    void _check_delete_ptr()
    {
        _ref_cntr->dec_weak();



        if (_ref_cntr->shared_refs() == 0 &&
            _ref_cntr->weak_refs() == 0)
        {
            delete _ref_cntr;
        }

        _reset_no_delete();
    }

    void _reset_no_delete()
    {
        _obj_ptr = nullptr;
        _ref_cntr = nullptr;
    }

    template<class U>
    friend class shared_ptr;

public:
    weak_ptr() :
        _obj_ptr(nullptr),
        _ref_cntr(nullptr)
    {
    }

    weak_ptr(const shared_ptr<T> & ptr) :
        _obj_ptr(ptr._obj_ptr),
        _ref_cntr(ptr._ref_cntr)
    {
        _ref_cntr->inc_weak();
    }

    weak_ptr<T> & operator=(const weak_ptr<T> & other)
    {
        _obj_ptr = other._obj_ptr;
        _ref_cntr = other._ref_cntr;

        _ref_cntr->inc_weak();

        return *this;
    }

    template<class U>
    weak_ptr(const shared_ptr<U> & ptr) :
        _obj_ptr(ptr._obj_ptr),
        _ref_cntr(ptr._ref_cntr)
    {
        _ref_cntr->inc_weak();
    }

    template<class U>
    weak_ptr<T> & operator=(const weak_ptr<U> & other)
    {
        _obj_ptr = other._obj_ptr;
        _ref_cntr = other._ref_cntr;

        _ref_cntr->inc_weak();
    }

    template<class U>
    weak_ptr(const weak_ptr<U> & other)
    {
        *this = other;
    }

    ~weak_ptr()
    {
        _check_delete_ptr();
    }

    /*
    * releases the ownership of the managed object
    */

    void reset()
    {
        _check_delete_ptr();
    }

    /*
    * swaps the managed objects
    */

    void swap(weak_ptr<T> & other)
    {
        std::swap(_obj_ptr, other._obj_ptr);
        std::swap(_ref_cntr, other._ref_cntr);
    }

    /*
    * returns the number of shared_ptr objects that manage the object
    */

    long use_count() const
    {
        if (_ref_cntr == nullptr)
        {
            return 0;
        }

        return _ref_cntr->shared_count();
    }

    /*
    * checks whether the referenced object was already deleted
    */

    bool expired() const
    {
        if (_ref_cntr == nullptr)
        {
            return true;
        }

        return _ref_cntr->shared_refs() == 0;
    }

    /*
    * creates a shared_ptr that manages the referenced object
    */

    shared_ptr<T> lock() const
    {
        if (expired())
        {
            return shared_ptr<T>();
        }

        return shared_ptr<T>(*this);
    }
};

template<class T, class ... Args>
shared_ptr<T> make_shared(Args && ... args)
{
    shared_ptr<T> ptr;

    auto cntr = new object_and_block<T>(args...);
    ptr._reset_ref_counter(&cntr->object, cntr);

    return ptr;
}

template<class T>
std::ostream & operator<<(std::ostream & stream, const shared_ptr<T> & ptr)
{
    stream << ptr.get();
    return stream;
}

template< class T, class U >
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r)
{
    if (!r)
    {
        return shared_ptr<T>();
    }

    return shared_ptr<T>(static_cast<T*>(r.get()));
}

template< class T, class U >
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r)
{
    if (!r)
    {
        return shared_ptr<T>();
    }

    return shared_ptr<T>(static_cast<T*>(r.get()));
}

template< class T, class U >
shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r)
{
    if (!r)
    {
        return shared_ptr<T>();
    }

    return shared_ptr<T>(const_cast<T*>(r.get()));
}

template<class Deleter, class T>
Deleter* get_deleter(const shared_ptr<T>& p)
{
    ref_counter* cntr = p._ref_cntr;
    typedef std::remove_pointer<typename function_traits<Deleter>::arg<0>::type>::type Y;
    auto ref_del_cntr = dynamic_cast<ptr_and_deleter<Y, Deleter>*>(cntr);
    if (!ref_del_cntr)
    {
        return nullptr;
    }

    return &ref_del_cntr->_deleter;
}

namespace std
{
    template<class T>
    struct hash<shared_ptr<T>>
    {
        size_t operator()(const shared_ptr<T> & ptr) const
        {
            return std::hash<T*>()(ptr.get());
        }
    };
}
