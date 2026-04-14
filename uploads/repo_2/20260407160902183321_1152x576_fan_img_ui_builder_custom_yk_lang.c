#include "custom.h"


// 语言类型
const char* current_lang = "zh";

#define TR(...) (TranslationPair){ __VA_ARGS__ }
#define K(x) .key = #x
#define ZH(x) .zh = (x)
#define EN(x) .en = (x)
#define FR(x) .fr = (x)
#define SP(x) .sp = (x)
#define JA(x) .ja = (x)
#define KO(x) .ko = (x)
#define DE(x) .de = (x)
#define IT(x) .it = (x)

const TranslationPair translations[] = {
    //screen_02_main
    TR(K(airfry), ZH("空气炸"), EN("AIR FRY"), FR("AIR FRY"), SP("FREÍR AL AIRE"), JA("エアフライ"), KO("에어프라이어"), DE("HEIßLUFT"), IT("FRIGGERE AD ARIA")),  //ẞ
    TR(K(bake), ZH("烘烤"), EN("BAKE"), FR("CUIRE"), SP("HORNEAR"), JA("焼く"), KO("굽기"), DE("BACKEN"), IT("CUOCERE AL FORNO")),
    TR(K(preheat), ZH("预热"), EN("PREHEAT"), FR("PRÉCHAUFFER"), SP("PRECALENTAR"), JA("予熱"), KO("예열"), DE("VORHEIZEN"), IT("PRERISCALDARE")),
    TR(K(dehydrate), ZH("脱水"), EN("DEHYDRATE"), FR("DÉSHYDRATER"), SP("DESHIDRATAR"), JA("乾燥"), KO("건조"), DE("DÖRREN"), IT("ESSICCARE")),
    TR(K(ingredients), ZH("食材"), EN("INGREDIENTS"), FR("INGRÉDIENTS"), SP("INGREDIENTES"), JA("材料"), KO("재료"), DE("ZUTATEN"), IT("INGREDIENTI")),
    TR(K(steps), ZH("步骤"), EN("STEPS"), FR("ÉTAPES"), SP("PASOS"), JA("手順"), KO("단계"), DE("SCHRITTE"), IT("PASSAGGI")),
    TR(K(customize), ZH("我的食谱"), EN("My Recipes"), FR("Mes recettes"), SP("Mis recetas"), JA("カスタム"), KO("사용자 지정"), DE("Meine Rezepte"), IT("Le mie ricette")),
    TR(K(food_00_name), ZH("DIY00"), EN("DIY00"), FR("DIY00"), SP("DIY00"), JA("DIY00"), KO("DIY00"), DE("DIY00"), IT("DIY00")),
    TR(K(food_01_name), ZH("薯条"), EN("Fries"), FR("Frites"), SP("Papas fritas"), JA("フライドポテト"), KO("감자튀김"), DE("Pommes frites"), IT("Patatine fritte")),
    TR(K(food_02_name), ZH("鸡翅"), EN("Chicken Wings"), FR("Ailes de poulet"), SP("Alitas de pollo"), JA("手羽先"), KO("닭날개"), DE("Hähnchenflügel"), IT("Alette di pollo")),
    TR(K(food_03_name), ZH("鸡腿"), EN("Drumsticks"), FR("Pilons de poulet"), SP("Muslos de pollo"), JA("ドラムスティック"), KO("닭다리"), DE("Hähnchenschenkel"), IT("Cosce di pollo")),
    TR(K(food_04_name), ZH("烤鸡"), EN("Whole Chicken"), FR("Poulet entier"), SP("Pollo entero"), JA("丸鶏"), KO("통닭"), DE("Ganzes Hähnchen"), IT("Pollo intero")),
    TR(K(food_05_name), ZH("土豆块"), EN("Potato Wedges"), FR("Quartiers de pommes de terre"), SP("Gajos de papa"), JA("ポテトウェッジ"), KO("감자 웨지"), DE("Kartoffelspalten"), IT("Spicchi di patate")),
    TR(K(food_06_name), ZH("牛排"), EN("Steak"), FR("Steak"), SP("Bistec"), JA("ステーキ"), KO("스테이크"), DE("Steak"), IT("Bistecca")),
    TR(K(food_07_name), ZH("培根"), EN("Bacon"), FR("Bacon"), SP("Tocino"), JA("ベーコン"), KO("베이컨"), DE("Speck"), IT("Pancetta")),
    TR(K(food_08_name), ZH("肉"), EN("Meat"), FR("Viande"), SP("Carne"), JA("肉"), KO("고기"), DE("Fleisch"), IT("Carne")),
    TR(K(food_09_name), ZH("虾"), EN("Shrimp"), FR("Crevettes"), SP("Camarones"), JA("えび"), KO("새우"), DE("Garnelen"), IT("Gamberi")),
    TR(K(food_10_name), ZH("鱼"), EN("Fish"), FR("Poisson"), SP("Pescado"), JA("魚"), KO("생선"), DE("Fisch"), IT("Pesce")),
    TR(K(food_11_name), ZH("烤肠"), EN("Hot Dog"), FR("Hot-dog"), SP("Hot dog"), JA("ホットドッグ"), KO("핫도그"), DE("Hotdog"), IT("Hot dog")),
    TR(K(food_12_name), ZH("糕点"), EN("Pastry"), FR("Pâtisserie"), SP("Pastelería"), JA("ペストリー"), KO("페이스트리"), DE("Gebäck"), IT("Pasticceria")),
    TR(K(food_13_name), ZH("蔬菜"), EN("Vegetables"), FR("Légumes"), SP("Verduras"), JA("野菜"), KO("채소"), DE("Gemüse"), IT("Verdure")),
    TR(K(food_14_name), ZH("排骨"), EN("Ribs"), FR("Côtes"), SP("Costillas"), JA("スペアリブ"), KO("갈비"), DE("Rippchen"), IT("Costine")),
    TR(K(food_15_name), ZH("披萨"), EN("Pizza"), FR("Pizza"), SP("Pizza"), JA("ピザ"), KO("피자"), DE("Pizza"), IT("Pizza")),
    TR(K(food_16_name), ZH("洋葱圈"), EN("Onion Rings"), FR("Rondelles d'oignon"), SP("Aros de cebolla"), JA("オニオンリング"), KO("어니언 링"), DE("Zwiebelringe"), IT("Anelli di cipolla")),
    TR(K(states_in_traffic), ZH("正在工作状态中，请勿息屏，否则设备会停止工作！"), EN("Device in operation. Do not turn off the screen, or it will stop working!"), FR("Appareil en fonctionnement. Ne pas éteindre l'écran, sinon il s'arrêtera !"), SP("Dispositivo en funcionamiento. No apague la pantalla, ¡o dejará de funcionar!"), JA("お気に入り"), KO("즐겨찾기"), DE("Gerät in Betrieb. Bildschirm nicht ausschalten, sonst stoppt es!"), IT("Dispositivo in funzione. Non spegnere lo schermo, altrimenti si fermerà!")),
    TR(K(shake), ZH("翻面"), EN("SHAKE"), FR("RETOURNER LES ALIMENTS"), SP("DALE LA VUELTA A LA COMIDA"), JA("裏返す"), KO("뒤집기"), DE("WENDEN"), IT("GIRARE")),
    TR(K(cancel), ZH("取消"), EN("Cancel"), FR("Annuler"), SP("Cancelar"), JA("キャンセル"), KO("취소"), DE("Abbrechen"), IT("Annulla")),
    TR(K(choose_mode), ZH("选择模式"), EN("Choose Mode"), FR("Choisissez un mode"), SP("Seleccione la categoría"), JA("モードを選択"), KO("모드 선택"), DE("Modus wählen"), IT("Scegli modalità")),
    TR(K(insert_basket), ZH("插入炸篮"), EN("Insert basket"), FR("Insérer la friteuse"), SP("Insertar la freidora"), JA("バスケットを挿入"), KO("바스켓 삽입"), DE("Korb einsetzen"), IT("Inserisci cestello")),
    TR(
        K(personalized_recipes_hint),
        ZH(
            "（可以在此处设置您的个性化菜谱）\n\n"
            "1. 列出食材清单\n\n"
            "2. 输入操作步骤\n\n"
            "3. 点击右侧“爱心”按钮加入收藏方便以后使用"
        ),
        EN(
            "(You can set your personalized recipes here)\n\n"
            "1. List ingredients.\n\n"
            "2. Enter steps.\n\n"
            "3. Click the heart button on the right to save for later use."
        ),
        FR(
            "(Vous pouvez définir vos recettes personnalisées ici)\n\n"
            "1. Listez les ingrédients.\n\n"
            "2. Saisissez les étapes.\n\n"
            "3. Cliquez sur le bouton « cœur » à droite pour enregistrer et l'utiliser plus tard."
        ),
        SP(
            "(Aquí puedes configurar tus recetas personalizadas)\n\n"
            "1. Enumera los ingredientes.\n\n"
            "2. Introduce los pasos.\n\n"
            "3. Pulsa el botón « corazón » a la derecha para guardarlo y usarlo más tarde."
        ),
        JA(
            "（ここであなたの個人レシピを設定できます）\n\n"
            "1. 材料をリストアップ。\n\n"
            "2. 手順を入力。\n\n"
            "3. 右側の「ハート」ボタンを押して保存し、後で使用できます。"
        ),
        KO(
            "(여기에서 개인 레시피를 설정할 수 있습니다)\n\n"
            "1. 재료를 나열합니다.\n\n"
            "2. 조리 단계를 입력합니다.\n\n"
            "3. 오른쪽의 \"하트\" 버튼을 눌러 저장하면 나중에 사용할 수 있습니다."
        ),
        DE(
            "(Hier können Sie Ihre persönlichen Rezepte anlegen)\n\n"
            "1. Zutaten auflisten.\n\n"
            "2. Schritte eingeben.\n\n"
            "3. Zum Speichern für die spätere Verwendung auf die \"Herz\"-Schaltfläche rechts klicken."
        ),
        IT(
            "(Qui puoi impostare le tue ricette personalizzate)\n\n"
            "1. Elenca gli ingredienti.\n\n"
            "2. Inserisci i passaggi.\n\n"
            "3. Clicca sul pulsante « cuore » a destra per salvarla e usarla in futuro."
        )
    ),

    TR(
        K(input_only_alnum),
        ZH("只能输入英文字母和数字"),
        EN("Only letters and numbers can be entered."),
        FR("Seuls les lettres et les chiffres sont autorisés."),
        SP("Solo se permiten letras y números."),
        JA("英字と数字のみ入力できます。"),
        KO("영문자와 숫자만 입력할 수 있습니다."),
        DE("Es sind nur Buchstaben und Zahlen eingebbar."),
        IT("Sono consentiti solo lettere e numeri.")
    ),
    TR(
        K(confirm),
        ZH("确定"),
        EN("Confirm"),
        FR("Confirmer"),
        SP("Confirmar"),
        JA("確認"),
        KO("확인"),
        DE("Bestätigen"),
        IT("Conferma")
    ),

    //screen_03_setting
    TR(K(setting), ZH("设置"), EN("Setting"), FR("Paramètres"), SP("Configuración"), JA("設定"), KO("설정"), DE("Einstellungen"), IT("Impostazioni")),
    TR(K(display), ZH("显示"), EN("Display"), FR("Affichage"), SP("Pantalla"), JA("ディスプレイ"), KO("디스플레이"), DE("Anzeige"), IT("Display")),
    TR(K(language), ZH("语言"), EN("Language"), FR("Langue"), SP("Idioma"), JA("言語"), KO("언어"), DE("Sprache"), IT("Lingua")),
    TR(K(brightness), ZH("亮度"), EN("Brightness"), FR("Luminosité"), SP("Brillo"), JA("明るさ"), KO("밝기"), DE("Helligkeit"), IT("Luminosità")),
    TR(K(clock), ZH("时钟"), EN("Clock"), FR("Horloge"), SP("Reloj"), JA("時計"), KO("시계"), DE("Uhr"), IT("Orologio")),
    TR(K(sound), ZH("声音"), EN("Sound"), FR("Son"), SP("Sonido"), JA("音量"), KO("소리"), DE("Ton"), IT("Suono")),
    TR(K(f_to_c), ZH("°F to °C"), EN("°F to °C"), FR("°F to °C"), SP("°F to °C"), JA("°F から °C へ"), KO("°F to °C"), DE("°F to °C"), IT("°F to °C")),
    TR(K(version), ZH("版本"), EN("Version"), FR("Version"), SP("Versión"), JA("バージョン"), KO("버전"), DE("Version"), IT("Versione")),
    TR(K(restore_factory_settings), ZH("恢复出厂设置"), EN("Restore Factory Settings"), FR("Réinitialiser les paramètres"), SP("Restaurar a configuración predeterminada"), JA("ファクトリ設定を復元"), KO("제품 설정 복원"), DE("Zurücksetzen auf Werkseinstellungen"), IT("Ripristina impostazioni di fabbrica")),
    TR(K(factory_reset_warning),ZH("恢复出厂设置后，所有数据会被删除，请谨慎操作。"),EN("After restoring factory settings, all data will be deleted. Please proceed with caution."),FR("Après la restauration des paramètres d’usine, toutes les données seront supprimées. Veuillez procéder avec prudence."),SP("Después de restaurar la configuración de fábrica, se eliminarán todos los datos. Proceda con precaución."),JA("工場出荷時設定に戻すと、すべてのデータが削除されます。慎重に操作してください。"),KO("공장 초기화 후 모든 데이터가 삭제됩니다. 주의해서 진행하세요."),DE("Nach dem Zurücksetzen auf Werkseinstellungen werden alle Daten gelöscht. Bitte vorsichtig fortfahren."),IT("Dopo il ripristino delle impostazioni di fabbrica, tutti i dati verranno eliminati. Procedere con cautela.")),
    TR(K(factory_reset_success),ZH("恢复出厂设置成功"),EN("Factory reset successful"),FR("Réinitialisation d’usine réussie"),SP("Restablecimiento de fábrica exitoso"),JA("工場出荷時設定に復元しました"),KO("공장 초기화 완료"),DE("Werkseinstellungen wiederhergestellt"),IT("Ripristino impostazioni di fabbrica riuscito")),

    //主题界面
    TR(K(light), ZH("亮"), EN("Light"), FR("Clair"), SP("Claro"), JA("明るい"), KO("밝은"), DE("Hell"), IT("Chiaro")),
    TR(K(dark), ZH("暗"), EN("Dark"), FR("Foncé"), SP("Oscuro"), JA("暗い"), KO("어두운"), DE("Dunkel"), IT("Scuro")),

    //语言界面
    TR(K(english), ZH("英文"), EN("English"), FR("Anglais"), SP("Inglés"), JA("英語"), KO("영어"), DE("Englisch"), IT("Inglese")),
    TR(K(chinese), ZH("中文"), EN("Chinese"), FR("Chinois"), SP("Chino"), JA("中国語"), KO("중국어"), DE("Chinesisch"), IT("Cinese")),
    TR(K(french), ZH("法语"), EN("French"), FR("Français"), SP("Francés"), JA("フランス語"), KO("프랑스어"), DE("Französisch"), IT("Francese")),
    TR(K(spanish), ZH("西班牙语"), EN("Spanish"), FR("Espagnol"), SP("Español"), JA("スペイン語"), KO("스페인어"), DE("Spanisch"), IT("Spagnolo")),
    TR(K(japan), ZH("日语"), EN("Japanese"), FR("Japonais"), SP("Japonés"), JA("日本語"), KO("일본어"), DE("Japanisch"), IT("Giapponese")),
    TR(K(korean), ZH("韩语"), EN("Korean"), FR("Coréen"), SP("Coreano"), JA("韓国語"), KO("한국어"), DE("Koreanisch"), IT("Coreano")),
    TR(K(german), ZH("德语"), EN("German"), FR("Allemand"), SP("Alemán"), JA("ドイツ語"), KO("독일어"), DE("Deutsch"), IT("Tedesco")),
    TR(K(italian), ZH("意大利语"), EN("Italian"), FR("Italien"), SP("Italiano"), JA("イタリア語"), KO("이탈리아어"), DE("Italienisch"), IT("Italiano")),

    //时钟界面
    TR(K(hour_system_24), ZH("24小时模式"), EN("24-hour system"), FR("Système 24 heures"), SP("Sistema de 24 horas"), JA("24 時間方式"), KO("24시간 모드"), DE("24-Stunden-System"), IT("Formato 24 ore")),
    TR(K(am), ZH("上午"), EN("AM"), FR("AM"), SP("AM"), JA("午前"), KO("오전"), DE("AM"), IT("AM")),
    TR(K(pm), ZH("下午"), EN("PM"), FR("PM"), SP("PM"), JA("午後"), KO("오후"), DE("PM"), IT("PM")),
    TR(K(time), ZH("时间"), EN("Time"), FR("Temps"), SP("Hora"), JA("時間"), KO("시간"), DE("Zeit"), IT("Ora")),
    TR(K(ok), ZH("确定"), EN("OK"), FR("OK"), SP("OK"), JA("OK"), KO("확인"), DE("OK"), IT("OK")),
    TR(K(screensaver_time_display), ZH("屏保时间显示"), EN("Screensaver time display"), FR("Affichage du temps (Économiseur d'écran)"), SP("Visualización del reloj (salvapantallas)"), JA("スクリーンセーバーの時間表示"), KO("화면보호기 시간 표시"), DE("Bildschirmschoner-Zeitanzeige"), IT("Visualizzazione dell'ora (salvaschermo)")),
    TR(K(digital), ZH("数字"), EN("Digital"), FR("Numérique"), SP("Digital"), JA("デジタル"), KO("디지털"), DE("Digital"), IT("Digitale")),
    TR(K(on), ZH("开"), EN("On"), FR("Allumé"), SP("Encendido"), JA("オン"), KO("켜짐"), DE("Ein"), IT("On")),
    TR(K(off), ZH("关"), EN("Off"), FR("Éteint"), SP("Apagado"), JA("オフ"), KO("꺼짐"), DE("Aus"), IT("Off")),

    //收藏界面
    TR(K(favorites), ZH("收藏"), EN("Favorites"), FR("Favoris"), SP("Favoritos"), JA("お気に入り"), KO("즐겨찾기"), DE("Favoriten"), IT("Preferiti")),
    TR(K(favorites_rename_title), ZH("修改名称"), EN("Edit name"), FR("Modifier le nom"), SP("Editar nombre"), JA("名前を編集"), KO("이름 편집"), DE("Name bearbeiten"), IT("Modifica nome")),
    TR(K(save), ZH("保存"), EN("Save"), FR("Enregistrer"), SP("Guardar"), JA("保存"), KO("저장"), DE("Speichern"), IT("Salva")),
    TR(K(favorites_name_empty), ZH("名称不能为空"), EN("Name cannot be empty"), FR("Le nom ne peut pas être vide"), SP("El nombre no puede estar vacío"), JA("名前は空にできません"), KO("이름은 비워둘 수 없습니다"), DE("Name darf nicht leer sein"), IT("Il nome non può essere vuoto")),
    TR(K(favorites_delete_confirm), ZH("是否删除？"), EN("Delete?"), FR("Supprimer ?"), SP("¿Eliminar?"), JA("削除しますか？"), KO("삭제하시겠습니까?"), DE("Löschen?"), IT("Eliminare?")),
    TR(K(favorites_save_confirm), ZH("保存修改？"), EN("Save changes?"), FR("Enregistrer les modifications ?"), SP("¿Guardar cambios?"), JA("変更を保存しますか？"), KO("변경 사항을 저장하시겠습니까?"), DE("Änderungen speichern?"), IT("Salvare le modifiche?")),
    TR(K(favorites_remove_confirm), ZH("确认取消收藏？"), EN("Remove from favorites?"), FR("Retirer des favoris ?"), SP("¿Eliminar de favoritos?"), JA("お気に入りを解除しますか？"), KO("즐겨찾기에서 제거할까요?"), DE("Aus Favoriten entfernen?"), IT("Rimuovere dai preferiti?")),
    TR(K(favorites_removed), ZH("取消收藏成功"), EN("Removed from favorites"), FR("Retiré des favoris"), SP("Eliminado de favoritos"), JA("お気に入りから削除しました"), KO("즐겨찾기에서 제거됨"), DE("Aus Favoriten entfernt"), IT("Rimosso dai preferiti")),
    TR(K(favorites_added), ZH("已收藏"), EN("Added to favorites"), FR("Ajouté aux favoris"), SP("Añadido a favoritos"), JA("お気に入りに追加しました"), KO("즐겨찾기에 추가됨"), DE("Zu Favoriten hinzugefügt"), IT("Aggiunto ai preferiti")),
    TR(K(favorites_max_10), ZH("最多添加20个收藏"), EN("Up to 20 favorites"), FR("Maximum 20 favoris"), SP("Máximo 20 favoritos"), JA("お気に入りは最大20件"), KO("즐겨찾기 최대 20개"), DE("Maximal 20 Favoriten"), IT("Massimo 20 preferiti")),
    TR(K(busy_cannot_switch), ZH("当前正在工作中无法切换"), EN("Currently in operation, switching is not available."), FR("Opération en cours, changement indisponible."), SP("Operación en curso, no se puede cambiar."), JA("動作中は切り替えできません"), KO("작동 중에는 전환할 수 없습니다"), DE("Derzeit in Betrieb, Umschaltung nicht möglich."), IT("Operazione in corso, impossibile cambiare.")),

    TR(
        K(food_00_ingredients),
        ZH(
            "（可以在此处设置您的个性化菜谱）\n\n"
            "1. 列出食材清单\n\n"
            "2. 输入操作步骤。\n\n"
            "3. 点击左上角“爱心”按钮加入收藏列表，方便以后使用。"
        ),
        EN(
            "(Here you can set up your personalized recipes)\n\n"
            "1. List the ingredients.\n\n"
            "2. Enter the cooking steps.\n\n"
            "3. Tap the \"Heart\" button in the top-left corner to add it to your favorites for later use."
        ),
        FR(
            "(Vous pouvez configurer votre recette personnalisée ici)\n\n"
            "1. Listez les ingrédients.\n\n"
            "2. Saisissez les étapes de préparation.\n\n"
            "3. Appuyez sur le bouton « Cœur » en haut à gauche pour l'ajouter à vos favoris et l'utiliser ultérieurement."
        ),
        SP(
            "(Aquí puedes configurar tu receta personalizada)\n\n"
            "1. Enumera los ingredientes.\n\n"
            "2. Introduce los pasos de preparación.\n\n"
            "3. Pulsa el botón de „Corazón“ en la esquina superior izquierda para añadirlo a favoritos y usarlo más tarde."
        ),
        JA(
            "（ここでオリジナルレシピを設定できます）\n\n"
            "1. 材料リストを入力\n\n"
            "2. 手順を入力\n\n"
            "3. 左上の「ハート」ボタンを押してお気に入りに追加"
        ),
        KO(
            "(여기에서 개인 레시피를 설정할 수 있습니다)\n\n"
            "1. 재료 목록 입력\n\n"
            "2. 조리 단계 입력\n\n"
            "3. 상단 왼쪽의 “하트” 버튼을 눌러 즐겨찾기에 추가"
        ),
        DE(
            "(Hier können Sie Ihr persönliches Rezept einrichten)\n\n"
            "1. Geben Sie die Zutatenliste an.\n\n"
            "2. Geben Sie die Zubereitungsschritte ein.\n\n"
            "3. Tippen Sie auf den „Herz“-Button oben links, um es zu Ihren Favoriten hinzuzufügen und später zu verwenden."
        ),
        IT(
            "(Qui puoi impostare la tua ricetta personalizzata)\n\n"
            "1. Elenca gli ingredienti.\n\n"
            "2. Inserisci i passaggi di preparazione.\n\n"
            "3. Tocca il pulsante „Cuore“ in alto a sinistra per aggiungerla ai preferiti e usarla in futuro."
        )
    ),
    TR(K(food_00_steps), ZH("food_00_steps:\n 自定义3 步骤"), EN("food_00_steps:\n Custom 3: Steps"), FR("food_00_steps:\n Personnalisé 3 : Étapes"), SP("food_00_steps:\n Personalizado 3: Pasos"), JA("food_00_steps:\n カスタム3：手順"), KO("food_00_steps:\n 사용자 지정 3: 단계"), DE("food_00_steps:\n Benutzerdefiniert 3: Schritte"), IT("food_00_steps:\n Personalizzato 3: Passaggi")),

    TR(
        K(food_01_ingredients),
        ZH("薯条：1磅 (450克)"),
        EN("Fries: 1 lb (450g)"),
        FR("Frites surgelées : 450 g"),
        SP("Patatas fritas: 450 g"),
        JA("フライドポテト：1ポンド（約450g）"),
        KO("감자튀김: 1파운드(약 450g)"),
        DE("Pommes frites: 1 lb (450 g)"),
        IT("Patatine fritte: 450 g")
    ),
    TR(
        K(food_01_steps),
        ZH(
            "1.从冰箱或冰柜取出速冻薯条，无需解冻。\n\n"
            "2.把薯条倒入空气炸锅里，设置温度400℉，时间15-18分钟。\n\n"
            "注意：薯条不要堆叠过高！"
        ),
        EN(
            "1. Take the frozen fries directly from the refrigerator or freezer—no need to thaw.\n\n"
            "2. Pour the fries into the air fryer, set the temperature to 400°F (approx. 200°C), and cook for 15–18 minutes.\n\n"
            "Note: Do not stack the fries too high!"
        ),
        FR(
            "1. Sortez les frites surgelées du réfrigérateur ou du congélateur — inutile de décongeler.\n\n"
            "2. Versez les frites dans la friteuse à air, réglez à 400°F (env. 200°C) et cuisez 15–18 minutes.\n\n"
            "Remarque : ne les empilez pas trop haut !"
        ),
        SP(
            "1. Saque las papas congeladas del refrigerador o congelador—no es necesario descongelar.\n\n"
            "2. Vierta las papas en la freidora de aire, ajuste a 400°F (aprox. 200°C) y cocine 15–18 min.\n\n"
            "Nota: No amontones las patatas en capas demasiado altas!"
        ),
        JA(
            "1. 冷凍フライドポテトを冷蔵庫または冷凍庫から取り出します（解凍不要）。\n\n"
            "2. エアフライヤーに入れ、400°F（約200°C）で15〜18分加熱します。\n\n"
            "注意：高く積み重ねないでください。"
        ),
        KO(
            "1. 냉동 감자튀김을 냉장/냉동 보관함에서 꺼내세요(해동 불필요).\n\n"
            "2. 에어프라이어에 넣고 400°F(약 200°C)에서 15~18분 조리하세요.\n\n"
            "주의: 감자튀김을 너무 높게 쌓지 마세요."
        ),
        DE(
            "1. Die tiefgekühlten Pommes frites direkt aus dem Kühlschrank oder Gefrierfach nehmen – nicht auftauen.\n\n"
            "2. Die Pommes in die Heißluftfritteuse geben, Temperatur auf 200°C einstellen und 15-18 Minuten garen.\n\n"
            "Hinweis: Nicht zu hoch stapeln!"
        ),
        IT(
            "1.Prendere le patatine fritte surgelate dal frigorifero o dal congelatore: non è necessario scongelarle.\n\n"
            "2.  Versare le patatine nella friggitrice ad aria, impostare la temperatura a 200°C e cuocere per 15-18 minuti.\n\n"
            "Nota: Non disporre le patatine a strati troppo alti!"
        )
    ),

    TR(
        K(food_02_ingredients),
        ZH(
            "鸡翅：1磅（450克）\n"
            "橄榄油：1汤匙\n"
            "盐：½茶匙\n"
            "黑胡椒：½茶匙\n"
            "其他调味料（可选）：红椒粉、蒜粉、辣椒粉各½茶匙"
        ),
        EN(
            "Chicken wings: 1 lb (450 g)\n"
            "Olive oil: 1 tbsp\n"
            "Salt: ½ tsp\n"
            "Black pepper: ½ tsp\n"
            "Optional seasonings: ½ tsp each of paprika, garlic powder, and chili powder"
        ),
        FR(
            "Ailes de poulet : 450 g\n"
            "Huile d’olive : 15 ml\n"
            "Sel : 2,5 ml\n"
            "Poivre noir : 2,5 ml\n"
            "Épices optionnelles : 2,5 ml chacune de paprika, poudre d’ail et poudre de piment"
        ),
        SP(
            "Alitas de pollo: 450 g\n"
            "Aceite de oliva: 15 ml\n"
            "Sal: 2,5 ml\n"
            "Pimienta negra: 2,5 ml\n"
            "Condimentos opcionales: 2,5 ml cada uno de pimentón, ajo en polvo y chile en polvo"
        ),
        JA(
            "手羽先：1ポンド（約450g）\n"
            "オリーブオイル：大さじ1\n"
            "塩：小さじ1/2\n"
            "黒こしょう：小さじ1/2\n"
            "お好み：パプリカ、ガーリックパウダー、チリパウダー 各小さじ1/2"
        ),
        KO(
            "닭날개: 1파운드(약 450g)\n"
            "올리브오일: 1큰술\n"
            "소금: ½작은술\n"
            "후추: ½작은술\n"
            "선택 양념: 파프리카, 마늘가루, 고춧가루 각 ½작은술"
        ),
        DE(
            "Hähnchenflügel: 1 lb (450 g)\n"
            "Olivenöl: 15 ml\n"
            "Salz: 2,5 ml\n"
            "Schwarzer Pfeffer: 2,5 ml\n"
            "Optionale Gewürze: je 2,5 ml Paprikapulver, Knoblauchpulver und Chilipulver"
        ),
        IT(
            "Ali di pollo: 450 g\n"
            "Olio d’oliva: 15 ml\n"
            "Sale: 2,5 ml\n"
            "Pepe nero: 2,5 ml\n"
            "Condimenti opzionali: 2,5 ml ciascuno di paprika, aglio in polvere e peperoncino in polvere"
        )
    ),
    TR(
        K(food_02_steps),
        ZH(
            "1.用厨房纸吸干鸡翅表面的水分，这样表皮更酥脆。\n\n"
            "2.确保所有调料和鸡翅混合均匀，烹饪前腌制至少1个小时，这样口感会更好。\n\n"
            "3.空气炸锅400℉预热1分钟。\n\n"
            "4.把鸡翅放入空气炸锅，设置温度400℉，时间13-16分钟。"
        ),
        EN(
            "1. Pat the chicken wings dry with paper towels to achieve crispier skin.\n\n"
            "2. Ensure all seasonings are evenly mixed with the wings. Marinate for at least 1 hour before cooking for better flavor.\n\n"
            "3. Preheat the air fryer to 400°F (about 200°C) for 1 minute.\n\n"
            "4. Place the chicken wings in the air fryer, set the temperature to 400°F (about 200°C), and cook for 13–16 minutes."
        ),
        FR(
            "1. Sécher les ailes de poulet avec du papier absorbant pour une peau plus croustillante.\n\n"
            "2. Bien mélanger toutes les épices avec les ailes. Faire mariner au moins 1 heure avant la cuisson pour plus de saveur.\n\n"
            "3. Préchauffer la friteuse à air chaud à 200°C pendant 1 minute.\n\n"
            "4. Placer les ailes de poulet dans la friteuse, régler la température sur 200°C et cuire pendant 13 à 16 minutes."
        ),
        SP(
            "1. Secar las alitas de pollo con papel de cocina para lograr una piel más crujiente.\n\n"
            "2. Asegurarse de que todos los condimentos se mezclen uniformemente con las alitas. Marinar al menos 1 hora antes de cocinar para un mejor sabor.\n\n"
            "3. Precalentar la freidora de aire a 200°C durante 1 minuto.\n\n"
            "4. Colocar las alitas de pollo en la freidora, ajustar la temperatura a 200°C y cocinar durante 13–16 minutos."
        ),
        JA(
            "1. 手羽先の表面の水分をペーパーで拭き取り、皮をよりカリッとさせます。\n\n"
            "2. 調味料を全体に均一に絡め、調理前に最低1時間漬けるとよりおいしくなります。\n\n"
            "3. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "4. 手羽先を入れ、400°F（約200°C）で13〜16分加熱します。"
        ),
        KO(
            "1. 닭날개 겉면의 물기를 키친타월로 닦아 더 바삭하게 만듭니다.\n\n"
            "2. 양념이 고르게 묻도록 버무린 뒤, 조리 전 최소 1시간 재우면 맛이 좋아집니다.\n\n"
            "3. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "4. 닭날개를 넣고 400°F(약 200°C)에서 13~16분 조리하세요."
        ),
        DE(
            "1. Hähnchenflügel mit Küchenpapier trocken tupfen, damit die Haut knuspriger wird.\n\n"
            "2. Sicherstellen, dass alle Gewürze gleichmäßig mit den Flügeln vermischt sind. Mindestens 1 Stunde vor dem Garen marinieren, für besseren Geschmack.\n\n"
            "3. Die Heißluftfritteuse auf 200°C für 1 Minute vorheizen.\n\n"
            "4. Die Hähnchenflügel in die Heißluftfritteuse legen, Temperatur auf 200°C einstellen und 13–16 Minuten garen."
        ),
        IT(
            "1. Asciugare le ali di pollo con carta da cucina per una pelle più croccante.\n\n"
            "2. Assicurarsi che tutti i condimenti siano ben distribuiti sulle ali. Marinare per almeno 1 ora prima della cottura per un sapore migliore.\n\n"
            "3. Preriscaldare la friggitrice ad aria a 200°C per 1 minuto.\n\n"
            "4. Posizionare le ali di pollo nella friggitrice, impostare la temperatura a 200°C e cuocere per 13–16 minuti."
        )
    ),

    TR(
        K(food_03_ingredients),
        ZH(
            "鸡腿: 2磅(900克)\n"
            "橄榄油或黄油: 1汤匙\n"
            "蒜粉: 2茶匙\n"
            "洋葱粉: 1茶匙\n"
            "烟熏红椒粉: 1茶匙\n"
            "盐: 1茶匙\n"
            "黑胡椒粉: 1茶匙"
        ),
        EN(
            "drumsticks: 2 lb (900 g)\n"
            "Olive oil: 1 tbsp\n"
            "Garlic powder: 1 tsp\n"
            "Onion powder: 1 tsp\n"
            "Smoked paprika: 1 tsp\n"
            "Salt: 1 tsp\n"
            "Black pepper: 1 tsp"
        ),
        FR(
            "Cuisses de poulet : 900 g\n"
            "Huile d’olive : 15 ml\n"
            "Ail en poudre : 5 ml\n"
            "Oignon en poudre : 5 ml\n"
            "Paprika fumé : 5 ml\n"
            "Sel : 5 ml\n"
            "Poivre noir : 5 ml"
        ),
        SP(
            "Muslos de pollo: 900 g\n"
            "Aceite de oliva: 15 ml\n"
            "Ajo en polvo: 5 ml\n"
            "Cebolla en polvo: 5 ml\n"
            "Pimentón ahumado: 5 ml\n"
            "Sal: 5 ml\n"
            "Pimienta negra: 5 ml"
        ),
        JA(
            "鶏もも（ドラム）：2ポンド（約900g）\n"
            "オリーブオイルまたはバター：大さじ1\n"
            "ガーリックパウダー：小さじ2\n"
            "オニオンパウダー：小さじ1\n"
            "スモークパプリカ：小さじ1\n"
            "塩：小さじ1\n"
            "黒こしょう（粉）：小さじ1"
        ),
        KO(
            "닭다리(드럼스틱): 2파운드(약 900g)\n"
            "올리브오일 또는 버터: 1큰술\n"
            "마늘가루: 2작은술\n"
            "양파가루: 1작은술\n"
            "훈제 파프리카: 1작은술\n"
            "소금: 1작은술\n"
            "후추가루: 1작은술"
        ),
        DE(
            "Hähnchenkeule: 900 g\n"
            "Olivenöl: 15 ml\n"
            "Knoblauchpulver: 5 ml\n"
            "Zwiebelpulver: 5 ml\n"
            "Geräuchertes Paprikapulver: 5 ml\n"
            "Salz: 5 ml\n"
            "Schwarzer Pfeffer: 5 ml"
        ),
        IT(
            "Cosce di pollo: 900 g\n"
            "Olio d’oliva: 15 ml\n"
            "Aglio in polvere: 5 ml\n"
            "Cipolla in polvere: 5 ml\n"
            "Paprika affumicato: 5 ml\n"
            "Sale: 5 ml\n"
            "Pepe nero: 5 ml"
        )
    ),
    TR(
        K(food_03_steps),
        ZH(
            "1.用厨房纸把鸡腿擦干。\n\n"
            "2.把鸡腿和各种调味料混合均匀。\n\n"
            "3.空气炸锅设置400℉预热1分钟。\n\n"
            "4.把鸡腿放入空气炸锅，设置温度400℉，时间25-30分钟。"
        ),
        EN(
            "1. Pat chicken thighs dry with paper towels.\n\n"
            "2. Toss chicken thighs with seasonings until evenly coated.\n\n"
            "3. Preheat air fryer to 400°F (about 200°C) for 1 min.\n\n"
            "4. Place chicken thighs in air fryer. Set to 400°F (about 200°C), cook 25–30 min."
        ),
        FR(
            "1. Sécher les cuisses de poulet avec du papier absorbant.\n\n"
            "2. Mélanger les cuisses de poulet avec les assaisonnements.\n\n"
            "3. Préchauffer la friteuse à air à 200°C pendant 1 min.\n\n"
            "4. Placer les cuisses de poulet dans la friteuse. Régler sur 200°C, cuire 25–30 min."
        ),
        SP(
            "1. Secar los muslos de pollo con papel de cocina.\n\n"
            "2. Mezclar los muslos de pollo con los condimentos.\n\n"
            "3. Precalentar la freidora de aire a 200°C durante 1 min.\n\n"
            "4. Colocar los muslos de pollo en la freidora. Ajustar a 200°C, cocinar 25–30 min."
        ),
        JA(
            "1. 鶏ももの水気をペーパーで拭き取ります。\n\n"
            "2. 調味料を全体に均一に絡めます。\n\n"
            "3. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "4. 鶏ももを入れ、400°F（約200°C）で25〜30分加熱します。"
        ),
        KO(
            "1. 닭다리의 물기를 키친타월로 닦아주세요.\n\n"
            "2. 양념을 고르게 묻도록 버무립니다.\n\n"
            "3. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "4. 닭다리를 넣고 400°F(약 200°C)에서 25~30분 조리하세요."
        ),
        DE(
            "1. Hähnchenschenkel mit Küchenpapier trocken tupfen.\n\n"
            "2. Hähnchenschenkel mit Gewürzen gleichmäßig vermengen.\n\n"
            "3. Heißluftfritteuse auf 200°C für 1 Min. vorheizen.\n\n"
            "4. Hähnchenschenkel in die Fritteuse legen. Auf 200°C einstellen, 25–30 Min. garen."
        ),
        IT(
            "1. Asciugare le cosce di pollo con carta da cucina.\n\n"
            "2. Mescolare le cosce di pollo con i condimenti.\n\n"
            "3. Preriscaldare la friggitrice ad aria a 200°C per 1 min.\n\n"
            "4.Inserire le cosce di pollo nella friggitrice. Impostare 200°C, cuocere 25–30 min."
        )
    ),

    TR(
        K(food_04_ingredients),
        ZH(
            "整鸡: 2.6 磅（最大重量）\n"
            "橄榄油: 1 汤匙\n"
            "蒜新鲜香草：2-3 支\n"
            "蒜泥：1 茶匙\n"
            "盐：1 茶匙\n"
            "黑胡椒粉：½ 茶匙\n"
            "柠檬：1个"
            "洋葱：1个"
        ),
        EN(
            "Whole chicken: 2.6 lb (max weight)\n"
            "Olive oil: 1 tbsp\n"
            "MFresh herbs: 2-3 sprigs\n"
            "Minced garlic: 1 tsp\n"
            "BSalt: 1 tsp\n"
            "Black pepper: 1 tsp\n"
            "Lemon: 1\n"
            "Onion: 1"
        ),
        FR(
            "Poulet entier : 1,2 kg (poids max)\n"
            "Huile d’olive : 15 ml\n"
            "Herbes fraîches : 2-3 brins\n"
            "Ail haché : 3 ml\n"
            "Sel : 4,5 ml\n"
            "Poivre noir : 3 ml\n"
            "Citron : 1\n"
            "Oignon : 1"
        ),
        SP(
            "Pollo entero: 1,2 kg (peso máx.)\n"
            "Aceite de oliva: 15 ml\n"
            "Hierbas frescas: 2-3 ramitas\n"
            "Ajo picado: 3 ml\n"
            "Sal: 4,5 ml\n"
            "Pimienta negra: 3 ml\n"
            "Limón: 1"
            "Cebolla: 1"
        ),
        JA(
            "丸鶏：2.2〜2.6ポンド（約1.0〜1.2kg）\n"
            "生ハーブ：大さじ2\n"
            "にんにく（みじん切り）：小さじ1\n"
            "塩：小さじ1と1/2\n"
            "黒こしょう：小さじ1\n"
            "レモン：1個\n"
            "玉ねぎ：1個"
        ),
        KO(
            "통닭: 2.2~2.6파운드(약 1.0~1.2kg)\n"
            "신선한 허브: 2큰술\n"
            "다진 마늘: 1작은술\n"
            "소금: 1½작은술\n"
            "후추: 1작은술\n"
            "레몬: 1개\n"
            "양파: 1개"
        ),
        DE(
            "Hähnchen (ganz): 1,2 kg (max. Gewicht)\n"
            "Olivenöl: 15 ml\n"
            "Frische Kräuter: 2-3 Zweige\n"
            "Knoblauch, gehackt: 5 ml\n"
            "Salz: 5 ml\n"
            "Schwarzer Pfeffer: 3 ml\n"
            "Zitrone: 1\n"
            "Zwiebel: 1"
        ),
        IT(
            "Pollo intero: 1,2 kg (peso max)\n"
            "Olio d'oliva: 15 ml\n"
            "Erbe aromatiche fresche: 2-3 rametti\n"
            "Aglio tritato: 3 ml\n"
            "Sale: 4,5 ml\n"
            "Pepe nero: 3 ml\n"
            "Limone: 1\n"
            "Cipolla: 1"
        )
    ),
    TR(
        K(food_04_steps),
        ZH(
            "1. 用厨房纸将整鸡内外完全擦干。\n\n"
            "2. 将各种调味料混合均匀。涂抹在整只鸡的内外表面，将柠檬和洋葱切成块，塞入鸡腔内。\n\n"
            "3. 空气炸锅设置400℉预热1分钟\n\n"
            "4. 将整鸡放入空气炸锅篮中，设置温度400℉，时间30分钟。\n\n"
            "5. 15分钟后翻面。"
        ),
        EN(
            "1. Pat whole chicken dry inside and out with paper towels.\n\n"
            "2. Mix seasonings evenly. Rub over entire chicken. Place chopped lemon and onion inside cavity.\n\n"
            "3. Preheat air fryer to 400°F (about 200°C) for 1 min.\n\n"
            "4. Place chicken in fryer basket. Set to 400°F (about 200°C), cook 30 min.\n\n"
            "5. Flip after 15 min."
        ),
        FR(
            "1. Sécher le poulet entier à l’intérieur et à l’extérieur avec du papier absorbant.\n\n"
            "2. Mélanger les assaisonnements. Enduire le poulet. Placer le citron et l’oignon hachés dans la cavité.\n\n"
            "3. Préchauffer la friteuse à air à 200°C pendant 1 min.\n\n"
            "4. Placer le poulet dans le panier. Régler sur 200°C, cuire 30 min.\n\n"
            "5.Retourner après 15 min."
        ),
        SP(
            "1. Secar el pollo entero por dentro y por fuera con papel de cocina.\n\n"
            "2. Mezclar los condimentos. Untar sobre el pollo. Colocar el limón y la cebolla picados en la cavidad.\n\n"
            "3. Precalentar la freidora de aire a 200°C  durante 1 min.\n\n"
            "4. Colocar el pollo en la cesta. Ajustar a 200°C, cocinar 30 min.\n\n"
            "5. Voltear después de 15 min."
        ),
        JA(
            "1. 丸鶏の内外の水気をペーパーでしっかり拭き取ります。\n\n"
            "2. 室温に戻したバターに調味料を混ぜます。\n\n"
            "3. ハーブバターを鶏の内外に塗り、レモンと玉ねぎを切って腹に詰めます。\n\n"
            "4. エアフライヤーを400°F（200°C）で1分予熱します。\n\n"
            "5. 胸を上にして入れ、400°F（200°C）で25〜30分加熱します。"
        ),
        KO(
            "1. 통닭의 안팎 물기를 키친타월로 완전히 제거하세요.\n\n"
            "2. 실온의 버터와 양념을 잘 섞습니다.\n\n"
            "3. 허브버터를 닭의 안팎에 고르게 바르고, 레몬과 양파를 썰어 닭 속에 넣습니다.\n\n"
            "4. 에어프라이어를 400°F(200°C)로 1분 예열하세요.\n\n"
            "5. 가슴살이 위로 가게 바스켓에 넣고 400°F(200°C)에서 25~30분 조리하세요."
        ),
        DE(
            "1. Hähnchen innen und außen mit Küchenpapier trocken tupfen.\n\n"
            "2. Gewürze gleichmäßig mischen. Hähnchen einreiben. Gehackte Zitrone und Zwiebel in die Höhle geben.\n\n"
            "3. Heißluftfritteuse auf 200°C 1 Min. vorheizen.\n\n"
            "4. Hähnchen in den Korb legen. Auf 200°C einstellen, 30 Min. garen.\n\n"
            "5. Nach 15 Min. wenden."
        ),
        IT(
            "1. Asciugare il pollo intero dentro e fuori con carta da cucina.\n\n"
            "2. Mescolare i condimenti. Spalmare sul pollo. Mettere limone e cipolla tritati nella cavità.\n\n"
            "3. Preriscaldare la friggitrice ad aria a 200°C per 1 min.\n\n"
            "4. Inserire il pollo nel cestello. Impostare 200°C, cuocere 30 min.\n\n"
            "5. Girare dopo 15 min."
        )
    ),

    TR(
        K(food_05_ingredients),
        ZH(
            "土豆：1.5磅（680克）\n"
            "橄榄油：1-2汤匙\n"
            "蒜粉：1茶匙\n"
            "干迷迭香：½ 茶匙\n"
            "辣椒粉：½ 茶匙\n"
            "盐：1 茶匙\n"
            "黑胡椒粉：½ 茶匙"
        ),
        EN(
            "Potatoes: 1.5 lb (680 g)\n"
            "Olive oil: 1 tbsp\n"
            "Garlic powder: 1 tsp\n"
            "Dried rosemary: ½ tsp\n"
            "Paprika: ½ tsp\n"
            "Salt: 1 tsp\n"
            "Black pepper: ½ tsp"
        ),
        FR(
            "Pommes de terre : 680 g\n"
            "Huile d’olive : 15 ml\n"
            "Ail en poudre : 5 ml\n"
            "RRomarin séché : 2,5 ml\n"
            "Paprika : 2,5 ml\n"
            "Sel : 5 ml\n"
            "Poivre noir : 2,5 ml"
        ),
        SP(
            "Patatas: 680 g\n"
            "Aceite de oliva: 15 ml\n"
            "Ajo en polvo: 5 ml\n"
            "Romero seco: 2,5 ml\n"
            "Pimentón: 2,5 ml\n"
            "Sal: 5 ml\n"
            "Pimienta negra: 2,5 ml"
        ),
        JA(
            "じゃがいも：1.5ポンド（約680g）\n"
            "オリーブオイル：大さじ1〜2\n"
            "ガーリックパウダー：小さじ1\n"
            "乾燥ローズマリー：小さじ1/2\n"
            "パプリカ：小さじ1/2\n"
            "塩：小さじ1\n"
            "黒こしょう：小さじ1/2"
        ),
        KO(
            "감자: 1.5파운드(약 680g)\n"
            "올리브오일: 1~2큰술\n"
            "마늘가루: 1작은술\n"
            "건조 로즈마리: ½작은술\n"
            "파프리카: ½작은술\n"
            "소금: 1작은술\n"
            "후추: ½작은술"
        ),
        DE(
            "Kartoffeln: 680 g\n"
            "Olivenöl: 15 ml\n"
            "Knoblauchpulver: 5 ml\n"
            "Getrockneter Rosmarin: 2,5 ml\n"
            "Paprikapulver: 2,5 ml\n"
            "Salz: 5 ml\n"
            "Schwarzer Pfeffer: 2,5 ml"
        ),
        IT(
            "Patate: 680 g\n"
            "Olio d’oliva: 15 ml\n"
            "Aglio in polvere: 5 ml\n"
            "Rosmarino secco: 2,5 ml\n"
            "Paprika: 2,5 ml\n"
            "Sale: 5 ml\n"
            "Pepe nero: 2,5 ml"
        )
    ),
    TR(
        K(food_05_steps),
        ZH(
            "1.将土豆切成约1.5-2厘米大小均匀的方块。\n\n"
            "2.将土豆块放入冷水中浸泡10-15分钟。这一步可以去除表面多余淀粉，使烤出来的土豆块更酥脆，内部更蓬松。\n\n"
            "3.用厨房纸把土豆块彻底擦干，将完全干燥的土豆块淋上橄榄油，撒上所有调味料，确保每一块土豆都均匀裹上油和调味料。\n\n"
            "4.设置温度350℉，时间20-25分钟。"
        ),
        EN(
            "1. Cut potatoes into even 1.5-2 cm cubes.\n\n"
            "2. Soak cubes in cold water 10-15 min. Removes starch for crispier outside, fluffier inside.\n\n"
            "3. Pat dry with paper towels. Drizzle with oil, toss with seasonings until evenly coated.\n\n"
            "4. Set air fryer to 355°F (about 180°C), cook 20–25 min."
        ),
        FR(
            "1. Couper les pommes de terre en cubes de 1.5-2 cm.\n\n"
            "2. Faire tremper les cubes dans l’eau froide 10-15 min. Élimine l’amidon pour un extérieur plus croustillant, un intérieur plus moelleux.\n\n"
            "3. Sécher avec du papier absorbant. Arroser d’huile, mélanger avec les assaisonnements jusqu’à enrobage uniforme.\n\n"
            "4. Régler la friteuse à air sur 180°C, cuire 20–25 min."
        ),
        SP(
            "1. Cortar las patatas en cubos de 1.5-2 cm.\n\n"
            "2. Remojar los cubos en agua fría 10-15 min. Elimina almidón para exterior más crujiente, interior más esponjoso.\n\n"
            "3. Secar con papel de cocina. Rociar con aceite, mezclar con condimentos hasta cubrir uniformemente.\n\n"
            "4.Ajustar la freidora de aire a 180°C, cocinar 20–25 min."
        ),
        JA(
            "1. じゃがいもを1.5〜2cm角の大きさに揃えて切ります。\n\n"
            "2. 冷水に10〜15分浸して表面の余分なでんぷんを落とします。\n\n"
            "3. 水気をペーパーでしっかり拭き取り、オリーブオイルと調味料を加えて全体に絡めます。\n\n"
            "4. 355°F（約180°C）で20〜25分加熱します。"
        ),
        KO(
            "1. 감자를 1.5~2cm 크기의 균일한 큐브로 자릅니다.\n\n"
            "2. 찬물에 10~15분 담가 전분을 제거하면 겉은 더 바삭하고 속은 포슬포슬해집니다.\n\n"
            "3. 키친타월로 물기를 완전히 제거한 뒤 올리브오일과 양념을 넣고 골고루 버무립니다.\n\n"
            "4. 355°F(약 180°C)에서 20~25분 조리하세요."
        ),
        DE(
            "1. Kartoffeln in gleichmäßige 1.5-2 cm Würfel schneiden.\n\n"
            "2. Würfel 10–15 Min. in kaltem Wasser einweichen. Entfernt Stärke für knusprigere Außenseite, lockerere Innenseite.\n\n"
            "3. Mit Küchenpapier trocken tupfen. Mit Öl beträufeln, mit Gewürzen mischen, bis gleichmäßig bedeckt.\n\n"
            "4. Heißluftfritteuse auf 180°C einstellen, 20–25 Min. garen."
        ),
        IT(
            "1. Tagliare le patate a cubetti di 1.5-2 cm.\n\n"
            "2. Mettere i cubetti a bagno in acqua fredda 10–15 min. Rimuove l’amido per esterno più croccante, interno più soffice.\n\n"
            "3. Asciugare con carta da cucina. Condire con olio, mescolare con i condimenti fino a copertura uniforme.\n\n"
            "4. Impostare la friggitrice ad aria a 180°C, cuocere 20–25 min."
        )
    ),

    TR(
        K(food_06_ingredients),
        ZH(
            "牛排：1-1.5英寸厚（2.5-3.8厘米）\n"
            "橄榄油：2 汤匙\n"
            "粗盐：1 茶匙\n"
            "黑胡椒粉：1 茶匙\n"
            "蒜粉：½ 茶匙\n"
            "辣椒粉：¼ 茶匙\n"
            "大蒜：0.1 盎司\n"
            "新鲜香草：1-2 支"
        ),
        EN(
            "Steak: 1-1.5 inches thick (2.5-3.8 cm)\n"
            "Olive oil: 2 tablespoons\n"
            "Coarse salt: 1 teaspoon\n"
            "Black pepper: 1 teaspoon\n"
            "Garlic powder: ½ teaspoon\n"
            "Paprika: ¼ teaspoon\n"
            "Garlic: 0.1 ounce\n"
            "Fresh herbs: 1-2 sprigs"
        ),
        FR(
            "Steak : épaisseur 2,5–3,8 cm\n"
            "Huile d’olive : 30 ml\n"
            "Gros sel : 5 ml\n"
            "Poivre noir : 5 ml\n"
            "Ail en poudre : 2,5 ml\n"
            "Paprika : 1,25 ml\n"
            "Ail : 3 g\n"
            "Herbes fraîches : 1–2 brins"
        ),
        SP(
            "Filete (bistec): 2,5–3,8 cm de grosor\n"
            "Aceite de oliva: 30 ml\n"
            "Sal gruesa: 5 ml\n"
            "Pimienta negra: 5 ml\n"
            "Ajo en polvo: 2,5 ml\n"
            "Pimentón: 1,25 ml\n"
            "Ajo: 3 g\n"
            "Hierbas frescas: 1–2 ramitas"
        ),
        JA(
            "ステーキ：厚さ1〜1.5インチ（約2.5〜3.8cm）\n"
            "オリーブオイル：大さじ2\n"
            "粗塩：小さじ1\n"
            "黒こしょう：小さじ1\n"
            "ガーリックパウダー：小さじ1/2\n"
            "パプリカ：小さじ1/4\n"
            "にんにく：0.1オンス\n"
            "生ハーブ：1〜2本"
        ),
        KO(
            "스테이크: 두께 1~1.5인치(약 2.5~3.8cm)\n"
            "올리브오일: 2큰술\n"
            "굵은 소금: 1작은술\n"
            "후추: 1작은술\n"
            "마늘가루: ½작은술\n"
            "파프리카: ¼작은술\n"
            "마늘: 0.1 oz\n"
            "신선한 허브: 1~2줄기"
        ),
        DE(
            "Steak: 2,5–3,8 cm dick\n"
            "Olivenöl: 30 ml\n"
            "Grobkörniges Salz: 5 ml\n"
            "Schwarzer Pfeffer: 5 ml\n"
            "Knoblauchpulver: 2,5 ml\n"
            "Paprikapulver: 1,25 ml\n"
            "Knoblauch: 3 g\n"
            "Frische Kräuter: 1–2 Zweige"
        ),
        IT(
            "Bistecca: spessore 2,5–3,8 cm\n"
            "Olio d’oliva: 30 ml\n"
            "Sale grosso: 5 ml\n"
            "Pepe nero: 5 ml\n"
            "Aglio in polvere: 2,5 ml\n"
            "Paprika: 1,25 ml\n"
            "Aglio: 3 g\n"
            "Erbe aromatiche fresche: 1–2 rametti"
        )
    ),
    TR(
        K(food_06_steps),
        ZH(
            "1.用厨房纸彻底擦干牛排表面的水分，在牛排两面均匀涂抹橄榄油，然后撒上调味料。\n\n"
            "2.空气炸锅设置400℉预热1分钟。\n\n"
            "3.把牛排放入空气炸锅，设置温度400℉，时间6-8分钟。\n\n"
            "4.在烹饪时间过半时，取出炸篮，用夹子将牛排翻面。"
        ),
        EN(
            "1. Thoroughly pat the steak dry with paper towels. Rub olive oil evenly over both sides of the steak, then sprinkle with seasonings.\n\n"
            "2. Preheat the air fryer to 400°F (about 200°C) for 1 minute.\n\n"
            "3. Place the steak in the air fryer and set the temperature to 400°F (about 200°C). Cook for 6-8 minutes.\n\n"
            "4. Halfway through the cooking time, remove the basket and flip the steak using tongs."
        ),
        FR(
            "1. Bien sécher le steak avec du papier absorbant. Badigeonner d’huile d’olive sur les deux faces, puis saupoudrer d’assaisonnements.\n\n"
            "2. Préchauffer la friteuse à air à 200°C pendant 1 minute.\n\n"
            "3. Placer le steak dans la friteuse. Régler sur 200°C, cuire 6 à 8 minutes.\n\n"
            "4. À mi-cuisson, retirer le panier et retourner le steak à l’aide de pinces."
        ),
        SP(
            "1. Secar bien el filete con papel de cocina. Frotar el aceite por ambos lados, luego espolvorear con condimentos.\n\n"
            "2. Precalentar la freidora de aire a 200°C durante 1 minuto.\n\n"
            "3. Colocar el filete en la freidora. Ajustar a 200°C, cocinar 6–8 minutos.\n\n"
            "4. A mitad de cocción, retirar la cesta y voltear el filete con pinzas."
        ),
        JA(
            "1. ステーキの水気をペーパーでしっかり拭き取り、両面にオリーブオイルを塗って調味料を振ります。\n\n"
            "2. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "3. 入れて400°F（約200°C）で6〜8分加熱します。\n\n"
            "4. 途中で一度取り出し、トングで裏返します。"
        ),
        KO(
            "1. 스테이크 겉면의 물기를 키친타월로 닦고 양면에 올리브오일을 바른 뒤 양념을 뿌립니다.\n\n"
            "2. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "3. 400°F(약 200°C)에서 6~8분 조리하세요.\n\n"
            "4. 중간에 바스켓을 꺼내 집게로 뒤집습니다."
        ),
        DE(
            "1. Steak gründlich mit Küchenpapier trocken tupfen. Olivenöl gleichmäßig auf beiden Seiten verreiben, dann mit Gewürzen bestreuen.\n\n"
            "2. Heißluftfritteuse auf 200°C für 1 Minute vorheizen.\n\n"
            "3. Steak in die Fritteuse legen. Temperatur auf 200°C einstellen, 6–8 Minuten garen.\n\n"
            "4. Nach der Hälfte der Zeit Korb entnehmen und Steak mit Zange wenden."
        ),
        IT(
            "1. Asciugare bene la bistecca con carta da cucina. Strofinare l’olio su entrambi i lati, poi cospargere di condimenti.\n\n"
            "2. Preriscaldare la friggitrice ad aria a 200°C per 1 minuto.\n\n"
            "3. Inserire la bistecca nella friggitrice. Impostare 200°C, cuocere 6–8 minuti.\n\n"
            "4. A metà cottura, rimuovere il cestello e girare la bistecca con una pinza."
        )
    ),

    TR(
        K(food_07_ingredients),
        ZH(
            "培根6-8片\n"
            "可选配料：黑胡椒碎、辣椒粉、蒜粉"
        ),
        EN(
            "Bacon: 6-8 slices\n"
            "Optional seasonings: cracked black pepper, paprika, garlic powder"
        ),
        FR(
            "Bacon : 6–8 tranches\n"
            "Assaisonnements (option) : poivre noir concassé, paprika, ail en poudre"
        ),
        SP(
            "Tocino: 6-8 lonchas\n"
            "Condimentos opcionales: pimienta negra molida gruesa, pimentón, ajo en polvo"
        ),
        JA(
            "ベーコン：6〜8枚\n"
            "お好み：粗びき黒こしょう、パプリカ、ガーリックパウダー"
        ),
        KO(
            "베이컨: 6~8장\n"
            "선택 양념: 굵은 후추, 파프리카, 마늘가루"
        ),
        DE(
            "Speck: 6-8 Scheiben\n"
            "Optionale Gewürze: grober schwarzer Pfeffer, Paprikapulver, Knoblauchpulver"
        ),
        IT(
            "Pancetta: 6-8 fette\n"
            "Condimenti opzionali: pepe nero in grani, paprika, aglio in polvere"
        )
    ),
    TR(
        K(food_07_steps),
        ZH(
            "1.把培根平铺在炸篮里，不要重叠。\n\n"
            "2.设置温度355℉，时间5-8分钟。"
        ),
        EN(
            "1. Lay the bacon slices flat in the air fryer basket without overlapping.\n\n"
            "2. Set the temperature to 355°F (about 180°C) and cook for 5–8 minutes."
        ),
        FR(
            "1. Disposer les tranches de bacon à plat dans le panier de la friteuse sans les chevaucher.\n\n"
            "2. Régler la température sur 180°C et cuire pendant 5 à 8 minutes."
        ),
        SP(
            "1. Colocar las lonchas de tocino planas en la cesta de la freidora de aire sin que se solapen.\n\n"
            "2. Ajustar la temperatura a 180°C y cocinar durante 5–8 minutos."
        ),
        JA(
            "1. ベーコンをバスケットに重ならないように並べます。\n\n"
            "2. 400°F（約200°C）で5〜8分加熱します。"
        ),
        KO(
            "1. 베이컨을 바스켓에 겹치지 않게 펼쳐 담습니다.\n\n"
            "2. 400°F(약 200°C)에서 5~8분 조리하세요."
        ),
        DE(
            "1. Die Speckscheiben flach und ohne Überlappung in den Korb der Heißluftfritteuse legen.\n\n"
            "2. Temperatur auf 180°C einstellen und für 5–8 Minuten garen"
        ),
        IT(
            "1. Disporre le fette di pancetta in un unico strato nel cestello della friggitrice ad aria, senza sovrapporle.\n\n"
            "2. Impostare la temperatura a 180°C e cuocere per 5–8 minuti."
        )
    ),

    TR(
        K(food_08_ingredients),
        ZH(
            "猪排：厚度 2-3cm\n"
            "橄榄油：1 汤匙\n"
            "盐：½ 茶匙\n"
            "大蒜：0.1 盎司\n"
            "黑胡椒碎：1 茶匙\n"
            "蜂蜜：1 茶匙\n"
            "可选配料：辣椒粉，洋葱丝，迷迭香"
        ),
        EN(
            "Pork chops: 2-3 cm thick\n"
            "Olive oil: 1 tbsp\n"
            "Salt: ½ tsp\n"
            "Garlic: 0.1 oz\n"
            "Cracked black pepper: 1 tsp\n"
            "Honey: 1 tsp\n"
            "Optional: paprika, onion strips, rosemary"
        ),
        FR(
            "Côtes de porc : 2–3 cm d’épaisseur\n"
            "Huile d’olive : 15 ml\n"
            "Sel : 2,5 ml\n"
            "Ail : 3 g\n"
            "Poivre noir concassé : 5 ml\n"
            "Miel : 5 ml\n"
            "Optionnel : paprika, lamelles d’oignon, romarin"
        ),
        SP(
            "Chuletas de cerdo: 2–3 cm de grosor\n"
            "Aceite de oliva: 15 ml\n"
            "Sal: 2,5 ml\n"
            "Ajo: 3 g\n"
            "Pimienta negra molida gruesa: 5 ml\n"
            "Miel: 5 ml\n"
            "Opcional: pimentón, tiras de cebolla, romero"
        ),
        JA(
            "豚肉（ポークチョップ）：厚さ2〜3cm\n"
            "オリーブオイル：大さじ1\n"
            "塩：小さじ1/2\n"
            "にんにく：0.1オンス（約3g）\n"
            "粗びき黒こしょう：小さじ1\n"
            "はちみつ：小さじ1\n"
            "お好み：パプリカ、玉ねぎスライス、ローズマリー"
        ),
        KO(
            "돼지 목살/돈까스용(두께 2~3cm)\n"
            "올리브오일: 1큰술\n"
            "소금: ½작은술\n"
            "마늘: 0.1 oz(약 3g)\n"
            "굵은 후추: 1작은술\n"
            "꿀: 1작은술\n"
            "선택 재료: 파프리카, 양파채, 로즈마리"
        ),
        DE(
            "Schweinekoteletts: 2–3 cm dick\n"
            "Olivenöl: 15 ml\n"
            "Salz: 2.5 ml\n"
            "Knoblauch: 3 g\n"
            "Grob gemahlener Pfeffer: 5 ml\n"
            "Honig: 5 ml\n"
            "Optional: Paprikapulver, Zwiebelstreifen, Rosmarin"
        ),
        IT(
            "Bistecche di maiale: spessore 2–3 cm\n"
            "Olio d’oliva: 15 ml\n"
            "Sale: 2,5 ml\n"
            "Aglio: 3 g\n"
            "Pepe nero in grani: 5 ml\n"
            "Miele: 5 ml\n"
            "Opzionale: paprika, strisce di cipolla, rosmarino"
        )
    ),
    TR(
        K(food_08_steps),
        ZH(
            "1.用厨房纸擦干猪排表面的水分，用刀背轻拍猪排两面，让肉质更松软，更容易入味。\n\n"
            "2.把所有腌料混合均匀，涂抹在猪排两面，确保每一处都裹上腌料，密封后冷藏腌制1小时。\n\n"
            "3.空气炸锅设置400℉预热1分钟。\n\n"
            "4.设置温度400℉，时间15-20分钟。"
        ),
        EN(
            "1. Pat chops dry. Tenderize both sides with back of knife.\n\n"
            "2. Mix marinade, coat chops evenly. Cover, refrigerate 1 hr.\n\n"
            "3. Preheat air fryer to 400°F (about 200°C) for 1 min.\n\n"
            "4. Set the temperature to 400°F (about 200°C) for 15–20 min."
        ),
        FR(
            "1. Sécher les côtes. Attendrir les deux faces avec le dos d’un couteau.\n\n"
            "2. Mélanger la marinade, enrober les côtes. Couvrir, réfrigérer 1 h.\n\n"
            "3. Préchauffer la friteuse à air à 200°C pendant 1 min.\n\n"
            "4. Régler la température à 200°C pour 15–20 min."
        ),
        SP(
            "1. Secar las chuletas. Ablandar ambos lados con dorso del cuchillo\n\n"
            "2. Mezclar el adobo, cubrir las chuletas. Tapar, refrigerar 1 h.\n\n"
            "3. Precalentar la freidora de aire a 200°C durante 1 min.\n\n"
            "4. Ajustar la temperatura a 200°C durante 15–20 min."
        ),
        JA(
            "1. 豚肉の水気を拭き取り、包丁の背で両面を軽くたたいて柔らかくします。\n\n"
            "2. マリネを混ぜ、豚肉の両面に塗って密閉し、冷蔵で1時間漬けます。\n\n"
            "3. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "4. 400°F（約200°C）で15〜20分加熱します。"
        ),
        KO(
            "1. 돼지고기 표면의 물기를 닦고 칼등으로 양면을 가볍게 두드려 부드럽게 합니다.\n\n"
            "2. 양념을 섞어 양면에 고르게 바르고 밀봉해 냉장 1시간 재웁니다.\n\n"
            "3. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "4. 400°F(약 200°C)에서 15~20분 조리하세요."
        ),
        DE(
            "1. Koteletts trocken tupfen. Beide Seiten mit Messerrücken klopfen.\n\n"
            "2. Marinade mischen, Koteletts gleichmäßig bestreichen. Abdecken, 1 Std. kühlen.\n\n"
            "3. Heißluftfritteuse auf 200°C für 1 Min. vorheizen.\n\n"
            "4. Bei 200°C für 15–20 Min. garen."
        ),
        IT(
            "1. Asciugare le bistecche. Ammorbidire entrambi i lati con dorso del coltello.\n\n"
            "2. Mescolare la marinata, ricoprire le bistecche. Coprire, refrigerare 1 h\n\n"
            "3. Preriscaldare la friggitrice ad aria a 200°C per 1 min.\n\n"
            "4. Impostare la temperatura a 200°C per 15–20 min."
        )
    ),

    TR(
        K(food_09_ingredients),
        ZH(
            "虾：1 磅（约450g）\n"
            "大蒜：0.1 盎司\n"
            "橄榄油：2 茶匙\n"
            "柠檬：1个\n"
            "海盐：½ 茶匙\n"
            "黑胡椒：½ 茶匙\n"
            "干欧芹碎：½ 茶匙"
        ),
        EN(
            "Shrimp: 1 lb (approx. 450g)\n"
            "Garlic: 0.1 oz (approx. 3g)\n"
            "Olive oil: 2 teaspoons\n"
            "Lemon: 1\n"
            "Sea salt: ½ teaspoon\n"
            "Black pepper: ½ teaspoon\n"
            "Dried parsley flakes: ½ teaspoon"
        ),
        FR(
            "Crevettes : 450 g\n"
            "Ail : 3 g\n"
            "Huile d'olive : 10 ml\n"
            "Citron : 1\n"
            "Sel de mer : 2,5 ml\n"
            "Poivre noir : 2,5 ml\n"
            "Flocons de persil séché : 2,5 ml"
        ),
        SP(
            "Gambas/Camarones: 450 g\n"
            "Ajo: 3 g\n"
            "Aceite de oliva: 10 ml\n"
            "Limón: 1\n"
            "Sal marina: 2,5 ml\n"
            "Pimienta negra: 2,5 ml\n"
            "Hojuelas de perejil seco: 2,5 ml"
        ),
        JA(
            "えび：1ポンド（約450g）\n"
            "にんにく：0.1オンス（約3g）\n"
            "オリーブオイル：小さじ2\n"
            "レモン：1個\n"
            "海塩：小さじ1/2\n"
            "黒こしょう：小さじ1/2\n"
            "乾燥パセリ：小さじ1/2"
        ),
        KO(
            "새우: 1파운드(약 450g)\n"
            "마늘: 0.1 oz(약 3g)\n"
            "올리브오일: 2작은술\n"
            "레몬: 1개\n"
            "천일염: ½작은술\n"
            "후추: ½작은술\n"
            "건파슬리: ½작은술"
        ),
        DE(
            "Garnelen: 450 g\n"
            "Knoblauch: 3 g\n"
            "Olivenöl: 10 ml\n"
            "Zitrone: 1\n"
            "Meersalz: 2,5 ml\n"
            "Schwarzer Pfeffer: 2,5 ml\n"
            "Getrocknete Petersilienflocken: 2,5 ml"
        ),
        IT(
            "Gamberetti: 450 g\n"
            "Aglio: 3 g\n"
            "Olio d'oliva: 10 ml\n"
            "Limone: 1\n"
            "Sale marino: 2,5 ml\n"
            "Pepe nero: 2,5 ml\n"
            "Fiocchi di prezzemolo essiccato: 2,5 ml"
        )
    ),
    TR(
        K(food_09_steps),
        ZH(
            "1.用厨房纸吸干虾表面的水分，平铺在碗中。\n\n"
            "加入调味料搅拌均匀。\n\n"
            "2.空气炸锅设置400℉预热1分钟。\n\n"
            "3.把虾放入空气炸锅，设置温度400℉，时间8分钟。"
        ),
        EN(
            "1. Pat the shrimp dry with paper towels and place them in a bowl. Add the seasonings and mix well.\n\n"
            "2. Preheat the air fryer to 400°F (approx. 200°C) for 1 minute.\n\n"
            "3. Place the shrimp in the air fryer, set the temperature to 400°F (approx. 200°C), and cook for 7-8 minutes."
        ), 
        FR(
            "1. Sécher les crevettes avec du papier absorbant et les placer dans un bol. Ajouter les assaisonnements et bien mélanger.\n\n"
            "2. Préchauffer la friteuse à air chaud à 200°C pendant 1 minute.\n\n"
            "3. Placer les crevettes dans la friteuse, régler la température sur 200°C et cuire pendant 7-8 minutes."
        ),
        SP(
            "1. Secar las gambas con papel de cocina y colocarlas en un bol. Añadir los condimentos y mezclar bien.\n\n"
            "2. Precalentar la freidora de aire a 200°C durante 1 minuto.\n\n"
            "3. Colocar las gambas en la freidora, ajustar la temperatura a 200°C y cocinar durante 7-8 minutos."
        ),
        JA(
            "1. えびの水気を拭き取り、ボウルに入れて調味料を加え、よく混ぜます。\n\n"
            "2. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "3. えびを入れ、400°F（約200°C）で8分加熱します。"
        ),
        KO(
            "1. 새우 물기를 닦아 볼에 담고 양념을 넣어 잘 버무립니다.\n\n"
            "2. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "3. 새우를 넣고 400°F(약 200°C)에서 8분 조리하세요."
        ),
        DE(
            "1. Garnelen mit Küchenpapier trocken tupfen und in eine Schüssel geben. Gewürze hinzufügen und gut vermischen.\n\n"
            "2. Heißluftfritteuse auf 200°C für 1 Minute vorheizen.\n\n"
            "3. Garnelen in die Heißluftfritteuse geben, Temperatur auf 200°C einstellen und für 7-8 Minuten garen."
        ),
        IT(
            "1. Asciugare i gamberetti con carta da cucina e metterli in una ciotola. Aggiungere i condimenti e mescolare bene.\n\n"
            "2. Preriscaldare la friggitrice ad aria a 200°C per 1 minuto.\n\n"
            "3. Posizionare i gamberetti nella friggitrice, impostare la temperatura a 200°C e cuocere per 7-8 minuti."
        )
    ),

    TR(
        K(food_10_ingredients),
        ZH(
            "1鱼块：1 磅（450g）\n"
            "辣椒粉：1茶匙\n"
            "蒜粉：½ 茶匙\n"
            "洋葱粉：½ 茶匙\n"
            "芥末粉：¼ 茶匙\n"
            "盐：¼ 茶匙\n"
            "黑胡椒粉：¼ 茶匙\n"
            "橄榄油：1茶匙"
        ),
        EN(
            "Fish pieces: 1 lb (450g)\n"
            "Paprika: 1 tsp\n"
            "Garlic powder: ½ tsp\n"
            "Onion powder: ½ tsp\n"
            "Mustard powder: ¼ tsp\n"
            "Salt: ¼ tsp\n"
            "Black pepper: ¼ tsp\n"
            "Olive oil: 1 tsp"
        ),
        FR(
            "Filets de poisson : 450 g\n"
            "Paprika : 5 ml\n"
            "Ail en poudre : 2,5 ml\n"
            "Oignon en poudre : 2,5 ml\n"
            "Moutarde en poudre : 1,25 ml\n"
            "Sel : 1,25 ml\n"
            "Poivre noir : 1,25 ml\n"
            "Huile d’olive : 5 ml"
        ),
        SP(
            "Filetes de pescado: 450 g\n"
            "Pimentón: 5 ml\n"
            "Ajo en polvo: 2,5 ml\n"
            "Cebolla en polvo: 2,5 ml\n"
            "Mostaza en polvo: 1,25 ml\n"
            "Sal: 1,25 ml\n"
            "Pimienta negra: 1,25 ml\n"
            "Aceite de oliva: 5 ml"
        ),
        JA(
            "魚の切り身：1ポンド（約450g）\n"
            "パプリカ：小さじ1\n"
            "ガーリックパウダー：小さじ1/2\n"
            "オニオンパウダー：小さじ1/2\n"
            "マスタードパウダー：小さじ1/4\n"
            "塩：小さじ1/4\n"
            "黒こしょう：小さじ1/4\n"
            "オリーブオイル：小さじ1"
        ),
        KO(
            "생선 필레: 1파운드(약 450g)\n"
            "파프리카: 1작은술\n"
            "마늘가루: ½작은술\n"
            "양파가루: ½작은술\n"
            "머스터드가루: ¼작은술\n"
            "소금: ¼작은술\n"
            "후추: ¼작은술\n"
            "올리브오일: 1작은술"
        ),
        DE(
            "Fischfilets: 450 g\n"
            "Paprikapulver: 5 ml\n"
            "Knoblauchpulver: 2,5 ml\n"
            "Zwiebelpulver: 2,5 ml\n"
            "Senfpulver: 1,25 ml\n"
            "Salz: 1,25 ml\n"
            "Schwarzer Pfeffer: 1,25 ml\n"
            "Olivenöl: 5 ml"
        ),
        IT(
            "Filetti di pesce: 450 g\n"
            "Paprika: 5 ml\n"
            "Aglio in polvere: 2,5 ml\n"
            "Cipolla in polvere: 2,5 ml\n"
            "Senape in polvere: 1,25 ml\n"
            "Sale: 1,25 ml\n"
            "Pepe nero: 1,25 ml\n"
            "Olio d’oliva: 5 ml"
        )
    ),
    TR(
        K(food_10_steps),
        ZH(
            "1.鱼块用厨房纸吸干水分，把所有调味料混合均匀，将调好的腌料均匀涂抹在鱼块表面，静置 10 分钟入味。\n\n"
            "2.空气炸锅设置400℉预热1分钟。\n\n"
            "3.把鱼块放入空气炸锅，设置温度355℉，时间20-25分钟。"
        ),
        EN(
            "1. Pat fish pieces dry. Mix seasonings, coat fish evenly. Rest 10 min.\n\n"
            "2. Preheat air fryer to 400°F (200°C) for 1 min.\n\n"
            "3. Place fish in fryer. Set to 355°F (180°C), cook 20–25 min."
        ),
        FR(
            "1. Sécher les filets avec du papier absorbant. Mélanger les assaisonnements, enrober les filets. Laisser reposer 10 min.\n\n"
            "2. Préchauffer la friteuse à air à 200°C pendant 1 min.\n\n"
            "3. Placer les filets dans la friteuse. Régler sur 180°C, cuire 20–25 min.."
        ),
        SP(
            "1. Secar los filetes con papel de cocina. Mezclar los condimentos, cubrir los filetes. Reposar 10 min.\n\n"
            "2. Precalentar la freidora de aire a 200°C durante 1 min.\n\n"
            "3. Colocar los filetes en la freidora. Ajustar a 180°C, cocinar 20–25 min."
        ),
        JA(
            "1. 魚の水気を拭き取り、調味料を混ぜて表面に塗り、10分置いてなじませます。\n\n"
            "2. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "3. 400°F（約180°C）で20〜25分加熱します。"
        ),
        KO(
            "1. 생선의 물기를 닦고 양념을 섞어 표면에 고르게 바른 뒤 10분 두어 맛을 배게 합니다.\n\n"
            "2. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "3. 355°F(약 180°C)에서 20~25분 조리하세요."
        ),
        DE(
            "1. Fischfilets mit Küchenpapier trocken tupfen. Alle Gewürze mischen, Filets gleichmäßig bestreichen. 10 Min. ziehen lassen.\n\n"
            "2. Heißluftfritteuse auf 200°C für 1 Min. vorheizen.\n\n"
            "3. Fischfilets in die Fritteuse legen. Auf 180°C einstellen, 20–25 Min. garen."
        ),
        IT(
            "1. Asciugare i filetti con carta da cucina. Mescolare i condimenti, spalmare sui filetti. Riposare 10 min.\n\n"
            "2. Preriscaldare la friggitrice ad aria a 200°C per 1 min.\n\n"
            "3. Inserire i filetti nella friggitrice. Impostare 180°C, cuocere 20–25 min."
        )
    ),

    TR(
        K(food_11_ingredients),
        ZH(
            "热狗肠：5根\n"
            "黄油：0.25盎司\n"
            "辣椒粉：½ 茶匙\n"
            "洋葱碎：½ 茶匙\n"
            "可选配料：黄芥末酱、酸黄瓜片"
        ),
        EN(
            "Hot dogs: 5 pieces\n"
            "Butter: 0.25 oz\n"
            "Paprika: ½ tsp\n"
            "Minced onion: ½ tsp\n"
            "Optional: yellow mustard, pickle slices"
        ),
        FR(
            "Saucisses (hot dogs) : 5 pièces\n"
            "Beurre : 7 g\n"
            "Paprika : 2,5 ml\n"
            "Oignon haché : 2,5 ml\n"
            "Optionnel : moutarde jaune, tranches de cornichon"
        ),
        SP(
            "Salchichas (hot dogs): 5 piezas\n"
            "Mantequilla: 7 g\n"
            "Pimentón: 2,5 ml\n"
            "Cebolla picada: 2,5 ml\n"
            "Opcional: mostaza amarilla, rodajas de pepinillo"
        ),
        JA(
            "ホットドッグソーセージ：5本\n"
            "バター：0.25オンス\n"
            "パプリカ：小さじ1/2\n"
            "玉ねぎのみじん切り：小さじ1/2\n"
            "お好み：イエローマスタード、ピクルス"
        ),
        KO(
            "핫도그 소시지: 5개\n"
            "버터: 0.25 oz\n"
            "파프리카: ½작은술\n"
            "다진 양파: ½작은술\n"
            "선택 토핑: 옐로 머스터드, 피클 슬라이스"
        ),
        DE(
            "Würstchen (Hot Dogs): 5 Stück\n"
            "Butter: 7 g\n"
            "Paprikapulver: 2,5 ml\n"
            "Zwiebelwürfel: 2,5 ml\n"
            "Optional: Senf, Gewürzgurkenscheiben"
        ),
        IT(
            "Wurstel (hot dog): 5 pezzi\n"
            "Burro: 7 g\n"
            "Paprika: 2,5 ml\n"
            "Cipolla tritata: 2,5 ml\n"
            "Opzionale: senape gialla, fette di cetriolino"
        )
    ),
    TR(
        K(food_11_steps),
        ZH(
            "1.用刀在热狗肠表面划 3-4 道斜刀纹，放入调味料混合均匀。\n\n"
            "2.空气炸锅设置400℉预热1分钟。\n\n"
            "3.把热狗肠放入空气炸锅，设置温度400℉，时间8-10分钟。"
        ),
        EN(
            "1. Score hot dogs 3–4 times. Toss with seasonings.\n\n"
            "2. Preheat air fryer to 400°F (about 200°C) for 1 minute.\n\n"
            "3. Place hot dogs in fryer. Set to 400°F (about 200°C), cook 10 minutes."
        ),
        FR(
            "1. Entailler les saucisses 3–4 fois. Mélanger avec les assaisonnements.\n\n"
            "2. Préchauffer la friteuse à air à 200°C pendant 1 minute.\n\n"
            "3.  Placer les saucisses dans la friteuse. Régler sur 200°C, cuire 10 minutes."
        ),
        SP(
            "1. Hacer 3–4 cortes en las salchichas. Mezclar con los condimentos.\n\n"
            "2. Precalentar la freidora de aire a 200°C durante 1 minuto.\n\n"
            "3. Colocar las salchichas en la freidora. Ajustar a 200°C, cocinar 10 minutos."
        ),
        JA(
            "1. ソーセージの表面に斜めに3〜4本切り込みを入れ、調味料と和えます。\n\n"
            "2. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "3. 入れて400°F（約200°C）で8〜10分加熱します。"
        ),
        KO(
            "1. 소시지 표면에 사선으로 3~4군데 칼집을 내고 양념과 버무립니다.\n\n"
            "2. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "3. 400°F(약 200°C)에서 8~10분 조리하세요."
        ),
        DE(
            "1. Würstchen 3–4 Mal einschneiden. Mit Gewürzen vermengen.\n\n"
            "2. Heißluftfritteuse auf 200°C für 1 Minute vorheizen.\n\n"
            "3. Würstchen in die Fritteuse legen. Auf 200°C einstellen, 10 Minuten garen."
        ),
        IT(
            "1. Incidere i wurstel 3–4 volte. Mescolare con i condimenti.\n\n"
            "2. Preriscaldare la friggitrice ad aria a 200°C per 1 minuto.\n\n"
            "3. Inserire i wurstel nella friggitrice. Impostare 200°C, cuocere 10 minuti."
        )
    ),

    TR(
        K(food_12_ingredients),
        ZH(
            "面粉：5盎司（约140g）\n"
            "细砂糖：0.7 盎司\n"
            "红糖：0.5 盎司\n"
            "泡打粉：2 茶匙\n"
            "无盐黄油：1.4 盎司\n"
            "牛奶：4 汤匙"
        ),
        EN(
            "Flour: 5 oz (approx. 140g)\n"
            "Granulated sugar: 0.7 oz (approx. 20g)\n"
            "Brown sugar: 0.5 oz (approx. 15g)\n"
            "Baking powder: 1 teaspoons\n"
            "Unsalted butter: 1.4 oz (approx. 40g)\n"
            "Milk: 4 tablespoons (approx. 60ml)"
        ),
        FR(
            "Farine : 140 g\n"
            "Sucre blanc : 20 g\n"
            "Cassonade : 15 g\n"
            "Levure chimique : 5 ml\n"
            "Beurre doux : 40 g\n"
            "Lait : 60 ml"
        ),
        SP(
            "Harina: 140 g\n"
            "Azúcar blanco: 20 g\n"
            "Azúcar moreno: 15 g\n"
            "Levadura en polvo: 5 ml\n"
            "Mantequilla sin sal: 40 g\n"
            "Leche: 60 ml"
        ),
        JA(
            "薄力粉：5オンス（約140g）\n"
            "グラニュー糖：0.7オンス（約20g）\n"
            "ブラウンシュガー：0.5オンス（約15g）\n"
            "ベーキングパウダー：小さじ2\n"
            "無塩バター：1.4オンス（約40g）\n"
            "牛乳：大さじ4（約60ml）"
        ),
        KO(
            "밀가루: 5 oz(약 140g)\n"
            "설탕: 0.7 oz(약 20g)\n"
            "흑설탕: 0.5 oz(약 15g)\n"
            "베이킹파우더: 2작은술\n"
            "무염버터: 1.4 oz(약 40g)\n"
            "우유: 4큰술(약 60ml)"
        ),
        DE(
            "Mehl: 140 g\n"
            "Feinkristallzucker: 20 g\n"
            "Brauner Zucker: 15 g\n"
            "Backpulver: 5 ml\n"
            "Ungesalzene Butter: 40 g\n"
            "Milch: 60 ml"
        ),
        IT(
            "Farina: 140 g\n"
            "Zucchero semolato: 20 g\n"
            "Zucchero di canna: 15 g\n"
            "Lievito istantaneo per dolci: 5 ml\n"
            "Burro non salato: 40 g\n"
            "Latte: 60 ml"
        )
    ),
    TR(
        K(food_12_steps),
        ZH(
            "1.把面粉和配料混合均匀，加入冷藏黄油丁，用手搓成粗玉米粉状态，倒入牛奶，翻拌成面团，不要揉面，避免起筋。\n\n"
            "2.把面团擀成 2cm 左右厚度的圆饼形状。\n\n"
            "3.空气炸锅设置400℉预热1分钟。\n\n"
            "4.把面团放入空气炸锅内，设置温度320℉，时间10-12分钟。"
        ),
        EN(
            "1. Mix the flour and other dry ingredients evenly. Add cold, diced butter and rub it in with your fingertips until the mixture resembles coarse crumbs. Pour in the milk and gently fold until just combined to form a dough – avoid overmixing to prevent gluten development.\n\n"
            "2. Gently roll or pat the dough into a round disc about 2cm thick.\n\n"
            "3. Preheat the air fryer to 400°F (approx. 200°C) for 1 minute.\n\n"
            "4. Place the dough disc in the air fryer, set the temperature to 320°F (approx. 160°C), and cook for 10–12 minutes."
        ),
        FR(
            "1. Mélanger uniformément la farine et les autres ingrédients secs. Ajouter le beurre froid coupé en dés et le sabler du bout des doigts jusqu'à obtenir une texture sableuse. Verser le lait et incorporer délicatement jusqu'à former une pâte – éviter de trop travailler pour ne pas développer le gluten.\n\n"
            "2. Étaler ou aplatir délicatement la pâte en un disque d'environ 2 cm d'épaisseur.\n\n"
            "3. Préchauffer la friteuse à air chaud 200°C pendant 1 minute.\n\n"
            "4. Déposer le disque de pâte dans la friteuse, régler la température sur 160°C et cuire pendant 10 à 12 minutes."
        ),
        SP(
            "1. Mezclar uniformemente la harina y los demás ingredientes secos. Añadir la mantequilla fría cortada en dados y frotarla con las yemas de los dedos hasta obtener una textura similar a migas gruesas. Verter la leche e integrar con suavidad hasta formar una masa – evitar amasar en exceso para no desarrollar el gluten.\n\n"
            "2. Aplanar suavemente la masa formando un disco redondo de unos 2 cm de grosor.\n\n"
            "3. Precalentar la freidora de aire a 200°C durante 1 minuto.\n\n"
            "4. Colocar el disco de masa en la freidora, ajustar la temperatura a 160°C y cocinar durante 10–12 minutos."
        ),
        JA(
            "1. 粉類を混ぜ、冷えたバター（角切り）を加えて指先ですり混ぜ、そぼろ状にします。牛乳を注ぎ、さっくり混ぜて生地にします。こねすぎるとグルテンが出るのでこねないでください。\n\n"
            "2. 生地を約2cm厚の丸いディスク状に伸ばす（または押し広げる）。\n\n"
            "3. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "4. 生地を入れ、320°F（約160°C）で10〜12分焼きます。"
        ),
        KO(
            "1. 밀가루 등 건재료를 섞고 차가운 버터(깍둑썰기)를 넣어 손끝으로 비벼 굵은 빵가루처럼 만듭니다. 우유를 붓고 가볍게 섞어 반죽을 만드세요. 글루텐이 생기지 않도록 치대지 마세요.\n\n"
            "2. 반죽을 약 2cm 두께의 둥근 원반 모양으로 밀거나 눌러 펴세요.\n\n"
            "3. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "4. 반죽을 넣고 320°F(약 160°C)에서 10~12분 조리하세요."
        ),
        DE(
            "1. Mehl und die anderen trockenen Zutaten gleichmäßig vermischen. Kalte, gewürfelte Butter hinzufügen und mit den Fingerspitzen einarbeiten, bis die Mischung groben Bröseln ähnelt. Milch einrühren und vorsichtig unterheben, bis sich ein Teig formt – nicht zu viel kneten, um die Kleberbildung zu vermeiden.\n\n"
            "2. Den Teig vorsichtig zu einer runden Scheibe von etwa 2 cm Dicke ausrollen oder flach drücken.\n\n"
            "3. Die Heißluftfritteuse auf 200°C für 1 Minute vorheizen.\n\n"
            "4. Die Teigscheibe in die Heißluftfritteuse legen, Temperatur auf 160°C einstellen und für 10–12 Minuten backen."
        ),
        IT(
            "1. Mescolare uniformemente la farina e gli altri ingredienti secchi. Aggiungere il burro freddo a cubetti e sbriciolarlo con la punta delle dita fino a ottenere un composto simile a briciole grossolane. Versare il latte e amalgamare delicatamente fino a formare un impasto – evitare di lavorarlo troppo per non sviluppare il glutine.\n\n"
            "2. Stendere delicatamente l'impasto formando un disco rotondo spesso circa 2 cm.\n\n"
            "3. Preriscaldare la friggitrice ad aria a 200°C per 1 minuto.\n\n"
            "4. Posizionare il disco di impasto nella friggitrice, impostare la temperatura a 160°C e cuocere per 10–12 minuti."
        )
    ),

    TR(
        K(food_13_ingredients),
        ZH(
            "比如：土豆、番茄、胡萝卜、红薯、南瓜、彩椒、西葫芦、西兰花等\n"
            "橄榄油：1 汤匙\n"
            "大蒜：0.25 盎司\n"
            "盐：适量\n"
            "黑胡椒：适量"
        ),
        EN(
            "Vegetables: e.g., potatoes, tomatoes, carrots, sweet potatoes, pumpkin, bell peppers, zucchini, broccoli, etc.\n"
            "Olive oil: 1 tablespoon\n"
            "Garlic: 0.25 oz (approx. 7 g)\n"
            "Salt: to taste\n"
            "Black pepper: to taste"
        ),
        FR(
            "Légumes : p. ex. pommes de terre, tomates, carottes, patates douces, potiron, poivrons, courgettes, brocoli, etc.\n"
            "Huile d’olive : 15 ml\n"
            "Ail : 7 g\n"
            "Sel : selon le goût\n"
            "Poivre noir : selon le goût"
        ),
        SP(
            "Verduras: p. ej., papas, tomates, zanahorias, camote, calabaza, pimientos, calabacín, brócoli, etc.\n"
            "Aceite de oliva: 15 ml\n"
            "Ajo: 7 g\n"
            "Sal: al gusto\n"
            "Pimienta negra: al gusto"
        ),
        JA(
            "野菜：例）じゃがいも、トマト、にんじん、さつまいも、かぼちゃ、パプリカ、ズッキーニ、ブロッコリーなど\n"
            "オリーブオイル：大さじ1\n"
            "にんにく：0.25オンス（約7g）\n"
            "塩：適量\n"
            "黒こしょう：適量"
        ),
        KO(
            "채소: 예) 감자, 토마토, 당근, 고구마, 단호박, 파프리카, 주키니, 브로콜리 등\n"
            "올리브오일: 1큰술\n"
            "마늘: 0.25 oz(약 7 g)\n"
            "소금: 기호에 따라\n"
            "후추: 기호에 따라"
        ),
        DE(
            "Gemüse: z. B. Kartoffeln, Tomaten, Karotten, Süßkartoffeln, Kürbis, Paprika, Zucchini, Brokkoli usw.\n"
            "Olivenöl: 15 ml\n"
            "Knoblauch: 7 g\n"
            "Salz: nach Belieben\n"
            "Schwarzer Pfeffer: nach Belieben"
        ),
        IT(
            "Verduras al gusto: por ejemplo patatas, tomates, zanahorias, batatas, calabaza, pimientos, calabacín, brócoli, etc.\n"
            "Aceite de oliva: 15 ml\n"
            "Aglio: 7 g\n"
            "Sal: al gusto\n"
            "Pimienta negra: al gusto"
        )
    ),
    TR(
        K(food_13_steps),
        ZH(
            "1.把蔬菜洗净后，用厨房纸擦干水分，把蔬菜切碎后和调味料混合均匀，放入空气炸锅中。\n\n"
            "2.设置温度375℉，时间10-20分钟。"
        ),
        EN(
            "1. Wash the vegetables, pat them dry with paper towels, then chop them into pieces. Toss the chopped vegetables with the seasonings until evenly coated, then place them in the air fryer basket.\n\n"
            "2. Set the temperature to 375°F (approx. 190°C) and cook for 10–20 minutes."
        ),
        FR(
            "1. Laver les légumes, les sécher avec du papier absorbant, puis les couper en morceaux. Mélanger les légumes coupés avec les assaisonnements jusqu’à enrobage uniforme, puis les disposer dans le panier de la friteuse à air.\n\n"
            "2. Régler la température sur 190°C et cuire pendant 10 à 20 minutes."
        ),
        SP(
            "1. Lavar las verduras, secarlas con papel de cocina y cortarlas en trozos. Mezclar los trozos de verdura con los condimentos hasta que queden bien cubiertos, luego colocarlos en la cesta de la freidora de aire.\n\n"
            "2. Ajustar la temperatura a 190°C y cocinar durante 10–20 minutos."
        ),
        JA(
            "1. 野菜を洗ってペーパーで水気を拭き取り、食べやすく切ります。調味料と和えて全体にまぶし、バスケットに入れます。\n\n"
            "2. 375°F（約190°C）で10〜20分加熱します。"
        ),
        KO(
            "1. 채소를 씻어 키친타월로 물기를 닦고 먹기 좋게 썰어 양념과 골고루 버무린 뒤 에어프라이어 바스켓에 담습니다.\n\n"
            "2. 375°F(약 190°C)로 설정하고 10~20분 조리하세요."
        ),
        DE(
            "1. Das Gemüse waschen, mit Küchenpapier trocken tupfen und in Stücke schneiden. Die Gemüsestücke mit den Gewürzen gleichmäßig vermengen und in den Korb der Heißluftfritteuse geben.\n\n"
            "2. Temperatur auf 190°C einstellen und für 10–20 Minuten garen."
        ),
        IT(
            "1. Lavare le verdure, asciugarle con carta da cucina e tagliarle a pezzetti. Mescolare le verdure tagliate con i condimenti fino a copertura uniforme, poi trasferirle nel cestello della friggitrice ad aria.\n\n"
            "2. Impostare la temperatura a 190°C e cuocere per 10–20 minuti."
        )
    ),

    TR(
        K(food_14_ingredients),
        ZH(
            "猪肋排: 1磅（切成单根）\n"
            "橄榄油: 1 汤匙\n"
            "烧烤酱: 2 汤匙\n"
            "蒜末: 0.2 盎司\n"
            "洋葱末： 1 汤匙\n"
            "辣椒粉: ½ 茶匙\n"
            "盐: ½ 茶匙\n"
            "黑胡椒: ½ 茶匙\n"
            "蜂蜜: 1 汤匙"
        ),
        EN(
            "Pork ribs: 1 lb (cut into individual ribs)\n"
            "Olive oil: 1 tbsp\n"
            "BBQ sauce: 2 tbsp\n"
            "Minced garlic: 0.2 oz\n"
            "Minced onion: 1 tbsp\n"
            "Paprika: ½ tsp\n"
            "Salt: ½ tsp\n"
            "Black pepper: ½ tsp\n"
            "Honey: 1 tbsp"
        ),
        FR(
            "Côtes de porc : 450 g (découpées en côtes individuelles)\n"
            "Huile d’olive : 15 ml\n"
            "Sauce barbecue : 30 ml\n"
            "Ail haché : 6 g\n"
            "Oignon haché : 15 ml\n"
            "Paprika : 2,5 ml\n"
            "Sel : 2,5 ml\n"
            "Poivre noir : 2,5 ml\n"
            "Miel : 15 ml"
        ),
        SP(
            "Costillas de cerdo: 450 g (cortadas en costillas individuales)\n"
            "Aceite de oliva: 15 ml\n"
            "Salsa barbacoa: 30 ml\n"
            "Ajo picado: 6 g\n"
            "Cebolla picada: 15 ml\n"
            "Pimentón: 2,5 ml\n"
            "Sal: 2,5 ml\n"
            "Pimienta negra: 2,5 ml\n"
            "Miel: 15 ml"
        ),
        JA(
            "ポークリブ：1ポンド（1本ずつに切る）\n"
            "オリーブオイル：大さじ1\n"
            "バーベキューソース：大さじ3\n"
            "にんにく（みじん切り）：0.2オンス（約6g）\n"
            "玉ねぎ（みじん切り）：大さじ2\n"
            "パプリカ：小さじ1/2\n"
            "塩：小さじ1/2\n"
            "黒こしょう：小さじ1/2\n"
            "はちみつ：大さじ1"
        ),
        KO(
            "돼지갈비: 1파운드(한 대씩 분리)\n"
            "올리브오일: 1큰술\n"
            "바비큐 소스: 3큰술\n"
            "다진 마늘: 0.2 oz(약 6g)\n"
            "다진 양파: 2큰술\n"
            "파프리카: ½작은술\n"
            "소금: ½작은술\n"
            "후추: ½작은술\n"
            "꿀: 1큰술"
        ),
        DE(
            "Schweinerippchen: 450 g (als einzelne Rippen geschnitten)\n"
            "Olivenöl: 15 ml\n"
            "Grillsauce: 30 ml\n"
            "Knoblauch, fein gehackt: 6 g\n"
            "Zwiebel, fein gehackt: 15 ml\n"
            "Paprikapulver: 2,5 ml\n"
            "Salz: 2,5 ml\n"
            "Schwarzer Pfeffer: 2,5 ml\n"
            "Honig: 15 ml"
        ),
        IT(
            "Costine di maiale: 450 g (tagliate a costine singole)\n"
            "Olio d’oliva: 15 ml\n"
            "Salsa barbecue: 30 ml\n"
            "Aglio tritato: 6 g\n"
            "Cipolla tritata: 15 ml\n"
            "Paprika: 2,5 ml\n"
            "Sale: 2,5 ml\n"
            "Pepe nero: 2,5 ml\n"
            "Miele: 15 ml"
        )
    ),
    TR(
        K(food_14_steps),
        ZH(
            "1.清洗排骨后，用厨房纸吸干表面水分，放入容器中，把所有腌料混合均匀，密封腌制2小时。\n\n"
            "2.空气炸锅设置400℉预热1分钟。\n\n"
            "3.把排骨放入空气炸锅中，设置温度355℉，时间20-22分钟。"
        ),
        EN(
            "1. Rinse ribs, pat dry. Mix marinade, coat ribs evenly. Seal, refrigerate 2 hours.\n\n"
            "2. Preheat air fryer to 400°F (200°C) for 1 min.\n\n"
            "3. Place ribs in fryer. Set to 355°F (180°C), cook 20-22 min."
        ),
        FR(
            "1. Rincer les côtes, sécher. Mélanger la marinade, enrober les côtes. Fermer, réfrigérer 2 h.\n\n"
            "2. Préchauffer la friteuse à air à 200°C pendant 1 min.\n\n"
            "3. Placer les côtes dans la friteuse. Régler sur 180°C, cuire 20-22 min."
        ),
        SP(
            "1. Enjuagar las costillas, secar. Mezclar el adobo, cubrir las costillas. Cerrar, refrigerar 2 h.\n\n"
            "2. Precalentar la freidora de aire a 200°C durante 1 min.\n\n"
            "3. Colocar las costillas en la freidora. Ajustar a 180°C, cocinar 20-22 min."
        ),
        JA(
            "1. 排骨を洗ってペーパーで水気を拭き取り、容器に入れます。マリネ材料をよく混ぜて絡め、ふたをして2時間漬け込みます。\n\n"
            "2. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "3. 排骨を入れ、355°F（約180°C）で20〜22分加熱します。"
        ),
        KO(
            "1. 갈비를 헹군 뒤 키친타월로 물기를 닦아 용기에 넣고, 양념을 골고루 섞어 밀봉해 2시간 재워주세요.\n\n"
            "2. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "3. 갈비를 넣고 355°F(약 180°C)에서 20~22분 조리하세요."
        ),
        DE(
            "1. Rippchen abspülen und trocken tupfen. Marinade mischen, Rippchen gleichmäßig bestreichen. Verschließen, 2 Std. kühlen.\n\n"
            "2. Heißluftfritteuse auf 180°C für 1 Min. vorheizen.\n\n"
            "3. Rippchen in die Fritteuse legen. Auf 180°C einstellen, 20-22 Min. garen."
        ),
        IT(
            "1. Sciacquare le costine, asciugare. Mescolare la marinata, ricoprire le costine. Chiudere, refrigerare 2 h.\n"
            "2. Preriscaldare la friggitrice ad aria a 200°C per 1 min.\n"
            "3. Inserire le costine nella friggitrice. Impostare 180°C, cuocere 20-22 min."
        )
    ),
    TR(
        K(food_15_ingredients),
        ZH(
            "披萨饼：1 片 (6-7寸披萨)\n"
            "披萨酱：1 汤匙\n"
            "芝士碎：3.5 盎司\n"
            "香肠：3 片\n"
            "培根：2 片\n"
            "彩椒丁：1 汤匙"
        ),
        EN(
            "Pizza crust: 1 piece (6–7 inches)\n"
            "Pizza sauce: 1 tbsp\n"
            "Shredded cheese: 3.5 oz\n"
            "Sausage: 3 slices\n"
            "Bacon: 2 slices\n"
            "Diced bell peppers: 1 tbsp"
        ),
        FR(
            "Pâte à pizza : 1 pièce (15–18 cm)\n"
            "Sauce à pizza : 15 ml\n"
            "Fromage râpé : 100 g\n"
            "Saucisse : 3 tranches\n"
            "Bacon : 2 tranches\n"
            "Poivrons coupés en dés : 15 ml"
        ),
        SP(
            "Base de pizza: 1 unidad (15–18 cm)\n"
            "Salsa de pizza: 15 ml\n"
            "Queso rallado: 100 g\n"
            "Salchicha: 3 rodajas\n"
            "Tocino (Bacon): 2 lonchas\n"
            "Pimiento en dados: 15 ml"
        ),
        JA(
            "ピザ生地：1枚 (15–18 cm)\n"
            "ピザソース：小さじ3\n"
            "シュレッドチーズ：3.5オンス（約100g）\n"
            "ソーセージ：3枚\n"
            "ベーコン：2枚\n"
            "パプリカ（角切り）：大さじ1"
        ),
        KO(
            "피자 도우: 1장 (15–18 cm)\n"
            "피자 소스: 3작은술\n"
            "모짜렐라 치즈(슈레드): 3.5 oz(약 100g)\n"
            "소시지: 3장\n"
            "베이컨: 2장\n"
            "파프리카(다이스): 1큰술"
        ),
        DE(
            "Pizzaboden: 1 Stück (15–18 cm)\n"
            "Pizzasoße: 15 ml\n"
            "Geriebener Käse: 100 g\n"
            "Wurst: 3 Scheiben\n"
            "Speck: 2 Scheiben\n"
            "Gewürfelte Paprika: 15 ml"
        ),
        IT(
            "Base per pizza: 1 (15–18 cm)\n"
            "Salsa per pizza: 15 ml\n"
            "Formaggio grattugiato: 100 g\n"
            "Salsiccia: 3 fette\n"
            "Pancetta (o Bacon): 2 fette\n"
            "Peperoni a cubetti: 15 ml"
        )
    ),
    TR(
        K(food_15_steps),
        ZH(
            "1.空气炸锅设置400℉预热1分钟。\n\n"
            "2.把披萨饼平铺在油纸上,均匀抹上披萨酱，撒一半芝士碎，再铺香肠、培根、彩椒等配料，最后撒上剩余芝士。放入空气炸锅内，400℉烤 7-8分钟。\n\n"
            
        ),
        EN(
            "1. Preheat air fryer to 400°F (about 200°C) for 1 min.\n\n"
            "2. Place pizza base on parchment paper. Spread sauce evenly, sprinkle half the cheese. Add sausage, bacon, and peppers. Top with remaining cheese.Place in air fryer and bake at 400°F (about 200°C) for 7–8 min.\n\n"
            
        ),
        FR(
            "1.Préchauffer la friteuse à air à 200°C pendant 1 min.\n\n"
            "2. Placer la pâte sur du papier sulfurisé. Étaler la sauce, saupoudrer la moitié du fromage. Ajouter la saucisse, le bacon et les poivrons. Terminer avec le reste du fromage. Placer dans la friteuse et cuire à 200°C pendant 7 à 8 min.\n\n"
            
        ),
        SP(
            "1. Precalentar la freidora de aire a 200°C durante 1 min.\n\n"
            "2. Colocar la base sobre papel de horno. Extender la salsa, espolvorear la mitad del queso. Añadir salchicha, tocino y pimiento. Terminar con el queso restante. Colocar en la freidora y hornear a 200°C durante 7–8 min.\n\n"
            
        ),
        JA(
            "1. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "2. ピザ生地をクッキングシートの上に置いて入れ、400°F（約200°C）で3分焼き、縁が薄く色づいて生地が固まるまで加熱します。\n\n"
            "3. 取り出してソースを塗り、チーズの半量を散らし、ソーセージ・ベーコン・パプリカなどをのせ、残りのチーズをかけます。\n\n"
            "4. 仕上げ：戻して400°F（約200°C）で3〜5分、チーズが溶けて縁がこんがりするまで焼きます。"
        ),
        KO(
            "1. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "2. 도우를 종이호일 위에 올려 에어프라이어에 넣고 400°F(약 200°C)에서 3분 구워 가장자리가 살짝 노릇해질 때까지 굽습니다.\n\n"
            "3. 꺼내서 소스를 고르게 바르고 치즈의 절반을 뿌린 뒤 소시지, 베이컨, 파프리카 등을 올리고 남은 치즈를 뿌립니다.\n\n"
            "4. 2차 굽기: 다시 넣고 400°F(약 200°C)에서 3~5분, 치즈가 완전히 녹고 가장자리가 노릇바삭해질 때까지 굽습니다."
        ),
        DE(
            "1. Heißluftfritteuse auf 200°C für 1 Min. vorheizen.\n\n"
            "2. Pizzaboden auf Backpapier legen. Soße verteilen, Hälfte des Käses streuen. Wurst, Speck, Paprika hinzufügen. Mit restlichem Käse bestreuen. In die Fritteuse legen und bei 200°C 7–8 Min. backen.\n\n"
            
        ),
        IT(
            "1.Preriscaldare la friggitrice ad aria a 200°C per 1 min.\n\n"
            "2. Posizionare la base sulla carta forno. Spalmare la salsa, cospargere metà del formaggio. Aggiungere salsiccia, pancetta e peperoni. Completare con il formaggio rimanente. Inserire nella friggitrice e cuocere a 200°C per 7–8 min.\n\n"
            
        )
    ),

    TR(
        K(food_16_ingredients),
        ZH(
            "洋葱：1个\n"
            "面粉：1/2 杯\n"
            "鸡蛋：2个\n"
            "面包糠：1 杯\n"
            "盐：¼ 茶匙\n"
            "黑胡椒：¼ 茶匙\n"
            "蒜粉：½ 茶匙\n"
            "辣椒粉：¼ 茶匙\n"
            "橄榄油：1 茶匙\n"
            "蘸料：番茄酱、黄芥末酱"
        ),
        EN(
            "Onion: 1\n"
            "Flour: ½ cup\n"
            "Eggs: 2\n"
            "Bread crumbs: 1 cup\n"
            "Salt: ¼ tsp\n"
            "Black pepper: ¼ tsp\n"
            "Garlic powder: ½ tsp\n"
            "Paprika: ¼ tsp\n"
            "Olive oil: 1 tsp\n"
            "Dipping sauces: ketchup, yellow mustard"
        ),
        FR(
            "Oignon : 1\n"
            "Farine : 120 ml\n"
            "Œufs : 2\n"
            "Chapelure : 240 ml\n"
            "Sel : 1,25 ml\n"
            "Poivre noir : 1,25 ml\n"
            "Ail en poudre : 2,5 ml\n"
            "Paprika : 1,25 ml\n"
            "Huile d’olive : 5 ml\n"
            "Sauces pour tremper : ketchup, moutarde jaune"
        ),
        SP(
            "Cebolla: 1\n"
            "Harina: 120 ml\n"
            "Huevos: 2\n"
            "Pan rallado: 240 ml\n"
            "Sal: 1,25 ml\n"
            "Pimienta negra: 1,25 ml\n"
            "Ajo en polvo: 2,5 ml\n"
            "Pimentón: 1,25 ml\n"
            "Aceite de oliva: 5 ml\n"
            "Salsas para mojar: kétchup, mostaza amarilla"
        ),
        JA(
            "玉ねぎ：1個\n"
            "小麦粉：1/2カップ\n"
            "卵：2個\n"
            "パン粉：1カップ\n"
            "塩：小さじ1/4\n"
            "黒こしょう：小さじ1/4\n"
            "ガーリックパウダー：小さじ1/2\n"
            "パプリカ：小さじ1/4\n"
            "オリーブオイル：小さじ1\n"
            "ディップ：ケチャップ、イエローマスタード"
        ),
        KO(
            "양파: 1개\n"
            "밀가루: ½컵\n"
            "달걀: 2개\n"
            "빵가루: 1컵\n"
            "소금: ¼작은술\n"
            "후추: ¼작은술\n"
            "마늘가루: ½작은술\n"
            "파프리카: ¼작은술\n"
            "올리브오일: 1작은술\n"
            "소스: 케첩, 옐로 머스터드"
        ),
        DE(
            "Zwiebel: 1\n"
            "Mehl: 120 ml\n"
            "Eier: 2\n"
            "Semmelbrösel: 240 ml\n"
            "Salz: 1,25 ml\n"
            "Schwarzer Pfeffer: 1,25 ml\n"
            "Knoblauchpulver: 2,5 ml\n"
            "Paprikapulver: 1,25 ml\n"
            "Olivenöl: 5 ml\n"
            "Dip-Saucen: Ketchup, Senf"
        ),
        IT(
            "Cipolla: 1\n"
            "Farina: 120 ml\n"
            "Uova: 2\n"
            "Pan grattato: 240 ml\n"
            "Sale: 1,25 ml\n"
            "Pepe nero: 1,25 ml\n"
            "Aglio in polvere: 2,5 ml\n"
            "Paprika: 1,25 ml\n"
            "Olio d’oliva: 5 ml\n"
            "Salse per intingere: ketchup, senape gialla"
        )
    ),
    TR(
        K(food_16_steps),
        ZH(
            "1.洋葱去皮，切成1.5厘米厚的圆环，分开成独立的洋葱圈，放入清水中浸泡10分钟，捞出用厨房纸彻底吸干水分。\n\n"
            "2.洋葱圈依次裹满面粉和调味料，同时蘸满鸡蛋液和面包糠。\n\n"
            "3.空气炸锅设置400℉预热1分钟。\n\n"
            "4.把洋葱圈放入空气炸锅，设置温度375℉，时间10-12分钟。"
        ),
        EN(
            "1. Peel the onion and cut it into 1.5 cm thick rings. Separate the rings and soak them in cold water for 10 minutes. Remove and pat completely dry with paper towels.\n\n"
            "2. Dredge the onion rings first in the flour and seasoning mixture, then dip them in beaten egg, and finally coat evenly with bread crumbs.\n\n"
            "3. Preheat the air fryer to 400°F (approx. 200°C) for 1 minute.\n\n"
            "4. Place the onion rings in the air fryer, set the temperature to 375°F (approx. 190°C), and cook for 10–12 minutes."
        ),
        FR(
            "1. Peler l’oignon et le couper en rondelles de 1,5 cm d’épaisseur. Séparer les anneaux et les faire tremper dans l’eau froide pendant 10 minutes. Égoutter et sécher complètement avec du papier absorbant.\n\n"
            "2. Enrober les anneaux d’oignon d’abord dans le mélange de farine et d’épices, puis les tremper dans l’œuf battu, et enfin les recouvrir uniformément de chapelure.\n\n"
            "3. Préchauffer la friteuse à air chaud à 200°C pendant 1 minute.\n\n"
            "4. Déposer les anneaux d’oignon dans la friteuse, régler la température sur 190°C et cuire pendant 10 à 12 minutes."
        ),
        SP(
            "1. Pelar la cebolla y cortarla en aros de 1,5 cm de grosor. Separar los aros y remojarlos en agua fría durante 10 minutos. Escurrir y secar completamente con papel de cocina.\n\n"
            "2. Pasar los aros de cebolla primero por la mezcla de harina y especias, luego por huevo batido, y finalmente cubrirlos uniformemente con pan rallado.\n\n"
            "3. Precalentar la freidora de aire a 200°C durante 1 minuto.\n\n"
            "4. Colocar los aros de cebolla en la freidora, ajustar la temperatura a 190°C y cocinar durante 10–12 minutos."
        ),
        JA(
            "1. 玉ねぎの皮をむき、1.5cm厚の輪切りにしてリング状に分けます。水に10分浸し、取り出してペーパーでしっかり水気を拭き取ります。\n\n"
            "2. 玉ねぎリングに粉（調味料を混ぜたもの）→溶き卵→パン粉の順にしっかり付けます。\n\n"
            "3. エアフライヤーを400°F（約200°C）で1分予熱します。\n\n"
            "4. 玉ねぎリングを入れ、375°F（約190°C）で10〜12分加熱します。"
        ),
        KO(
            "1. 양파 껍질을 벗기고 1.5cm 두께로 썰어 링을 분리합니다. 찬물에 10분 담갔다가 건져 키친타월로 물기를 완전히 제거하세요.\n\n"
            "2. 양파링을 밀가루+양념 → 달걀물 → 빵가루 순서로 고르게 묻힙니다.\n\n"
            "3. 에어프라이어를 400°F(약 200°C)로 1분 예열하세요.\n\n"
            "4. 양파링을 넣고 375°F(약 190°C)에서 10~12분 조리하세요."
        ),
        DE(
            "1. Zwiebel schälen und in 1,5 cm dicke Ringe schneiden. Die Ringe trennen und 10 Minuten in kaltem Wasser einweichen. Herausnehmen und gründlich mit Küchenpapier trocken tupfen.\n\n"
            "2. Die Zwiebelringe zuerst in der Mehl-Gewürz-Mischung wenden, dann in verquirltem Ei tauchen und zuletzt gleichmäßig mit Semmelbröseln panieren.\n\n"
            "3. Die Heißluftfritteuse auf 200°C für 1 Minute vorheizen.\n\n"
            "4. Die Zwiebelringe in die Heißluftfritteuse legen, Temperatur auf 190°C einstellen und für 10–12 Minuten garen."
        ),
        IT(
            "1. Sbucciare la cipolla e tagliarla in anelli spessi 1,5 cm. Separare gli anelli e immergerli in acqua fredda per 10 minuti. Scolare e asciugare completamente con carta da cucina.\n\n"
            "2. Passare gli anelli di cipolla prima nel miscuglio di farina e spezie, poi nell’uovo sbattuto, e infine ricoprirli uniformemente con il pan grattato.\n\n"
            "3. Preriscaldare la friggitrice ad aria a 200°C per 1 minuto.\n\n"
            "4. Posizionare gli anelli di cipolla nella friggitrice, impostare la temperatura a 190°C e cuocere per 10–12 minuti."
        )
    ),

};



// 设置语言：（zh中文 en英文 fr法语 sp西班牙语 ja日本 ko韩国 de德语 it意大利语）
void yk_f_set_language(const char* lang) {
    current_lang  = lang;
    // 更新所有界面元素的文本
    yk_f_update_ui_text();
}

// 获取翻译
const char* yk_f_get_translation(const char* key) {
    for (size_t i = 0; i < sizeof(translations) / sizeof(TranslationPair); ++i) {
        if (strcmp(translations[i].key, key) == 0) {
            const char *selected = NULL;
            if (strcmp(current_lang, "zh") == 0) selected = translations[i].zh;
            else if (strcmp(current_lang, "en") == 0) selected = translations[i].en;
            else if (strcmp(current_lang, "fr") == 0) selected = translations[i].fr;
            else if (strcmp(current_lang, "sp") == 0) selected = translations[i].sp;
            else if (strcmp(current_lang, "ja") == 0) selected = translations[i].ja;
            else if (strcmp(current_lang, "ko") == 0) selected = translations[i].ko;
            else if (strcmp(current_lang, "de") == 0) selected = translations[i].de;
            else if (strcmp(current_lang, "it") == 0) selected = translations[i].it;

            if (selected && selected[0] != '\0') return selected;
            if (translations[i].en && translations[i].en[0] != '\0') return translations[i].en;
            if (translations[i].zh && translations[i].zh[0] != '\0') return translations[i].zh;
            return translations[i].key;
        }
    }
    return key ? key : "";
}
