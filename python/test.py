# simple test script that runs the thermostat firmware, simulates some input and
# writes the program output to screen.

import sys
import pysix

# load a listing file
listing = pysix.ParseListingFile("../../test/thermostat.lst")

# create an emulator
sx = pysix.Emulator()

# load the listings program bytes into the sx rom
sx.load_rom( listing)

# some code labels
send_byte = listing.GetLabelAddress( "send_byte:wait") + 3
after_spi_read = 0x0141

#some data labels
spi_value = 0x08

# the entry point to the send_byte function. At this
# point w will contain a character to be sent to output (LCD/rs-232)
sx.set_breakpoint( send_byte) 

# right after a temperature has been read from the spi device
# ram location spi_value will contain the temperature reading.
sx.set_breakpoint( after_spi_read)

while True:
	sx.run(10000) # do 10 000 instructions, or stop if we hit a breakpoint
	
	if		send_byte		== sx.state.pc:
		sys.stdout.write( chr(sx.state.w))
		
	elif	after_spi_read	== sx.state.pc:
		# a bit convoluted right now. state can only be copied as a whole, not changed
		state = sx.state
		state.ram.set_absolute( spi_value, 100)
		state.ram.set_absolute( spi_value + 1, 0)
		sx.state = state

