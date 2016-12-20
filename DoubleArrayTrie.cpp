#include "DoubleArrayTrie.h"

BaseDoubleArrayTrie::BaseDoubleArrayTrie(void):_progress_(0),_array_(0),_used_(0),_alloc_size_(0),_values_(0),_Code_LowRange_(0),_Code_HighRange_(255)
{
}

BaseDoubleArrayTrie::~BaseDoubleArrayTrie(void)
{
    delete [] _used_;
    delete [] _array_;
}

unsigned int BaseDoubleArrayTrie::Resize(unsigned int new_size)
{
    unit_ tmp = {0, 0};
    _array_ = _resize_unit_(_array_, _alloc_size_, new_size, tmp);
    _used_  = _resize_used_(_used_,  _alloc_size_, new_size, 0);
    _alloc_size_ = new_size;
    return new_size;
}

unsigned int BaseDoubleArrayTrie::Insert(const std::vector <node_> &siblings)
{
    unsigned int begin = 0;
    unsigned int pos   = max((siblings[0].code + 1), _next_check_pos_) - 1;
    unsigned int nonzero_num = 0;

    int first = 0;
    if (_alloc_size_ <= pos) {
        Resize(pos + 1);
    }

    while (true) {
    next:
        ++pos;
        if (_alloc_size_ <= pos) {
            Resize(pos + 1);
        }

        if (_array_[pos].check) {
            ++nonzero_num;
            continue;
        }
        else if (!first) {
            _next_check_pos_ = pos;
            first = 1;
        }

        begin = pos - siblings[0].code;

        if (_alloc_size_ <= (begin + siblings[siblings.size()-1].code)) {
            Resize((unsigned int)(_alloc_size_ * max(1.05, 1.0 * _size_ / _progress_)));
        }

        if (_used_[begin]) continue;

        for (size_t i = 1; i < siblings.size(); ++i) {
            if (_array_[begin + siblings[i].code].check != 0) {
                goto next;
            }
        }
        break;
    }

    if (1.0 * nonzero_num/(pos - _next_check_pos_ + 1) >= 0.95) {
        _next_check_pos_ = pos;
    }

    _used_[begin] = 1;
    _size_ = max(_size_, (begin + siblings[siblings.size() - 1].code + 1));

    for (unsigned int i = 0; i < siblings.size(); ++i) {
        _array_[begin + siblings[i].code].check = begin;
    }

    for (unsigned int i = 0; i < siblings.size(); ++i) {
        std::vector<node_> new_siblings;
        if (!Fetch(siblings[i], new_siblings)) {
            _array_[begin + siblings[i].code].base = (-_values_[siblings[i].left]-1);
            if (_array_[begin + siblings[i].code].base >= 0) {
                return 0;
            }
            ++_progress_;
        }
        else {
            unsigned int h = Insert(new_siblings);
            _array_[begin + siblings[i].code].base = h;
        }
    }
    return begin;
}

int BaseDoubleArrayTrie::Lookup(const unsigned char *key, int len, int *result)
{
    return Lookup((const BaseDoubleArrayTrie::unit_ *)(GetArray()), key, len, result);
}

int BaseDoubleArrayTrie::Lookup(const unit_ *array, const unsigned char *key, int len, int *result)
{
    int seekto = 0;
    int n = 0;
    int b = array[0].base;
    unsigned int p = 0;
    unsigned int num = 0;
    *result = -1;
    for (int i = 0; i < len; ++i) {
        p = b;
        n = array[p].base;
        if (static_cast<unsigned int>(b) == array[p].check && n < 0) {
            seekto = i;
            *result = -n - 1;
            ++num;
        }
        unsigned int nKey = key[i];
        p = b + static_cast<unsigned char>(nKey + 1);
        if (static_cast<unsigned int>(b) == array[p].check) {
            b = array[p].base;
        }
        else {
            return seekto;
        }
    }
    p = b;
    n = array[p].base;
    if (static_cast<unsigned int>(b) == array[p].check && n < 0) {
        seekto = len;
        *result = -n - 1;
    }
    return seekto;
}

bool DoubleArrayTrie::Build(const char ** keys, const int* values, unsigned int size)
{
    _keys_   = keys;
    _values_ = values;
    _size_   = size;
    Resize(1024 * 8);
    _array_[0].base  = 1;
    _next_check_pos_ = 0;
    node_ root_node;
    root_node.code  = 0;
    root_node.left  = 0;
    root_node.right = size;
    root_node.depth = 0;
    std::vector<node_> siblings;
    if (Fetch(root_node, siblings) == 0) {
        return false;
    }
    Insert(siblings);
    return true;
}

unsigned int DoubleArrayTrie::Fetch(const node_ &parent, std::vector <node_> &siblings)
{
    unsigned int prev_code = 0;
    for (size_t i = parent.left; i < parent.right; ++i) {
        size_t len = strlen(_keys_[i]);
        if (len < parent.depth) {
            continue;
        }
        const unsigned char *tmp = (const unsigned char*)_keys_[i];
        unsigned int cur_code = 0;
        if (len != parent.depth) {
            cur_code = tmp[parent.depth] + 1;
        }
        if (prev_code > cur_code) {
            return 0;
        }
        if (cur_code != prev_code || siblings.empty()) {
            node_ tmp_node;
            tmp_node.depth = parent.depth + 1;
            tmp_node.code  = cur_code;
            tmp_node.left  = i;
            if (!siblings.empty()) {
                siblings[siblings.size() - 1].right = i;
            }
            siblings.push_back(tmp_node);
        }
        prev_code = cur_code;
    }

    if (!siblings.empty()) {
        siblings[siblings.size()-1].right = parent.right;
    }

    return siblings.size();
}
