import os

BASE_DIR = os.path.abspath(os.path.dirname(__file__))

class Config:
    SECRET_KEY = 'ytl-blog-secret-2026'
    SQLALCHEMY_DATABASE_URI = 'sqlite:///' + os.path.join(BASE_DIR, 'instance', 'blog.db')
    SQLALCHEMY_TRACK_MODIFICATIONS = False
    UPLOAD_FOLDER = os.path.join(BASE_DIR, 'uploads')
    MAX_CONTENT_LENGTH = 2 * 1024 * 1024 * 1024  # 2GB
    MAX_FORM_MEMORY_SIZE = 2 * 1024 * 1024 * 1024  # 2GB
    # 允许所有扩展名
    ALLOWED_EXTENSIONS = {
        'c', 'h', 'cpp', 'hpp', 'cc', 's', 'asm',
        'bin', 'img', 'hex', 'elf', 'fw',
        'py', 'js', 'ts', 'html', 'css', 'json', 'xml', 'yaml', 'yml',
        'md', 'txt', 'sh', 'bat', 'ps1',
        'zip', 'tar', 'gz', '7z',
        'pdf', 'doc', 'docx', 'xls', 'xlsx',
        'png', 'jpg', 'jpeg', 'gif', 'svg',
        'cmake', 'makefile', 'ini', 'cfg', 'conf', 'log',
    }
