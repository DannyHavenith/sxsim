import pysix
import curses
stdscr = curses.initscr()

# load a listing file
listing = pysix.ParseListingFile("../test/thermostat.lst")

# create an emulator
sx = pysix.Simulator()

# load the listing program into the sx rom
sx.load_rom( listing)

for x in range(10000): #repeat 10 000 times
	sx.run(100) # do 100 instructions
	stdscr.addstr( 0, 0,  str( sx.state.ram)) # ... and print the ram contents
	stdscr.refresh()

