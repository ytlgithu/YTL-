import sqlite3
import os

db_path = r'D:\桌面文件\ytl\QClawProject\博客系统\instance\blog.db'
print(f"DB exists: {os.path.exists(db_path)}")

conn = sqlite3.connect(db_path)
cur = conn.cursor()

# 列出所有表
cur.execute("SELECT name FROM sqlite_master WHERE type='table'")
tables = [r[0] for r in cur.fetchall()]
print(f"Tables: {tables}")

if 'posts' in tables:
    cur.execute("PRAGMA table_info(posts)")
    cols = cur.fetchall()
    print(f"Posts columns: {[c[1] for c in cols]}")
    
    # 添加 is_public 列
    col_names = [c[1] for c in cols]
    if 'is_public' not in col_names:
        cur.execute("ALTER TABLE posts ADD COLUMN is_public BOOLEAN DEFAULT 1")
        conn.commit()
        print("Added is_public column")
    else:
        print("is_public already exists")
else:
    print("posts table does not exist")

conn.close()
