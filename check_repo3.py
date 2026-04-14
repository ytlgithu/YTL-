import sqlite3
conn = sqlite3.connect('instance/blog.db')
cursor = conn.cursor()

# Check repo 3
cursor.execute("SELECT id, name, user_id FROM repos WHERE id=3")
repo = cursor.fetchone()
print(f"Repo 3: {repo}")

# Check files in repo 3
cursor.execute("SELECT path FROM repo_files WHERE repo_id=3")
paths = cursor.fetchall()
print(f"\nFiles in repo 3 ({len(paths)}):")
for p in paths:
    print(f"  {p[0]}")

conn.close()