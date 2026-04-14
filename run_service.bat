@echo off
chcp 65001 >nul
title YTL 固件管理系统服务

cd /d "%~dp0"
python app.py
