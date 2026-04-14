from app import app, init_db, Config
import os

if __name__ == '__main__':
    # 确保目录存在
    os.makedirs(Config.UPLOAD_FOLDER, exist_ok=True)
    os.makedirs(os.path.join(os.path.dirname(__file__), 'instance'), exist_ok=True)
    
    # 初始化数据库
    init_db()
    
    print('启动 Flask 服务器...')
    print('访问地址: http://127.0.0.1:5000')
    print('局域网地址: http://192.168.10.248:5000')
    print(f'MAX_CONTENT_LENGTH: {app.config["MAX_CONTENT_LENGTH"] / 1024 / 1024 / 1024:.1f} GB')
    
    app.run(host='0.0.0.0', port=5000, debug=False, use_reloader=False, threaded=True)
