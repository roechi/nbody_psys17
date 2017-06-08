//
// Created by Jonas Jaszkowic on 08.06.17.
//

#include "Util.h"

namespace Util {
    /**
    * Read the content of the file given by the filename.
    *
    * @param filename the name of the file which should be read.
    *
    * @return the contents or NULL on an error. The returned
    *  value must be freed with free() when no longer in use.
    */
    std::string read_file_contents(std::string filename)
    {
        std::ifstream ifs(filename);
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                             (std::istreambuf_iterator<char>()    ) );
        return content;
    }
}