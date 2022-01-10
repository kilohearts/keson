#pragma once

#include <string>
#include <istream>

#include "conf.h"
#include "node.h"

namespace keson
{
    class ParseError
    {
    public:
        // TODO: Add line and column number to this exception

        ParseError(const std::string& message)
            : _message(message)
        { }

        const std::string& what()
        {
            return _message;
        }

    private:
        std::string _message;
    };

    std::variant<Node, ParseError> decode(std::istream& s);
    
    std::variant<Node, ParseError> decode(const std::string& s);
}
