import sys
import pysix

# load a listing file
if len(sys.argv) > 1:
	filename = sys.argv[1]
else:
	filename = "../../test/thermostat.lst"

listing = pysix.ParseListingFile( filename)

# create an emulator
sx = pysix.Emulator()

# load the listings program bytes into the sx rom
sx.load_rom( listing)


def test():
		sx.run(1000000)

if __name__=='__main__':
    from timeit import Timer
    t = Timer("test()", "from __main__ import test")
    print t.repeat(3, 100)
		
