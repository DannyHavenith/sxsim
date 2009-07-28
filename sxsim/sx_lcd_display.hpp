
//          Copyright Danny Havenith 2006 - 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SX_LCD_DISPLAY_HPP
#define SX_LCD_DISPLAY_HPP

#include <algorithm>
#include <boost/bind.hpp>
#include <iostream>

#include "simulated_devices.hpp"

class lcd_display : public sx_device
{
public:
    lcd_display()
        : previous_e(false), four_bit(false), 
        expect_low_word( false), e(false),
        last_word(0), assembled_word(0),
        cursor_position(0)
    {
        clear_display();
        declare_input_port( &last_word, "db");
        declare_input_port( &r_not_w,   "r/!w");
        declare_input_port( &rs,        "rs");
        declare_input_port( &e, "e", boost::bind( &lcd_display::e_changed, this));
    }

    void e_changed( )
    {
        if (!e && previous_e)
        {
            assemble_word();
        }
        previous_e = e;
    }

    virtual std::string to_string() const
    {
        return 
            std::string( display_ram, display_ram + 20) + '\n' +
            std::string( display_ram + 64, display_ram + 84) + '\n' +
            std::string( display_ram + 20, display_ram + 40) + '\n' +
            std::string( display_ram + 84, display_ram + 104) + '\n';
    }

private:

    /// if in four-bit mode, assemble an 8-bit word before handling
    void assemble_word()
    {
        if (four_bit)
        {
            if (expect_low_word)
            {
                assembled_word |= last_word >> 4;
                expect_low_word = false;
                latch_data();
            }
            else
            {
                assembled_word = last_word & 0xF0;
                expect_low_word = true;
            }
        }
        else
        {
            assembled_word = last_word;
            latch_data();
        }
    }

    void latch_data()
    {
        if (!rs)
        {
            execute_command();
        }
        else
        {
            // note: reading ram locations is not implemented yet.
            if (!r_not_w)
            {
                // Write to ram
                display_ram[cursor_position++] = assembled_word;
                cursor_position %= display_ram_size;
            }
        }
    }

    void execute_command()
    {
        if (!r_not_w)
        {
            if (assembled_word == 1)
            {
                clear_display();
            }
            else if( (assembled_word>>1) == 1)
            {
                set_cursor(0);
            }
            else if( (assembled_word>>5) ==1)
            {
                if ( assembled_word & 0x10)
                {
                    four_bit = false;
                }
                else
                {
                    expect_low_word = !four_bit;
                    four_bit = true;
                }
            }
            else if ((assembled_word>>7) == 1)
            {
                set_cursor( assembled_word & 0xef);
            }
        }
    }

    void clear_display()
    {
        std::fill( display_ram, display_ram + display_ram_size, ' ');
        set_cursor(0);
    }

    void set_cursor( int position)
    {
        cursor_position = position;
    }

    static const int display_ram_size = 128;

    bool            previous_e;
    bool            four_bit;
    bool            expect_low_word;
    bool		    reset;
    bool            rs;
    bool            r_not_w;
    bool            e;

    unsigned char	last_word;
    unsigned char   assembled_word;
    int             cursor_position;
    unsigned char	display_ram[display_ram_size];
};

namespace {
    struct xlcd_display_tester
    {
        xlcd_display_tester()
        {
            lcd.set_value( "r/!w", false);
            lcd.set_value( "rs", false);
            write_data( 0x20);

            lcd.set_value( "rs", true);
            write_data('h');
            write_data('e');
            write_data('l');
            write_data('l');
            write_data('o');
            write_data(',');
            write_data(' ');
            write_data('w');
            write_data('o');
            write_data('r');
            write_data('l');
            write_data('d');

            std::cout << lcd;
        }

        void write_data( unsigned char data)
        {
            lcd.set_value( "db", unsigned char( data & 0xf0));
            lcd.set_value( "e", true);
            lcd.set_value( "e", false);

            lcd.set_value( "db", unsigned char( (data << 4) & 0xf0));
            lcd.set_value( "e", true);
            lcd.set_value( "e", false);
        }


        lcd_display lcd;
    };
}
#endif //SX_LCD_DISPLAY_HPP