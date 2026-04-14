#ifndef LANG_H
#define LANG_H


// // 语言类型枚举
// typedef enum {
//     LANG_EN,
//     LANG_ZH,
//     // 添加其他语言
// } LangType;

// 翻译对结构
typedef struct {
    const char* key;
    const char* zh;  // 中文
    const char* en;  // 英文
    const char* fr;  // 法语
    const char* sp;  // 西班牙语
    const char* ja;  // 日语
    const char* ko;  // 韩语
    const char* de;  // 德语
    const char* it;  // 意大利语
} TranslationPair;

void yk_f_set_language(const char* lang);
const char* yk_f_get_translation(const char* key);
void yk_f_update_ui_text();

#endif // LANG_H
