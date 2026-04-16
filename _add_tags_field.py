c = open(r'D:\桌面文件\ytl\QClawProject\YTL仓博系统\templates\post_edit.html', encoding='utf-8').read()
# Find the content label area and where to insert tags
ta_idx = c.find('name="content"')
if ta_idx > 0:
    fg_start = c.rfind('<div class="form-group">', 0, ta_idx)
    if fg_start > 0:
        tags_html = '''  <div class="form-group">
    <label>标签</label>
    <input type="text" name="tags" value="{{ post.tags if post and post.tags else '' }}" placeholder="多个标签用逗号分隔，如：Python,Django,全栈开发">
  </div>
'''
        c = c[:fg_start] + tags_html + c[fg_start:]
        open(r'D:\桌面文件\ytl\QClawProject\YTL仓博系统\templates\post_edit.html', 'w', encoding='utf-8').write(c)
        print('Done!')
    else:
        print('Cannot find form-group before content textarea')
else:
    print('Cannot find content textarea')
