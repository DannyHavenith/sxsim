import pysix

listing = pysix.ParseListingFile( "../../test/thermostat.lst")
print listing.GetLabelAddress( "Initialize"), listing.GetLabelAddress( "Initialize:zero_ram")
