#include "simulated_devices.hpp"
#include "sx_lcd_display.hpp"

boost::shared_ptr<sx_device> create_device( const std::string &type)
{
    typedef boost::shared_ptr<sx_device> ptr_type;
    if (type == "lcd")
    {
        return ptr_type( new lcd_display());
    }
    else
    {
        return ptr_type( new sx_device());
    }
}
