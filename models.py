from datetime import datetime, timezone, timedelta
from flask_sqlalchemy import SQLAlchemy
from werkzeug.security import generate_password_hash, check_password_hash

# 东八区时间
CN_TIMEZONE = timezone(timedelta(hours=8))

def cn_now():
    """返回东八区当前时间"""
    return datetime.now(CN_TIMEZONE)

db = SQLAlchemy()


class User(db.Model):
    __tablename__ = 'users'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(80), unique=True, nullable=False, index=True)
    email = db.Column(db.String(120), unique=True, nullable=False, index=True)
    password_hash = db.Column(db.String(256), nullable=False)
    avatar = db.Column(db.String(256), default='')
    is_admin = db.Column(db.Boolean, default=False)
    created_at = db.Column(db.DateTime, default=cn_now)
    last_login = db.Column(db.DateTime)

    repos = db.relationship('Repo', backref='owner', lazy='dynamic', cascade='all, delete-orphan')

    def avatar_url(self, req):
        if self.avatar:
            return req.host_url.rstrip('/') + '/static/avatars/' + self.avatar
        return req.host_url.rstrip('/') + '/static/img/default_avatar.png'

    def check_password(self, password):
        return check_password_hash(self.password_hash, password)


class Repo(db.Model):
    """仓库（类似 GitHub repo）"""
    __tablename__ = 'repos'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(120), nullable=False)
    description = db.Column(db.Text)
    is_public = db.Column(db.Boolean, default=True)
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)
    created_at = db.Column(db.DateTime, default=cn_now, index=True)
    updated_at = db.Column(db.DateTime, default=cn_now, onupdate=cn_now)

    files = db.relationship('RepoFile', backref='repo', lazy='dynamic', cascade='all, delete-orphan')

    @property
    def file_count(self):
        return self.files.count()

    @property
    def total_size(self):
        total = db.session.query(db.func.sum(RepoFile.file_size)).filter_by(repo_id=self.id).scalar() or 0
        return total

    @property
    def total_size_display(self):
        size = self.total_size
        for unit in ['B', 'KB', 'MB', 'GB']:
            if size < 1024:
                return f"{size:.1f} {unit}"
            size /= 1024
        return f"{size:.1f} TB"


class RepoFile(db.Model):
    """仓库中的文件（保留目录结构）"""
    __tablename__ = 'repo_files'
    id = db.Column(db.Integer, primary_key=True)
    repo_id = db.Column(db.Integer, db.ForeignKey('repos.id'), nullable=False)
    # 相对路径，如 src/main.c 或 include/config.h
    path = db.Column(db.String(512), nullable=False)
    # 存储在磁盘上的实际文件名
    stored_name = db.Column(db.String(512), nullable=False)
    file_size = db.Column(db.Integer, default=0)
    md5_hash = db.Column(db.String(32))
    upload_user_id = db.Column(db.Integer, db.ForeignKey('users.id'))
    created_at = db.Column(db.DateTime, default=cn_now)
    updated_at = db.Column(db.DateTime, default=cn_now, onupdate=cn_now)

    uploader = db.relationship('User', foreign_keys=[upload_user_id])

    @property
    def filename(self):
        """文件名（不含路径）"""
        return self.path.split('/')[-1]

    @property
    def dirname(self):
        """所在目录"""
        parts = self.path.split('/')
        return '/'.join(parts[:-1]) if len(parts) > 1 else ''

    @property
    def file_size_display(self):
        size = self.file_size or 0
        for unit in ['B', 'KB', 'MB', 'GB']:
            if size < 1024:
                return f"{size:.1f} {unit}"
            size /= 1024
        return f"{size:.1f} TB"

    @property
    def ext(self):
        name = self.filename
        if '.' in name:
            return name.rsplit('.', 1)[1].lower()
        return ''

    @property
    def is_text(self):
        return self.ext in {'c', 'h', 'cpp', 'hpp', 'py', 'js', 'ts', 'html', 'css',
                            'json', 'xml', 'yaml', 'yml', 'md', 'txt', 'sh', 'bat',
                            'cmake', 'makefile', 'ini', 'cfg', 'conf', 'log', 's', 'asm'}

    @property
    def lang(self):
        mapping = {
            'c': 'c', 'h': 'c', 'cpp': 'cpp', 'hpp': 'cpp',
            'py': 'python', 'js': 'javascript', 'ts': 'typescript',
            'html': 'html', 'css': 'css', 'json': 'json',
            'xml': 'xml', 'yaml': 'yaml', 'yml': 'yaml',
            'md': 'markdown', 'sh': 'bash', 'bat': 'batch',
            's': 'asm', 'asm': 'asm',
        }
        return mapping.get(self.ext, 'plaintext')


class Post(db.Model):
    """博客文章"""
    __tablename__ = 'posts'
    id = db.Column(db.Integer, primary_key=True)
    title = db.Column(db.String(200), nullable=False)
    content = db.Column(db.Text, nullable=False)
    summary = db.Column(db.String(300))
    is_public = db.Column(db.Boolean, default=True)
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)
    created_at = db.Column(db.DateTime, default=cn_now, index=True)
    updated_at = db.Column(db.DateTime, default=cn_now, onupdate=cn_now)
    view_count = db.Column(db.Integer, default=0)

    author = db.relationship('User', foreign_keys=[user_id])


class OperationLog(db.Model):
    """操作日志"""
    __tablename__ = 'operation_logs'
    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'))
    username = db.Column(db.String(80))          # 记录时快照，避免用户删除后无数据
    action = db.Column(db.String(50), nullable=False, index=True)   # login/logout/create_repo/...
    target = db.Column(db.String(200))           # 操作对象描述，如"仓库:xxx"
    detail = db.Column(db.String(500))           # 额外细节
    ip_address = db.Column(db.String(64))
    created_at = db.Column(db.DateTime, default=cn_now, index=True)

    user = db.relationship('User', foreign_keys=[user_id])
