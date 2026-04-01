import re
from dataclasses import dataclass

# --------------------------------------------------
# CONFIG
# --------------------------------------------------

FRAM_SIZE = 512 * 1024        # 512 KB
FRAM_RESERVED = 4             # 0..3 reserved (magic word)
FRAM_START = FRAM_RESERVED

ALIGNMENT = 4

TYPE_SIZES = {
    "CO_DEFTYPE_UNSIGNED8": 1,
    "CO_DEFTYPE_UNSIGNED16": 2,
    "CO_DEFTYPE_UNSIGNED32": 4,
}

PERSISTENT_ACCESS = "CO_ACCESS_RW"

# --------------------------------------------------

@dataclass
class Entry:
    index: str
    subindex: str
    name: str
    ctype: str
    size: int
    address: int
    padding: int


# --------------------------------------------------
# ALIGNMENT
# --------------------------------------------------

def align(addr):
    return (addr + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1)


# --------------------------------------------------
# PARSER + ADDRESS ASSIGNMENT
# --------------------------------------------------
def parse_struct(text):

    entries = []
    addr = FRAM_START

    block = []
    current_index = None

    idx_pattern = re.compile(r"\.idx\s*=\s*(0x[0-9a-fA-F]+)")
    sub_pattern = re.compile(r"\.subidx\s*=\s*(0x[0-9a-fA-F]+)")
    type_pattern = re.compile(r"\.type\s*=\s*(CO_DEFTYPE_\w+)")
    name_pattern = re.compile(r'\.name\s*=\s*CO_SDEV_STRING\("([^"]+)"\)')

    for line in text.splitlines():

        # index may appear outside block → update immediately
        idx_m = idx_pattern.search(line)
        if idx_m:
            current_index = idx_m.group(1)

        block.append(line)

        # wait for end of initializer
        if not line.strip().startswith("},"):
            continue

        block_text = "\n".join(block)

        sub_m = sub_pattern.search(block_text)
        type_m = type_pattern.search(block_text)
        name_m = name_pattern.search(block_text)

        # must have inherited index + subindex
        if not (current_index and sub_m and type_m):
            block = []
            continue

        ctype = type_m.group(1)

        if ctype not in TYPE_SIZES:
            block = []
            continue

        name = name_m.group(1) if name_m else "UNKNOWN"

        size = TYPE_SIZES[ctype]

        aligned = align(addr)
        padding = aligned - addr
        addr = aligned

        entries.append(
            Entry(
                current_index,
                sub_m.group(1),
                name,
                ctype,
                size,
                addr,
                padding,
            )
        )

        addr += size
        block = []

    return entries
# --------------------------------------------------
# PATCH STRUCT
# --------------------------------------------------

def inject_fram_addresses(text, entries):

    addr_map = {(e.index, e.subindex): e.address for e in entries}

    idx = None
    sub = None
    out = []

    for line in text.splitlines():

        m = re.search(r"\.idx\s*=\s*(0x[0-9a-fA-F]+)", line)
        if m:
            idx = m.group(1)

        m = re.search(r"\.subidx\s*=\s*(0x[0-9a-fA-F]+)", line)
        if m:
            sub = m.group(1)

            key = (idx, sub)
            if key in addr_map:
                out.append(line)
                out.append(
                    f"            .fram_address = 0x{addr_map[key]:06X},"
                )
                continue

        out.append(line)

    return "\n".join(out)


# --------------------------------------------------
# ASCII MAP
# --------------------------------------------------

def print_ascii_map(entries):

    print("\nFRAM MEMORY MAP")
    print("+" + "-" * 78 + "+")

    print("| 0x000000-0x000003 | RESERVED (MAGIC WORD)                        |")

    for e in entries:

        if e.padding:
            pad_start = e.address - e.padding
            pad_end = e.address - 1
            print(
                f"| 0x{pad_start:06X}-0x{pad_end:06X} | ALIGNMENT PADDING                    |"
            )

        end = e.address + e.size - 1

        print(
            f"| 0x{e.address:06X}-0x{end:06X} "
            f"| {e.index}:{e.subindex} "
            f"| {e.name[:30]:30} |"
        )

    print("+" + "-" * 78 + "+")


# --------------------------------------------------
# FRAM STATISTICS
# --------------------------------------------------

def print_fram_statistics(entries):

    used_data = sum(e.size for e in entries)
    used_padding = sum(e.padding for e in entries)

    used_total = used_data + used_padding + FRAM_RESERVED
    free = FRAM_SIZE - used_total

    print("\nFRAM USAGE STATISTICS")
    print("-" * 45)
    print(f"Total FRAM         : {FRAM_SIZE} Bytes (512 KB)")
    print(f"Reserved (magic)   : {FRAM_RESERVED} Bytes")
    print(f"Data used          : {used_data} Bytes")
    print(f"Alignment padding  : {used_padding} Bytes")
    print(f"---------------------------------------------")
    print(f"Total used         : {used_total} Bytes ({used_total/FRAM_SIZE*100:.3f} %)")
    print(f"Free               : {free} Bytes ({free/FRAM_SIZE*100:.3f} %)")
    print("-" * 45)

    bar_len = 40
    used_bar = int(bar_len * used_total / FRAM_SIZE)

    print(
        "[" +
        "#" * used_bar +
        "-" * (bar_len - used_bar) +
        "]"
    )


# --------------------------------------------------
# MAIN
# --------------------------------------------------

def main():

    with open("input.c", "r") as f:
        text = f.read()

    entries = parse_struct(text)

    patched = inject_fram_addresses(text, entries)

    with open("output_fram.c", "w") as f:
        f.write(patched)

    print_ascii_map(entries)
    print_fram_statistics(entries)

    print("\nPersistent entries:", len(entries))
    print("Output file: output_fram.c")


if __name__ == "__main__":
    main()