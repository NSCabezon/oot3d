#!/usr/bin/env python3
"""Recreate uncommitted ASM splits and data tails from the user's extracted game."""
import argparse
import json
from pathlib import Path


def statement_size(line):
    text = line.split(';', 1)[0].strip()
    if not text:
        return 0
    if not line.startswith(('\t', ' ')):
        text = text.partition(' ')[2].strip()
    if not text:
        return 0
    op = text.split()[0]
    if op in ('EXPORT', 'IMPORT', 'EXTERN', 'END', 'AREA'):
        return 0
    if op in ('DCB', 'DCW', 'DCD'):
        return {'DCB': 1, 'DCW': 2, 'DCD': 4}[op] * len(text[3:].split(','))
    return 4  # ARM instruction or DCI


def source_body(path, name):
    lines = path.read_text().splitlines()
    body = lines[lines.index(name) + 1:]
    while body and body[-1].strip() in ('', 'END'):
        body.pop()
    return body


def split_body(lines, recipe):
    start, end = recipe['offset'], recipe['offset'] + recipe['size']
    pc, result = 0, []
    for line in lines:
        size = statement_size(line)
        if size == 0:
            if start <= pc < end and line.strip() != recipe['name']:
                result.append(line)
        elif start <= pc and pc + size <= end:
            result.append(line)
        elif pc < end and start < pc + size:
            if not line.strip().startswith('DCB '):
                raise ValueError('Split crosses an instruction: ' + recipe['name'])
            values = line.strip()[3:].split(',')
            result.append('\tDCB ' + ', '.join(v.strip() for v in values[max(start - pc, 0):min(end - pc, size)]))
        pc += size
    if pc < end or sum(statement_size(x) for x in result) != recipe['size']:
        raise ValueError('Source span does not cover ' + recipe['name'])
    first = next((i for i, line in enumerate(result) if line.startswith('\t') and line.strip()), None)
    if first is not None and result[first].strip().startswith('DCB '):
        values = [v.strip() for v in result[first].strip()[3:].split(';', 1)[0].split(',')]
        if len(values) >= 4:
            word = sum(int(v, 0) << (8 * j) for j, v in enumerate(values[:4]))
            result[first:first + 1] = ['\tDCI 0x%08x' % word] + (['\tDCB ' + ', '.join(values[4:])] if len(values) > 4 else [])
    return result


def write_if_changed(path, text):
    path.parent.mkdir(parents=True, exist_ok=True)
    if not path.exists() or path.read_text() != text:
        path.write_text(text)


def regenerate(tree):
    recipes = json.loads((tree / 'tools/local_assets.json').read_text())
    cache = {}
    for recipe in recipes['splits']:
        source, name = recipe['source'], recipe['name']
        if source not in cache:
            cache[source] = source_body(tree / 'disassembly/asm' / (source + '.s'), source)
        body = split_body(cache[source], recipe)
        header = ['\tAREA |i.' + name + '|, CODE, READONLY']
        header += ['\tEXPORT ' + symbol for symbol in recipe['exports']]
        write_if_changed(tree / 'split/asm' / (name + '.s'), '\n'.join(header + [name] + body) + '\n\n\tEND\n')
    rom = (tree / 'disassembly/baserom/exefs/code.bin').read_bytes()
    for recipe in recipes['data']:
        offset = recipe['address'] - 0x100000
        data = rom[offset:offset + recipe['size']]
        if offset < 0 or len(data) != recipe['size']:
            raise ValueError('Extracted code.bin does not cover ' + recipe['path'])
        lines = ['\tAREA |' + recipe['area'] + '|, DATA, READWRITE', '']
        lines += ['\tDCB ' + ', '.join('0x%02x' % b for b in data[i:i + 16]) for i in range(0, len(data), 16)]
        write_if_changed(tree / recipe['path'], '\n'.join(lines) + '\n\n\tEND\n')
    print('Regenerated %d ASM splits and %d data tails locally.' % (len(recipes['splits']), len(recipes['data'])))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('tree', nargs='?', type=Path, default=Path(__file__).resolve().parent.parent)
    args = parser.parse_args()
    regenerate(args.tree.resolve())
