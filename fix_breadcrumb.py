import re

with open(r'D:\桌面文件\ytl\QClawProject\博客系统\templates\repo_view.html', 'r', encoding='utf-8') as f:
    content = f.read()

# Find the breadcrumb section
m = re.search(r'<div class="breadcrumb">.*?</div>', content, re.DOTALL)
if m:
    print("Found breadcrumb section:")
    print(repr(m.group()))
    
    # Replace: remove the {% if subpath %}...{% endif %} block
    old_section = m.group()
    # Remove the extra subpath block (the duplicate last item)
    new_section = re.sub(
        r'\s*{% if subpath %}.*?{% endif %}\n\s*',
        '\n      ',
        old_section,
        flags=re.DOTALL
    )
    print("\nNew section:")
    print(repr(new_section))
    
    if new_section != old_section:
        content = content.replace(old_section, new_section)
        with open(r'D:\桌面文件\ytl\QClawProject\博客系统\templates\repo_view.html', 'w', encoding='utf-8') as f:
            f.write(content)
        print("\nReplaced successfully!")
    else:
        print("\nNo change needed (pattern didn't match)")
else:
    print("Breadcrumb section not found")
