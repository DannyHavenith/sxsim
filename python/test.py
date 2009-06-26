import pysix
import curses
stdscr = curses.initscr()

listing = pysix.ParseListingFile("../test/thermostat.lst")
sx = pysix.Simulator()
sx.load_rom( listing)

for x in range(10000):
	sx.run(100)
	stdscr.addstr( 0, 0,  str( sx.get_state().ram))
	stdscr.refresh()

