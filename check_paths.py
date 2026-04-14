import sys; sys.path.insert(0,'.')
from app import app, db
c = app.app_context()
c.push()

# 查 custom 前缀的文件
r = db.session.execute(db.text(
    "SELECT path FROM repo_files WHERE repo_id=2 AND path LIKE '1152x576_fan_img/custom%' LIMIT 10"
)).fetchall()
print(f'1152x576_fan_img/custom/: {len(r)} results')
for x in r: print(' ', x[0])

print()

# 查 ui_builder/custom 前缀的文件
r2 = db.session.execute(db.text(
    "SELECT path FROM repo_files WHERE repo_id=2 AND path LIKE '1152x576_fan_img/ui_builder/custom%' LIMIT 10"
)).fetchall()
print(f'1152x576_fan_img/ui_builder/custom/: {len(r2)} results')
for x in r2: print(' ', x[0])
