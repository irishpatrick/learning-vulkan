#!/usr/bin/env python3

import pathlib
import subprocess


def compile():
    files = [p for p in pathlib.Path('.').iterdir() if p.is_file() and p.suffix in ['.glsl', '.vert', '.frag']]
    for file in files:
        print('compile', file)
        subprocess.run(['glslc', str(file), '-o', f'build/{str(file)}.spv'])

def inline():
    files = [p for p in pathlib.Path('./build').iterdir() if p.is_file and p.suffix in ['.spv']]
    for file in files:
        print('inline', file)
        with open(f'{str(file)}.inl', 'w') as fp:
            with open(file, 'rb') as data:
                bytecode = bytearray(data.read())

            varname = file.stem.replace('.', '_')
            size = len(bytecode)
            fp.write(f"constexpr std::array<unsigned char, {size}> {varname}[] = {{ {','.join(str(x) for x in bytecode)} }};")



def main():
    compile()
    inline()

if __name__ == '__main__':
    main()
