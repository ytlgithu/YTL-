import sys; sys.path.insert(0,'.')
from app import app, db
c = app.app_context()
c.push()

# 查所有顶级目录
r = db.session.execute(db.text(
    "SELECT DISTINCT SUBSTR(path, 1, INSTR(path || '/', '/') - 1) as top FROM repo_files WHERE repo_id=2 LIMIT 20"
)).fetchall()
print('Top-level dirs:', [x[0] for x in r])

print()

# 查 custom 相关的所有路径
r2 = db.session.execute(db.text(
    "SELECT path FROM repo_files WHERE repo_id=2 AND path LIKE '%/custom%' LIMIT 15"
)).fetchall()
print('All paths containing /custom/:')
for x in r2: print(' ', x[0])

print()

# 查 driver 相关的路径
r3 = db.session.execute(db.text(
    "SELECT path FROM repo_files WHERE repo_id=2 AND path LIKE '%driver%' LIMIT 10"
)).fetchall()
print('All paths containing driver:')
for x in r3: print(' ', x[0])
