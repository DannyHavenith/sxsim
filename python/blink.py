from pysix import ParseListingFile, Simulator
import sys

listing = ParseListingFile( "../../test/blink.lst")
sx = Simulator()

sx.load_rom( listing)

#callback that shows the status of bit 1 of the changed value
def ShowLedStatus( address, value):
	if value & 0x02 :
		sys.stdout.write( "ON \r")
	else:
		sys.stdout.write( "OFF\r");

# trigger an action if rb changes		
RB = 0x06		
sx.on_memory_access( RB, ShowLedStatus)

sx.run( 100000000) # run 100 Mcycles, about 2 seconds