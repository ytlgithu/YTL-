"""迁移：为posts表添加is_public字段"""
import sqlite3
import os

db_path = os.path.join(os.path.dirname(__file__), 'instance', 'firmware.db')
print(f"数据库路径: {db_path}")

conn = sqlite3.connect(db_path)
cur = conn.cursor()

# 检查posts表是否存在
cur.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='posts'")
if not cur.fetchone():
    print("posts 表不存在，无需迁移")
    conn.close()
    exit(0)

# 检查列是否存在
cur.execute("PRAGMA table_info(posts)")
columns = [row[1] for row in cur.fetchall()]
print(f"当前列: {columns}")

if 'is_public' not in columns:
    cur.execute("ALTER TABLE posts ADD COLUMN is_public BOOLEAN DEFAULT 1")
    conn.commit()
    print("已添加 is_public 列到 posts 表")
else:
    print("is_public 列已存在")

conn.close()
