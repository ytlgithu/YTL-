import re

with open(r'D:\桌面文件\ytl\QClawProject\博客系统\app.py', 'r', encoding='utf-8') as f:
    content = f.read()

# Find the render_template line in repo_file_edit and add breadcrumbs before it
old = '''    return render_template('repo_file_edit.html', repo=repo, rf=rf, content=content)'''
new = '''    # 面包屑（动态去掉实际路径前缀）
    breadcrumbs = []
    dirname = rf.dirname
    actual_prefix = rf.path[:rf.path.find('/') + 1] if '/' in rf.path else ''
    if actual_prefix and dirname.startswith(actual_prefix):
        dirname = dirname[len(actual_prefix):]
    if dirname:
        parts = dirname.split('/')
        for i, p in enumerate(parts):
            breadcrumbs.append({'name': p, 'path': '/'.join(parts[:i+1])})

    return render_template('repo_file_edit.html', repo=repo, rf=rf, content=content, breadcrumbs=breadcrumbs)'''

if old in content:
    content = content.replace(old, new, 1)
    with open(r'D:\桌面文件\ytl\QClawProject\博客系统\app.py', 'w', encoding='utf-8') as f:
        f.write(content)
    print('app.py updated!')
else:
    print('Pattern not found!')
    # Try to find it
    idx = content.find("render_template('repo_file_edit.html'")
    if idx >= 0:
        print(f'Found at index {idx}')
        print(repr(content[idx-100:idx+100]))
