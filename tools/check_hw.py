import glob
import re
import sys

HW_DIRS = ['core', 'cpu', 'memory', 'bus', 'io', 'system']
PRIMITIVES = 'core/gates/'
RULES = {
    'control flow': r'\b(if|for|while|switch|do|goto)\b\s*[({]',
    'comparison/logic': r'==|!=|<=|>=|&&|\|\||<<|>>|\?|\+\+|--|[-+*/%&|^]=(?!=)',
    'bitwise': r'[\w\)\]]\s*[&|^]\s*[\w(]',
    'arithmetic': r'[\w\)\]]\s*[+/%]\s*[\w(]|[\w\)\]]\s*-\s*[\w(]',
    'unary': r'(^|[^\w\s)\]])\s*[!~]\s*[\w(]',
    'relational': r'[\w\)\]]\s*[<>]\s*[\w(]',
}


def clean(src):
    src = re.sub(r'/\*.*?\*/', lambda m: '\n' * m.group(0).count('\n'), src, flags=re.S)
    lines = []
    for line in src.split('\n'):
        line = re.sub(r'//.*', '', line)
        line = re.sub(r'"(\\.|[^"\\])*"', '""', line)
        line = re.sub(r"'(\\.|[^'\\])*'", "''", line)
        line = re.sub(r'\breturn\s*[&*]', 'return ', line)
        lines.append('' if line.strip().startswith('#') else line)
    return lines


def violations(path):
    for n, line in enumerate(clean(open(path).read()), 1):
        for rule, pattern in RULES.items():
            if re.search(pattern, line):
                yield f'{path}:{n}: {rule}: {line.strip()}'


def main():
    files = sorted(f for d in HW_DIRS for ext in ('*.c', '*.h')
                   for f in glob.glob(f'{d}/**/{ext}', recursive=True)
                   if not f.startswith(PRIMITIVES))
    found = [v for f in files for v in violations(f)]
    for v in found:
        print(v)
    print(f'{len(files)} files checked, {len(found)} violations')
    return 1 if found else 0


sys.exit(main())
