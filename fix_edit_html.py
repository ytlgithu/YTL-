import re

with open(r'D:\桌面文件\ytl\QClawProject\博客系统\templates\repo_file_edit.html', 'r', encoding='utf-8') as f:
    content = f.read()

# Add breadcrumb CSS
old_css = '''.shortcut-hint kbd {'''
new_css = '''.breadcrumb-edit {
  font-size: 13px;
  color: #57606a;
  margin-bottom: 10px;
  padding: 6px 12px;
  background: #f6f8fa;
  border-radius: 6px;
  display: flex;
  align-items: center;
  flex-wrap: wrap;
  gap: 2px;
}
.breadcrumb-edit a {
  color: #0969da;
  text-decoration: none;
}
.breadcrumb-edit a:hover {
  text-decoration: underline;
}
.breadcrumb-edit .sep {
  margin: 0 3px;
  color: #8c959f;
}
.breadcrumb-edit .current {
  color: #24292f;
}

.shortcut-hint kbd {'''

content = content.replace(old_css, new_css)

# Add breadcrumb HTML after <div class="page-header">
old_html = '''<div class="page-header">
  <h1>✏️ 编辑文件</h1>'''

new_html = '''<div class="page-header">
  <h1>✏️ 编辑文件</h1>
  <div class="breadcrumb-edit">
    <a href="{{ url_for('repo_view', repo_id=repo.id) }}">{{ repo.name }}</a>
    {% if breadcrumbs %}
      {% for crumb in breadcrumbs %}
        <span class="sep">/</span>
        <a href="{{ url_for('repo_view', repo_id=repo.id, subpath=crumb.path) }}">{{ crumb.name }}</a>
      {% endfor %}
    {% endif %}
    <span class="sep">/</span>
    <span class="current">{{ rf.filename }}</span>
  </div>'''

if old_html in content:
    content = content.replace(old_html, new_html)
    print('Breadcrumb HTML added!')
else:
    print('HTML pattern not found!')
    idx = content.find('<div class="page-header">')
    if idx >= 0:
        print(repr(content[idx:idx+200]))

with open(r'D:\桌面文件\ytl\QClawProject\博客系统\templates\repo_file_edit.html', 'w', encoding='utf-8') as f:
    f.write(content)
print('Done!')
