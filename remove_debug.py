with open(r'D:\桌面文件\ytl\QClawProject\博客系统\templates\repo_view.html', 'r', encoding='utf-8') as f:
    lines = f.readlines()

# Find and remove the debug block (lines 159-168, 0-indexed: 158-167)
# Keep lines before 158 and after 168
new_lines = lines[:158] + lines[168:]

with open(r'D:\桌面文件\ytl\QClawProject\博客系统\templates\repo_view.html', 'w', encoding='utf-8') as f:
    f.writelines(new_lines)

print(f'Done. Removed {len(lines) - len(new_lines)} lines. New total: {len(new_lines)}')
