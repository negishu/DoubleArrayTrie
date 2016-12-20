#pragma once
#include <vector>
class BaseDoubleArrayTrie
{
public:
    BaseDoubleArrayTrie();
    virtual ~BaseDoubleArrayTrie(void);
    const void* GetArray() { return _array_; };
    const unsigned int GetSize() { return _size_; };
    struct unit_ {
      int          base;
      unsigned int check;
    };
    int Lookup(const unsigned char *key, int len, int *result);
protected:
    int Lookup(const unit_ *array, const unsigned char *key, int len, int *result);
    struct node_ {
        unsigned int code;
        unsigned int depth;
        unsigned int left;
        unsigned int right;
    };
    virtual unsigned int Fetch(const node_ &parent, std::vector <node_> &siblings) = 0;
    template <class T> inline T max(T x, T y) { return(x > y) ? x : y; }
    unsigned int Resize(unsigned int resize);
    unsigned int Insert(const std::vector <node_> &siblings);
    unsigned int           _size_;
    unsigned int _next_check_pos_;
    const int*    _values_;
    unit_ *_array_;
    inline unit_* _resize_unit_(unit_* ptr, unsigned int n, unsigned int l, unit_ v) {
        unit_ *tmp = new unit_[l];
        for (size_t i = 0; i < n; ++i) tmp[i] = ptr[i];
        for (size_t i = n; i < l; ++i) tmp[i] = v;
        delete [] ptr;
        return tmp;
    }
    unsigned char *_used_;
    inline unsigned char* _resize_used_(unsigned char* ptr, unsigned int n, unsigned int l, unsigned char v) {
        unsigned char *tmp = new unsigned char[l];
        for (size_t i = 0; i < n; ++i) tmp[i] = ptr[i];
        for (size_t i = n; i < l; ++i) tmp[i] = v;
        delete [] ptr;
        return tmp;
    }
    unsigned int _alloc_size_;
    unsigned int _progress_;
    unsigned int _Code_LowRange_;
    unsigned int _Code_HighRange_;
};

class DoubleArrayTrie : public BaseDoubleArrayTrie
{
public:
    DoubleArrayTrie() {
    }
    virtual bool Build(const char ** keys, const int* values, unsigned int size);
protected:
    const char ** _keys_;
    virtual unsigned int Fetch(const node_ &parent, std::vector <node_> &siblings);
};
