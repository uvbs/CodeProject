// exception.h (2016-05-18)
// Yan Gaofeng (yangaofeng@360.cn)

#include <exception>

namespace serializer {

class exception : public std::exception 
{
    public :
        exception(const char* message) 
            : std::exception() 
        { message_ = message; }

        ~exception() throw() {}

        const char* what() const throw() { return message_; }

    private :
        const char* message_;
};

}
