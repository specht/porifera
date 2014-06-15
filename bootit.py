import sys

fname = sys.argv[1]

with open(fname, "rb") as f:
    sector_size = len(f.read(512))

    if sector_size > 510:
        print("Error: Boot sector can't exceed 510 bytes")
        sys.exit(1)
    else:
        pad_size = 510 - sector_size

with open(fname, "ab") as f:
    write_str = b''
    for i in range(0, pad_size):
        write_str += b'\x00'

    write_str += b'\x55\xaa'
    f.write(write_str)

    
