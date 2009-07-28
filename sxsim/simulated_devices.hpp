
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIMULATED_DEVICES_HPP
#define SIMULATED_DEVICES_HPP
#include <map>
#include <string>
#include <iostream>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

class sx_device
{
public:

    friend std::ostream &operator<<( std::ostream &strm, const sx_device &d) 
    {
        return strm << d.to_string();
    }

    template< typename T>
    void set_value( const std::string &portname, T value)
    {
        // todo: check more efficiently for existence of portname
        if ((ports( &value)[ portname]))
        {
            *(ports( &value)[ portname]) = value;
            if (triggers[portname])
            {
                triggers[portname]();
            }
        }
    }

protected:
    virtual std::string to_string() const
    {
        return "null device";
    }

    template< typename T>
    void declare_input_port( T *port, const std::string &name)
    {
        ports( port)[name] = port;
    }

    template< typename T>
    void declare_input_port( T *port, const std::string &name, boost::function< void ()> f)
    {
        declare_input_port( port, name);
        triggers[name] = f;
    }

private:
    std::map< std::string, unsigned char *> &ports( unsigned char *) 
    {
        return byte_ports;
    }

    std::map< std::string, bool *> &ports( bool *)
    {
        return bit_ports;
    }

    std::map< std::string, unsigned char *> byte_ports;
    std::map< std::string, bool *> bit_ports;
    std::map< std::string, boost::function< void ()> > triggers;
};

boost::shared_ptr<sx_device> create_device( const std::string &type);

#endif //SIMULATED_DEVICES_HPP