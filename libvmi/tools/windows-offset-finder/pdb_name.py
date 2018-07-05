import os
import sys
if __name__ == "__main__":
	if len(sys.argv) < 2:
		print "usage: python pdb_name domain_name"
	print sys.argv[1]+".pdb"
