#ifdef _WIN32
#include "socket.h"
#endif
#include "dbhelper.h"
#include "myassert.h"
#include <stdio.h>
#include "string_util.h"

namespace db_helper
{
    bool  saveUserData(const FullUserData& value, const char* dir)
    {
        char file[256] = {0};
        str_util::snprintf(file, sizeof(file), "%s/USER-%llx.db", dir, value._human_data._guid.value());
        FILE* f = fopen(file, "wb");
        if (f)
        {
            size_t writeSize = 0;
            FILE_DB_WRITE(value._human_data);
            FILE_DB_WRITE(value._bag_data);
            FILE_DB_WRITE(value._equip_data);
            fclose(f);
            return true;
        }
        return false;
    }

}
