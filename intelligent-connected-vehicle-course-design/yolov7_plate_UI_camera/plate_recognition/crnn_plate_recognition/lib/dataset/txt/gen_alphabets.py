from pathlib import Path


a = 'chars_5990.txt'

with open(a, encoding='utf-8') as f:
    s = f.readlines()

aa = ''
for ss in s:
    aa += ss.strip()

Path('a.txt').write_text(aa, encoding='utf-8')
