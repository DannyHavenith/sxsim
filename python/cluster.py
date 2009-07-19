from pysix import ParseListingFile, Emulator, Cluster
import sys

listing1 = ParseListingFile( "../../test/wireless_transmitter.lst")
listing2 = ParseListingFile( "../../test/wireless_display.lst")
cluster = Cluster()
sx1 = cluster.new_controller();
sx2 = cluster.new_controller();

sx1.load_rom( listing1)
sx2.load_rom( listing2)

def SetSx2Serial( value):
	state = sx2.state
	RA = 0x05
	if value:
		state.ram.set( RA, state.ram(RA) | 0x01)
	else:
		state.ram.set( RA, state.ram(RA) & 0xfe)
	sx2.state = state
	
def TransferSerialLine( address, value):
	#sys.stdout.write('.')
	if (value & 0x02):
		SetSx2Serial( True)
	else:
		SetSx2Serial( False)

def ShowReceivedData( address, value):
	sys.stdout.write( chr( value))


# trigger an action if rb changes		
RB = 0x06		
sx1.on_memory_write( RB, TransferSerialLine)

int_temp = 0x08
#sx2.on_memory_write( int_temp, ShowReceivedData)

send_byte = 0x40F
handle_byte = 0x0027
rx_byte = 0x16

sx2.set_breakpoint( handle_byte)
sx1.set_breakpoint( send_byte)

while True:
	cluster.run( 10000)# run the cluster for a few seconds
	if sx2.state.pc == handle_byte:
		print "received", sx2.state.ram( rx_byte)
	elif sx1.state.pc == send_byte:
		print "sending", sx1.state.w
		

