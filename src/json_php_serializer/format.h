#ifndef __LIB_SERIALIZER_FORMAT_INT_H__
#define __LIB_SERIALIZER_FORMAT_INT_H__

#include <limits>
#include <stdint.h>

namespace serializer {

class FormatInt {
    public:
        explicit FormatInt(bool value) { size_ = FormatDecimal(value); }
        explicit FormatInt(int32_t value) { size_ = FormatDecimal(value); }
        explicit FormatInt(int64_t value) { size_ = FormatDecimal(value); }
        explicit FormatInt(uint32_t value) { size_ = FormatDecimal(value); }
        explicit FormatInt(uint64_t value) { size_ = FormatDecimal(value); }

        /* 
         * Returns the number of characters written to the output buffer.
         * */
        std::size_t size() const { return size_; }
        std::size_t length() const { return size(); }

        /*
         * Returns a pointer to the output buffer content with terminating null
         * character appended.
         * */
        const char* c_str() const { return buffer_; }

        /*
         * Returns the content of the output buffer as an `std::string`.
         * */
        std::string str() const { return std::string(buffer_, size_); }

    private:
        template<class Int>
        size_t FormatDecimal(Int x) 
        {
            const static char digits[] = "9876543210123456789";
            const char* base = digits + 9;
            const Int radix = 10;
            size_t i = 0;
            bool sign = x < 0;
            do {
                Int mod = x % radix;
                x /= radix;
                buffer_[i++] = base[mod];
            } while (x);
            if (sign)
                buffer_[i++] = '-';
            size_t n = i;
            for (size_t j = 0; j < i; ++j) {
                --i;
                std::swap(buffer_[j], buffer_[i]);
            }
            buffer_[n] = '\0';
            return n;
        }

        size_t FormatDecimal(bool x) 
        {
            buffer_[0] = "01"[x?1:0];
            buffer_[1] = 0;
            return 1;
        }

    private:
        // Buffer should be large enough to hold all digits (digits10 + 1),
        // a sign and a null character.
        enum {BUFFER_SIZE = std::numeric_limits<uint64_t>::digits10 + 3};
        mutable char buffer_[BUFFER_SIZE];
        size_t size_;
};

}

#endif
