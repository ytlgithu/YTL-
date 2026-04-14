"""
数据库迁移脚本：为 products 表创建数据库结构
运行一次即可
"""
import sqlite3, os

db_path = os.path.join(os.path.dirname(__file__), 'instance', 'firmware.db')
conn = sqlite3.connect(db_path)
cursor = conn.cursor()

# 检查 products 表是否存在
cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='products'")
if cursor.fetchone():
    print('[INFO] products 表已存在，无需迁移')
else:
    cursor.execute('''
        CREATE TABLE products (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name VARCHAR(120) NOT NULL UNIQUE,
            description TEXT,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    ''')
    # 为 firmwares 表添加 product_id 列
    cursor.execute("PRAGMA table_info(firmwares)")
    cols = [row[1] for row in cursor.fetchall()]
    if 'product_id' not in cols:
        cursor.execute('ALTER TABLE firmwares ADD COLUMN product_id INTEGER REFERENCES products(id)')
        print('[OK] products 表创建成功，firmwares 表 product_id 列已添加')
    else:
        print('[OK] products 表创建成功')
    conn.commit()

conn.close()
