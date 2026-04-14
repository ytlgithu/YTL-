# -*- coding: utf-8 -*-
"""添加 Markdown 使用教程文章"""

import sys
sys.path.insert(0, '.')

from app import app, db
from models import Post, User, cn_now

MARKDOWN_TUTORIAL = """# Markdown 使用教程

Markdown 是一种轻量级标记语言，让你用简单的语法编写格式化的文档。广泛用于博客、文档、README 等。

## 1. 标题

使用 `#` 号表示标题，数量对应级别：

```markdown
# 一级标题
## 二级标题
### 三级标题
#### 四级标题
##### 五级标题
###### 六级标题
```

## 2. 段落与换行

- 段落之间用**空行**分隔
- 行末加两个空格或 `<br>` 换行

## 3. 文本样式

| 语法 | 效果 |
|------|------|
| `**粗体**` | **粗体** |
| `*斜体*` | *斜体* |
| `***粗斜体***` | ***粗斜体*** |
| `~~删除线~~` | ~~删除线~~ |
| `` `代码` `` | `代码` |

## 4. 列表

### 无序列表

```markdown
- 项目一
- 项目二
  - 嵌套项目
```

效果：
- 项目一
- 项目二
  - 嵌套项目

### 有序列表

```markdown
1. 第一步
2. 第二步
3. 第三步
```

效果：
1. 第一步
2. 第二步
3. 第三步

### 任务列表

```markdown
- [x] 已完成
- [ ] 未完成
```

效果：
- [x] 已完成
- [ ] 未完成

## 5. 链接与图片

### 链接

```markdown
[链接文字](https://example.com)
[带标题的链接](https://example.com "鼠标悬停显示")
```

效果：[QClaw 官网](https://qclaw.cn)

### 图片

```markdown
![图片描述](图片URL)
![本地图片](./images/logo.png)
```

## 6. 引用

```markdown
> 这是一段引用
> 可以多行
>> 还可以嵌套
```

效果：
> 这是一段引用
> 可以多行
>> 还可以嵌套

## 7. 代码块

### 行内代码

使用反引号包裹：`` `print("Hello")` `` → `print("Hello")`

### 代码块

用三个反引号包裹，可指定语言：

````markdown
```python
def hello():
    print("Hello, World!")
```
````

效果：
```python
def hello():
    print("Hello, World!")
```

支持的语言：`python`, `javascript`, `c`, `cpp`, `java`, `bash`, `json`, `html`, `css` 等

## 8. 表格

```markdown
| 列1 | 列2 | 列3 |
|-----|-----|-----|
| 内容 | 内容 | 内容 |
| 左对齐 | 居中 | 右对齐 |
| :--- | :---: | ---: |
```

效果：
| 列1 | 列2 | 列3 |
|-----|:---:|-----|
| 内容 | 内容 | 内容 |
| 左对齐 | 居中 | 右对齐 |

## 9. 分割线

三个或更多的 `-`、`*`、`_`：

```markdown
---
***
___
```

效果：

---

## 10. 转义字符

使用反斜杠 `\` 转义特殊字符：

```markdown
\*不是斜体\*
\[不是链接\]
```

效果：\*不是斜体\*

## 11. HTML 标签

Markdown 中可以直接写 HTML：

```html
<div style="color: red;">红色文字</div>
<kbd>Ctrl</kbd> + <kbd>C</kbd>
```

效果：<kbd>Ctrl</kbd> + <kbd>C</kbd>

---

## 快速参考

| 功能 | 语法 |
|------|------|
| 标题 | `# H1` ~ `###### H6` |
| 粗体 | `**text**` |
| 斜体 | `*text*` |
| 链接 | `[text](url)` |
| 图片 | `![alt](url)` |
| 代码 | `` `code` `` |
| 引用 | `> quote` |
| 列表 | `- item` 或 `1. item` |
| 分割线 | `---` |

---

> 💡 **提示**：不同平台对 Markdown 的支持可能略有差异，建议在目标平台上预览效果。
"""

def main():
    with app.app_context():
        # 查找杨天龙用户
        user = User.query.filter_by(username='杨天龙').first()
        if not user:
            print("未找到用户 '杨天龙'")
            # 列出所有用户
            users = User.query.all()
            print("现有用户：", [u.username for u in users])
            return
        
        # 检查是否已存在该文章
        existing = Post.query.filter_by(title='Markdown 使用教程').first()
        if existing:
            print(f"文章已存在，ID: {existing.id}")
            return
        
        # 创建文章
        post = Post(
            title='Markdown 使用教程',
            content=MARKDOWN_TUTORIAL,
            summary='Markdown 是一种轻量级标记语言，本文介绍标题、列表、链接、代码块、表格等常用语法。',
            user_id=user.id,
            is_public=True
        )
        db.session.add(post)
        db.session.commit()
        print(f"文章创建成功！ID: {post.id}, 作者: {user.username}")

if __name__ == '__main__':
    main()
